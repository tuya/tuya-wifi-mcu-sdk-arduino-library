# start 
[English](./README.md) | [中文](./README_zh.md) 

该库仅支持烧录了通用固件的涂鸦 WiFi+BLE 、WiFi模组，在Arduino UNO板子上验证通过。

**注意：Arduino 中的 默认Serial 串口已被Tuya mcu sdk 接管，请不要对默认Serial（引脚 0 ，1）做任何操作。** 



## 一、demo 介绍 

涂鸦模组和Arduino板连接时会干扰到对Arduino板的烧录，烧录时应断开Arduino板与涂鸦通信板或涂鸦通信板的连接。例程烧录成功后将Arduino板上串口(Serial) 与 涂鸦模组上的 `RXD` 和 `TXD` 连接起来，然后将UNO板子上的 `Pin7` 脚拉低1s左右（模拟按键按下），设备将会进入配网模式，在进入配网状态和配网过程中Arduino板上LED将会闪烁，打开涂鸦智能APP对设备（Arduino_led）进行配网操作，连接成功后Arduino板上LED将会停止闪烁，配网成功后即可通过APP对设备进行控制。

本例程硬件使用了Arduino板+涂鸦wifi通信板，可通过涂鸦智能APP配网后对Arduino板上LED进行控制。



## 二、功能实现 

### 1、创建产品 

进入 [Tuya IoT平台](https://iot.tuya.com/?_source=97c44038fafc20e9c8dd5fdb508cc9c2) 创建产品：

![creat_produce1](https://images.tuyacn.com/smart/shiliu_zone/Tuya_Arduino_library/creat_produce1.png) 

选择品类，方案：

![creat_produce2](https://images.tuyacn.com/smart/shiliu_zone/Tuya_Arduino_library/creat_produce2.png)

完善产品信息，点击创建产品：

![creat_produce3](https://images.tuyacn.com/smart/shiliu_zone/Tuya_Arduino_library/creat_produce3.png)

根据自身需求选择相关功能，点击确认：

![creat_produce4](https://images.tuyacn.com/smart/shiliu_zone/Tuya_Arduino_library/creat_produce4.png)

选择设备面板后，进入硬件开发，选择 涂鸦标准模组 MCU SDK 开发方式，选择对应的模组：

![creat_produce6](https://images.tuyacn.com/smart/shiliu_zone/Tuya_Arduino_library/creat_produce6.png)

选择固件：

![creat_produce7](https://images.tuyacn.com/smart/shiliu_zone/Tuya_Arduino_library/creat_produce7.png)

### 2、重要函数说明

初始化串口，涂鸦WIFi模组的通用固件支持115200、9600波特率自适应，所以初始化串口时应将串口波特率初始化为115200或9600。在 `setup()` 中：

```c++
Serial.begin(9600);
```

mcu需要将创建的PID和mcu的软件版本号发送给涂鸦云模组。在 `setup()` 中：

```c++
  //Enter the PID and MCU software version
  my_device.init("ma67l9sgmdyg3d2k", "1.0.0");
```

我们需要将创建产品时选择的所DP 的 ID 和类型传入MCU SDK内使用：

```c++
TuyaWifi my_device;

#define DPID_SWITCH 20 //DP ID

unsigned char dp_array[][2] =
{
  //该产品创建时只选择了一个DP点，所以该数组只有一条信息
  {DPID_SWITCH, DP_TYPE_BOOL}, //前一个填入DP点ID，后一个为DP点的数据类型
};

void setup()
{
    ...
    my_device.set_dp_cmd_total(dp_array, 1); //dp_array:为存储DP点ID和数据类型的数组， 1：为数组内定义的DP个数
    ...
}

```

初始化时还需注册DP点下发处理函数和上传所有DP点函数：

```c++
unsigned char led_state = 0;

void setup() 
{
...
  //register DP download processing callback function
  my_device.dp_process_func_register(led_dp_process); //注册DP下发处理函数
  //register upload all DP callback function
  my_device.dp_update_all_func_register(dp_update_all);//注册设备状态函数，上传所有DP点
...
}

void loop()
{
    ...
    my_device.uart_service();
    ...
}

/**
 * @description: DP download callback function.
 * @param {unsigned char} dpid
 * @param {const unsigned char} value
 * @param {unsigned short} length
 * @return {unsigned char}
 */
unsigned char led_dp_process(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  switch(dpid) {
    case DPID_SWITCH:
      led_state = my_device.mcu_get_dp_download_data(dpid, value, length);//得到下发数据帧中该DP的数据
      if (led_state) {
        digitalWrite(LED_BUILTIN, HIGH);
      } else {
        digitalWrite(LED_BUILTIN, LOW);
      }
      my_device.mcu_dp_update(dpid, value, length);
    break;

    default:break;
  }
  return 1;
}

/**
 * @description: Upload all DP status of the current device.
 * @param {*}
 * @return {*}
 */
void dp_update_all(void)
{
  my_device.mcu_dp_update(DPID_SWITCH, &led_state, 1);
}
```



## 三、技术支持

您可以通过以下方法获得涂鸦的支持:

- 开发者中心：https://developer.tuya.com
- 帮助中心: https://support.tuya.com/help
- 技术支持工单中心: https://service.console.tuya.com 