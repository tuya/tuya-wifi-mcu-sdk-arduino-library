/*
 * @FileName: TuyaExtras.h 
 * @Author: Tuya
 * @Email:
 * @LastEditors: shiliu
 * @Date: 2021-10-30 15:49:00
 * @LastEditTime: 2021-11-01 19:31:15
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Description: 
 */
#ifndef __TUYA_EXTRAS_H__
#define __TUYA_EXTRAS_H__

#include "../config.h"

extern "C"
{
#if SUPPORT_GREEN_TIME
    typedef struct
    {
        unsigned short year;
        unsigned char month;
        unsigned char day;
        unsigned char hour;
        unsigned char minute;
        unsigned char second;
        unsigned char weekday;

        char update_flag;
    }TUYA_WIFI_TIME;
#endif /* SUPPORT_GREEN_TIME */
}

class TuyaExtras
{
public:
#if SUPPORT_GREEN_TIME
    void mcu_request_green_time(void);
    void mcu_get_green_time(unsigned char time[], TUYA_WIFI_TIME *tuya_time);
#endif /* SUPPORT_GREEN_TIME */

#if SUPPORT_RTC_TIME
    void mcu_request_rtc_time(void);
    void mcu_get_rtc_time(unsigned char time[], TUYA_WIFI_TIME *tuya_time);
#endif /* SUPPORT_RTC_TIME */

private:
};

#endif /* __TUYA_EXTRA_H__ */
