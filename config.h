/*
 * @FileName: config.h
 * @Author: Tuya
 * @Email: 
 * @LastEditors: shiliu
 * @Date: 2021-04-10 11:24:27
 * @LastEditTime: 2021-11-04 14:00:42
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Description: Tuya mcu sdk Arduino library config file.
 */
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "TuyaDefs.h"

/*  Distribution mode selection, only three choices, Anti-touch mode is recommende */
#define CONFIG_MODE     CONFIG_MODE_DEFAULT     //Default working mode
// #define CONFIG_MODE     CONFIG_MODE_LOWPOWER    //Safe mode (low power working mode)
// #define CONFIG_MODE     CONFIG_MODE_SPECIAL     // Anti-touch mode (special working mode)

/* some extra features defined begin */
#define WIFI_CONTROL_SELF_MODE  0
#define SUPPORT_GREEN_TIME      1
#define SUPPORT_RTC_TIME        1
/* some extra features defined end */

#ifndef SUPPORT_MCU_FIRM_UPDATE
#define WIFI_UART_RECV_BUF_LMT  32      //UART data receiving buffer size, can be reduced if the MCU has insufficient RAM
#define WIFI_DATA_PROCESS_LMT   64      //UART data processing buffer size, according to the user DP data size, must be greater than 24
#else
#define WIFI_UART_RECV_BUF_LMT  128     //UART data receiving buffer size, can be reduced if the MCU has insufficient RAM
/*  Select the appropriate UART data processing buffer size here 
    (select the buffer size based on the size selected by the above MCU firmware upgrade package and whether to turn on the weather service)  */
#define WIFI_DATA_PROCESS_LMT   1000    //UART data processing buffer size. If the MCU firmware upgrade is required, the single-packet size is 256, the buffer must be greater than 260, or larger if the weather service is enabled
// #define WIFI_DATA_PROCESS_LMT   600     //UART data processing buffer size. If the MCU firmware upgrade is required, the single-packet size is 512, the buffer must be greater than 520, or larger if the weather service is enabled
// #define WIFI_DATA_PROCESS_LMT   1200    //UART data processing buffer size. If the MCU firmware upgrade is required, the single-packet size is 1024, the buffer must be greater than 1030, or larger if the weather service is enabled
#endif

#define WIFIR_UART_SEND_BUF_LMT 64      //According to the user's DP data size, it must be greater than 48


#endif /* __CONFIG_H__ */
