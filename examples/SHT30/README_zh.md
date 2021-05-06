# SHT30 

[English](./README.md) | [中文](./README_zh.md) 

该库仅支持烧录了通用固件的涂鸦 WiFi+BLE 、WiFi模组，在Arduino UNO板子上验证通过。

**注意：Arduino 中的 默认Serial 串口已被Tuya mcu sdk 接管，请不要对默认Serial（引脚 0 ，1）做任何操作。** 

## 一、demo 介绍 

涂鸦模组和Arduino板连接时会干扰到对Arduino板的烧录，烧录时应断开Arduino板与涂鸦通信板或涂鸦通信板的连接。例程烧录成功后将Arduino板上串口(Serial) 与 涂鸦模组上的 `RXD` 和 `TXD` 连接起来，然后将UNO板子上的 `Pin7` 脚拉低1s左右（模拟按键按下），设备将会进入配网模式，在进入配网状态和配网过程中Arduino板上LED将会闪烁，打开涂鸦智能APP对设备（Arduino_SHT30）进行配网操作，连接成功后Arduino板上LED将会停止闪烁，配网成功后即可通过APP对设备进行控制。

本例程硬件使用了Arduino板+涂鸦wifi通信板+SHT30温湿度传感器，设备每隔1秒上传一次当前环境下温湿度值，可通过涂鸦智能APP配网后查看。

## 二、功能实现 

在[涂鸦IoT平台](https://iot.tuya.com/?_source=97c44038fafc20e9c8dd5fdb508cc9c2) 创建完产品后，代码部分的实现主要可以分为配网、设备信息初始化、获取SHT30传感信息三大部分。

配网部分：

```c
/* Current LED status */
unsigned char led_state = 0;
/* Connect network button pin */
int wifi_key_pin = 7;
/* last time */
unsigned long last_time = 0;

void setup()
{
    ...
   /* 相关外设初始化 */
  //Initialize led port, turn off led.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  //Initialize networking keys.
  pinMode(wifi_key_pin, INPUT_PULLUP);
    ...
  last_time = millis();
}

void loop()
{
    ...
  //Enter the connection network mode when Pin7 is pressed.
  if (digitalRead(wifi_key_pin) == LOW) {
    delay(80);
    if (digitalRead(wifi_key_pin) == LOW) {
      my_device.mcu_set_wifi_mode(SMART_CONFIG);
    }
  }
  /* LED blinks when network is being connected */
  if ((my_device.mcu_get_wifi_work_state() != WIFI_LOW_POWER) && (my_device.mcu_get_wifi_work_state() != WIFI_CONN_CLOUD) && (my_device.mcu_get_wifi_work_state() != WIFI_SATE_UNKNOW)) {
    if (millis()- last_time >= 500) {
      last_time = millis();

      if (led_state == LOW) {
        led_state = HIGH;
      } else {
        led_state = LOW;
      }

      digitalWrite(LED_BUILTIN, led_state);
    }
      ...
}
```



设备信息初始化：

```c
#include <TuyaWifi.h>

TuyaWifi my_device;

/* Data point define */
#define DPID_TEMP_CURRENT     1
#define DPID_HUMIDITY_CURRENT 2

/* Current device DP values */
int temperature = 0;
int humidity = 0;

/* Stores all DPs and their types. PS: array[][0]:dpid, array[][1]:dp type. 
 *                                     dp type(TuyaDefs.h) : DP_TYPE_RAW, DP_TYPE_BOOL, DP_TYPE_VALUE, DP_TYPE_STRING, DP_TYPE_ENUM, DP_TYPE_BITMAP
*/
unsigned char dp_array[][2] =
{
  {DPID_TEMP_CURRENT, DP_TYPE_VALUE},
  {DPID_HUMIDITY_CURRENT, DP_TYPE_VALUE},
};

unsigned char pid[] = {"xxxxxxxxxxxxxxxx"};//xxxxxxxxxxxxxxxx 应为你的PID
unsigned char mcu_ver[] = {"1.0.0"};

void setup()
{
  Serial.begin(9600);
    ...
  my_device.init(pid, mcu_ver);
  //incoming all DPs and their types array, DP numbers
  my_device.set_dp_cmd_total(dp_array, 2);
  //register DP download processing callback function
  my_device.dp_process_func_register(dp_process);
  //register upload all DP callback function
  my_device.dp_update_all_func_register(dp_update_all);
    ...
}

void loop()
{
  my_device.uart_service();
    ...
  delay(1000);
}

/**
 * @description: DP download callback function.
 * @param {unsigned char} dpid
 * @param {const unsigned char} value
 * @param {unsigned short} length
 * @return {unsigned char}
 */
unsigned char dp_process(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  /* all DP only report */
  return SUCCESS;
}

/**
 * @description: Upload all DP status of the current device.
 * @param {*}
 * @return {*}
 */
void dp_update_all(void)
{
  my_device.mcu_dp_update(DPID_TEMP_CURRENT, temperature, 1);
  my_device.mcu_dp_update(DPID_HUMIDITY_CURRENT, humidity, 1);
}
```



获取SHT30传感器信息：

```c
#include <Wire.h>

/* SHT30 */
#define SHT30_I2C_ADDR 0x44

/* Current device DP values */
int temperature = 0;
int humidity = 0;

void setup()
{
    ...
  // Initialise I2C communication as MASTER
  Wire.begin();
    ...
}

void loop()
{
    ...
  /* get the temperature and humidity */
  get_sht30_value(&temperature, &humidity);

  if ((my_device.mcu_get_wifi_work_state() == WIFI_CONNECTED) || (my_device.mcu_get_wifi_work_state() == WIFI_CONN_CLOUD)) {
    my_device.mcu_dp_update(DPID_TEMP_CURRENT, temperature, 1);
    my_device.mcu_dp_update(DPID_HUMIDITY_CURRENT, humidity, 1);
  }

  delay(1000);
}

void get_sht30_value(int *temp_value, int *humi_value)
{
  unsigned char i2c_data[6];

  // Start I2C Transmission
  Wire.beginTransmission(SHT30_I2C_ADDR);
  // Send measurement command
  Wire.write(0x2C);
  Wire.write(0x06);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(500);

  // Request 6 bytes of data
  Wire.requestFrom(SHT30_I2C_ADDR, 6);

  // Read 6 bytes of i2c_data
  // temperature msb, temperature lsb, temperature crc, humidity msb, humidity lsb, humidity crc
  if (Wire.available() == 6) {
    for (int i = 0; i < 6 ; i++) {
      i2c_data[i] = Wire.read();
    }
    
    if ((sht30_crc(i2c_data, 2) == i2c_data[2]) && (sht30_crc(i2c_data+3, 2) == i2c_data[5])) {/* crc success */
      *temp_value = (((((i2c_data[0] * 256.0) + i2c_data[1]) * 175) / 65535.0) - 45) * 100;
      *humi_value = ((((i2c_data[3] * 256.0) + i2c_data[4]) * 100) / 65535.0) * 100;
    } else {
      *temp_value = 0;
      *humi_value = 0;
    }
  }
}

/**
 * @description: check sht30 temperature and humidity data
 * @param {unsigned char} *data
 * @param {unsigned int} count
 * @return {*}
 */
unsigned char sht30_crc(unsigned char *data, unsigned int count)
{
    unsigned char crc = 0xff;
    unsigned char current_byte;
    unsigned char crc_bit;

    /* calculates 8-Bit checksum with given polynomial */
    for (current_byte = 0; current_byte < count; ++current_byte)
    {
        crc ^= (data[current_byte]);
        for (crc_bit = 8; crc_bit > 0; --crc_bit)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc = (crc << 1);
        }
    }
    return crc;
}
```



## 三、技术支持

您可以通过以下方法获得涂鸦的支持:

- 开发者中心：https://developer.tuya.com
- 帮助中心: https://support.tuya.com/cn/help
- 技术支持工单中心: https://service.console.tuya.com 