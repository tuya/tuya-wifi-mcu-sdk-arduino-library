/*
 * @FileName: Tuya.h 
 * @Author: Tuya
 * @Email:
 * @LastEditors: Tuya
 * @Date: 2021-04-10 11:25:26
 * @LastEditTime: 2021-04-28 19:47:50
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Description: 
 */
#ifndef __TUYA_H__
#define __TUYA_H__

#include "../config.h"
#include "TuyaDefs.h"
#include "TuyaUart.h"

#ifdef TUYA_GLOBAL
#define TUYA_GLOBAL_EXTERN
#else
#define TUYA_GLOBAL_EXTERN extern
#endif

#ifndef TUYA_GLOBAL
extern TuyaUart tuya_uart;
#endif

#define PID_LEN 16
#define VER_LEN 5

TUYA_GLOBAL_EXTERN volatile unsigned char stop_update_flag; //ENABLE:Stop all data uploads   DISABLE:Restore all data uploads

#ifndef WIFI_CONTROL_SELF_MODE
TUYA_GLOBAL_EXTERN volatile unsigned char reset_wifi_flag;   //Reset wifi flag (TRUE: successful / FALSE: failed)
TUYA_GLOBAL_EXTERN volatile unsigned char set_wifimode_flag; //Set the WIFI working mode flag (TRUE: Success / FALSE: Failed)
TUYA_GLOBAL_EXTERN volatile unsigned char wifi_work_state;   //Wifi module current working status
#endif

extern "C"
{
    typedef void (*tuya_callback_dp_update_all)(void);
    typedef unsigned char (*tuya_callback_dp_download)(unsigned char dpid, const unsigned char value[], unsigned short length);
}

class TuyaWifi
{
public:
    TuyaWifi(void);
    TuyaWifi(HardwareSerial *serial);
    TuyaWifi(SoftwareSerial *serial);

    unsigned char init(unsigned char *pid, unsigned char *mcu_ver);
    void uart_service(void);

    void dp_process_func_register(tuya_callback_dp_download _func);
    void dp_update_all_func_register(tuya_callback_dp_update_all _func);

    void set_dp_cmd_total(unsigned char download_cmd_array[][2], unsigned char download_cmd_num);

    unsigned long mcu_get_dp_download_data(unsigned char dpid, const unsigned char value[], unsigned short len);

    /* char * */
    unsigned char mcu_dp_update(unsigned char dpid, const unsigned char value[], unsigned short len); /* char raw */
    /* unsigned long / long */
    unsigned char mcu_dp_update(unsigned char dpid, unsigned long value, unsigned short len);
    unsigned char mcu_dp_update(unsigned char dpid, long value, unsigned short len);
    /* unsigned char / char */
    unsigned char mcu_dp_update(unsigned char dpid, unsigned char value, unsigned short len);
    unsigned char mcu_dp_update(unsigned char dpid, char value, unsigned short len);
    /* unsigned int / int */
    unsigned char mcu_dp_update(unsigned char dpid, unsigned int value, unsigned short len);
    unsigned char mcu_dp_update(unsigned char dpid, int value, unsigned short len);

    void mcu_set_wifi_mode(unsigned char mode);
    unsigned char mcu_get_wifi_work_state(void);
    void mcu_reset_wifi(void);

private:
    unsigned char product_id[PID_LEN];
    unsigned char mcu_ver_value[VER_LEN];

    unsigned char download_dp_number;
    unsigned char (*download_cmd)[2];

    tuya_callback_dp_download dp_download_handle;
    tuya_callback_dp_update_all all_data_update;
    void data_handle(unsigned short offset);
    void heat_beat_check(void);
    void product_info_update(void);
    void get_mcu_wifi_mode(void);
    unsigned char data_point_handle(const unsigned char value[]);
    unsigned char get_dowmload_dpid_index(unsigned char dpid);
};

#endif /* __TUYA_H__ */
