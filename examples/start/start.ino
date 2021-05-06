/*
 * @FileName: start.ino
 * @Author: Tuya
 * @Email: 
 * @LastEditors: Tuya
 * @Date: 2021-04-10 11:24:27
 * @LastEditTime: 2021-04-28 19:48:31
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Description: This demo is based on the Arduino UNO, and the LEDs on the UNO board are controlled by the Tuya Smart App. 
 *               Enter network connection mode when Pin7 to GND.
 * @Github:https://github.com/tuya/tuya-wifi-mcu-sdk-arduino-library
 */

#include <TuyaWifi.h>
#include <SoftwareSerial.h>

TuyaWifi my_device;


/* Current LED status */
unsigned char led_state = 0;
/* Connect network button pin */
int key_pin = 7;

/* Data point define */
#define DPID_SWITCH 20

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
  // Serial.begin(9600);
  Serial.begin(9600);

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
  return SUCCESS;
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
