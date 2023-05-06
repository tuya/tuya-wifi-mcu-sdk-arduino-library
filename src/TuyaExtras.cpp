/*
 * @FileName: TuyaExtras.h
 * @Author: Tuya
 * @Email:
 * @LastEditors: shiliu
 * @Date: 2021-10-30 15:50:27
 * @LastEditTime: 2021-11-04 11:47:47
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Description: 
 */
#include "TuyaExtras.h"
#include "TuyaDefs.h"
#include "TuyaUart.h"

extern TuyaUart tuya_uart;

#if SUPPORT_GREEN_TIME
void TuyaExtras::mcu_request_green_time(void)
{
    tuya_uart.wifi_uart_write_frame(GET_ONLINE_TIME_CMD, MCU_TX_VER, 0);
}

void TuyaExtras::mcu_get_green_time(unsigned char time[], TUYA_WIFI_TIME *tuya_time)
{
    if (time == TY_NULL || tuya_time == TY_NULL) {
        return;
    }

    if(time[0] == 1) {
        tuya_time->update_flag = 1;
        tuya_time->year = 2000+time[1];
        tuya_time->month = time[2];
        tuya_time->day = time[3];
        tuya_time->hour = time[4];
        tuya_time->minute = time[5];
        tuya_time->second = time[6];
        tuya_time->weekday = 0;
    }else {
        tuya_time->update_flag = -1;
    }
}
#endif /* SUPPORT_GREEN_TIME */

#if SUPPORT_RTC_TIME
void TuyaExtras::mcu_request_rtc_time(void)
{
    tuya_uart.wifi_uart_write_frame(GET_LOCAL_TIME_CMD, MCU_TX_VER, 0);
}

void TuyaExtras::mcu_get_rtc_time(unsigned char time[], TUYA_WIFI_TIME *tuya_time)
{
    if (time == TY_NULL || tuya_time == TY_NULL) {
        return;
    }

    if(time[0] == 1) {
        tuya_time->update_flag = 1;
        tuya_time->year = time[1];
        tuya_time->month = time[2];
        tuya_time->day = time[3];
        tuya_time->hour = time[4];
        tuya_time->minute = time[5];
        tuya_time->second = time[6];
        tuya_time->weekday = time[7];
    }else {
        tuya_time->update_flag = -1;
    }
}
#endif /* SUPPORT_RTC_TIME */
