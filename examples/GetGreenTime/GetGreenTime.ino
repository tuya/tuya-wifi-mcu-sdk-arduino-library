/*
 * @FileName: GetGreenTime.ino
 * @Author: Tuya
 * @Email: 
 * @LastEditors: shiliu
 * @Date: 2021-11-04 14:44:25
 * @LastEditTime: 2021-11-04 14:50:21
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Description: get Greenwich Mean Time
 * @Github:https://github.com/tuya/tuya-wifi-mcu-sdk-arduino-library
 */
#include <TuyaWifi.h>

// STM32
// HardwareSerial Serial2(PA_3, PA_2);

TuyaWifi my_device;

/* Current LED status */
unsigned char led_state = 0;
/* Connect network button pin */
int key_pin = 7;

/* Data point define */
#define DPID_SWITCH 20

TUYA_WIFI_TIME green_time;
unsigned long last_get_green_time;

/* Stores all DPs and their types. PS: array[][0]:dpid, array[][1]:dp type. 
 *                                     dp type(TuyaDefs.h) : DP_TYPE_RAW, DP_TYPE_BOOL, DP_TYPE_VALUE, DP_TYPE_STRING, DP_TYPE_ENUM, DP_TYPE_BITMAP
*/
unsigned char dp_array[][2] =
{
  {DPID_SWITCH, DP_TYPE_BOOL},
};

unsigned char pid[] = {"ma67l9sgmdyg3d2k"};
unsigned char mcu_ver[] = {"1.0.0"};

/* last time */
unsigned long last_time = 0;

void setup() 
{
  Serial.begin(9600);
  Serial2.begin(9600);

  //Initialize led port, turn off led.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //Initialize networking keys.
  pinMode(key_pin, INPUT_PULLUP);

  //Enter the PID and MCU software version
  my_device.init(pid, mcu_ver);
  //incoming all DPs and their types array, DP numbers
  my_device.set_dp_cmd_total(dp_array, 1);
  //register DP download processing callback function
  my_device.dp_process_func_register(dp_process);
  //register upload all DP callback function
  my_device.dp_update_all_func_register(dp_update_all);

  last_time = millis();
}

void loop() 
{
  my_device.uart_service();

  //Enter the connection network mode when Pin7 is pressed.
  if (digitalRead(key_pin) == LOW) {
    delay(80);
    if (digitalRead(key_pin) == LOW) {
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
  }

  /* 5s get Greenwich Mean Time */
  if (millis() - last_get_green_time >= 3000) {
    last_get_green_time = millis();
    if (TY_SUCCESS == my_device.get_green_time(&green_time, 100)) { /* if network lag, you can increase the timeout */
      Serial2.print(green_time.year);
      Serial2.print("-");
      Serial2.print(green_time.month);
      Serial2.print("-");
      Serial2.println(green_time.day);

      Serial2.print(green_time.hour);
      Serial2.print(":");
      Serial2.print(green_time.minute);
      Serial2.print(":");
      Serial2.println(green_time.second);
    } else {
      Serial2.println("get green time failed");
    }
  }

  delay(10);
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
  switch(dpid) {
    case DPID_SWITCH:
      led_state = my_device.mcu_get_dp_download_data(dpid, value, length); /* Get the value of the down DP command */
      if (led_state) {
        //Turn on
        digitalWrite(LED_BUILTIN, HIGH);
      } else {
        //Turn off
        digitalWrite(LED_BUILTIN, LOW);
      }
      //Status changes should be reported.
      my_device.mcu_dp_update(dpid, value, length);
    break;

    default:break;
  }
  return TY_SUCCESS;
}

/**
 * @description: Upload all DP status of the current device.
 * @param {*}
 * @return {*}
 */
void dp_update_all(void)
{
  my_device.mcu_dp_update(DPID_SWITCH, led_state, 1);
}

