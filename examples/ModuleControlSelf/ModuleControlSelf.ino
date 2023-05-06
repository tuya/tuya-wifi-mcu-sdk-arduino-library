/*
 * @FileName: start.ino
 * @Author: Tuya
 * @Email: 
 * @LastEditors: shiliu
 * @Date: 2021-11-04 11:06:13
 * @LastEditTime: 2021-11-04 14:29:10
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Description: Note: Please read the readme!!!
 *               Note: Please read the readme!!!
 *               Note: Please read the readme!!!
 * @Github:https://github.com/tuya/tuya-wifi-mcu-sdk-arduino-library
 */

#include <TuyaWifi.h>

TuyaWifi my_device;

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

void setup() 
{
  // Serial.begin(9600);
  Serial.begin(9600);

  //Enter the PID and MCU software version
  my_device.init(pid, mcu_ver);
  //incoming all DPs and their types array, DP numbers
  my_device.set_dp_cmd_total(dp_array, 1);
  //register DP download processing callback function
  my_device.dp_process_func_register(dp_process);
  //register upload all DP callback function
  my_device.dp_update_all_func_register(dp_update_all);

  /* set tuya module pin 16 as led, set tuya module pin 28 as connect wifi key.
   * Long press 5s to enter networking mode.
  */
  my_device.set_state_pin(16, 28);
}

void loop() 
{
  my_device.uart_service();

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
