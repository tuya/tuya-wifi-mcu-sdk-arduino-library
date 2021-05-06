/*
 * @FileName: TuyaDataPoint.cpp 
 * @Author: Tuya
 * @Email: 
 * @LastEditors: Tuya
 * @Date: 2021-04-10 15:34:17
 * @LastEditTime: 2021-04-28 19:48:05
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Description: tuya mcu sdk Arduino library about data point process. 
 */
#include "TuyaDataPoint.h"
#include "TuyaDefs.h"
#include "TuyaTools.h"
#include "TuyaUart.h"
#include <TuyaWifi.h>

extern TuyaTools tuya_tools;
extern TuyaUart tuya_uart;

unsigned char TuyaDataPoint::mcu_get_dp_download_bool(const unsigned char value[], unsigned short len)
{
    return (value[0]);
}

unsigned char TuyaDataPoint::mcu_get_dp_download_enum(const unsigned char value[], unsigned short len)
{
    return (value[0]);
}

unsigned long TuyaDataPoint::mcu_get_dp_download_value(const unsigned char value[], unsigned short len)
{
    return (tuya_tools.byte_to_int(value));
}

unsigned char TuyaDataPoint::mcu_dp_raw_update(unsigned char dpid, const unsigned char value[], unsigned short len)
{
    unsigned short send_len = 0;

    if (stop_update_flag == ENABLE)
        return SUCCESS;
    //
    send_len = tuya_uart.set_wifi_uart_byte(send_len, dpid);
    send_len = tuya_uart.set_wifi_uart_byte(send_len, DP_TYPE_RAW);
    //
    send_len = tuya_uart.set_wifi_uart_byte(send_len, len / 0x100);
    send_len = tuya_uart.set_wifi_uart_byte(send_len, len % 0x100);
    //
    send_len = tuya_uart.set_wifi_uart_buffer(send_len, (unsigned char *)value, len);

    tuya_uart.wifi_uart_write_frame(STATE_UPLOAD_CMD, MCU_TX_VER, send_len);

    return SUCCESS;
}

unsigned char TuyaDataPoint::mcu_dp_bool_update(unsigned char dpid, unsigned char value)
{
    unsigned short send_len = 0;

    if (stop_update_flag == ENABLE)
        return SUCCESS;

    send_len = tuya_uart.set_wifi_uart_byte(send_len, dpid);
    send_len = tuya_uart.set_wifi_uart_byte(send_len, DP_TYPE_BOOL);
    //
    send_len = tuya_uart.set_wifi_uart_byte(send_len, 0);
    send_len = tuya_uart.set_wifi_uart_byte(send_len, 1);
    //
    if (value == FALSE)
    {
        send_len = tuya_uart.set_wifi_uart_byte(send_len, FALSE);
    }
    else
    {
        send_len = tuya_uart.set_wifi_uart_byte(send_len, 1);
    }

    tuya_uart.wifi_uart_write_frame(STATE_UPLOAD_CMD, MCU_TX_VER, send_len);

    return SUCCESS;
}

unsigned char TuyaDataPoint::mcu_dp_value_update(unsigned char dpid, unsigned long value)
{
    unsigned short send_len = 0;

    if (stop_update_flag == ENABLE)
        return SUCCESS;

    send_len = tuya_uart.set_wifi_uart_byte(send_len, dpid);
    send_len = tuya_uart.set_wifi_uart_byte(send_len, DP_TYPE_VALUE);
    //
    send_len = tuya_uart.set_wifi_uart_byte(send_len, 0);
    send_len = tuya_uart.set_wifi_uart_byte(send_len, 4);
    //
    send_len = tuya_uart.set_wifi_uart_byte(send_len, value >> 24);
    send_len = tuya_uart.set_wifi_uart_byte(send_len, value >> 16);
    send_len = tuya_uart.set_wifi_uart_byte(send_len, value >> 8);
    send_len = tuya_uart.set_wifi_uart_byte(send_len, value & 0xff);

    tuya_uart.wifi_uart_write_frame(STATE_UPLOAD_CMD, MCU_TX_VER, send_len);

    return SUCCESS;
}

unsigned char TuyaDataPoint::mcu_dp_string_update(unsigned char dpid, const unsigned char value[], unsigned short len)
{
    unsigned short send_len = 0;

    if (stop_update_flag == ENABLE)
        return SUCCESS;
    //
    send_len = tuya_uart.set_wifi_uart_byte(send_len, dpid);
    send_len = tuya_uart.set_wifi_uart_byte(send_len, DP_TYPE_STRING);
    //
    send_len = tuya_uart.set_wifi_uart_byte(send_len, len / 0x100);
    send_len = tuya_uart.set_wifi_uart_byte(send_len, len % 0x100);
    //
    send_len = tuya_uart.set_wifi_uart_buffer(send_len, (unsigned char *)value, len);

    tuya_uart.wifi_uart_write_frame(STATE_UPLOAD_CMD, MCU_TX_VER, send_len);

    return SUCCESS;
}

unsigned char TuyaDataPoint::mcu_dp_enum_update(unsigned char dpid, unsigned char value)
{
    unsigned short send_len = 0;

    if (stop_update_flag == ENABLE)
        return SUCCESS;

    send_len = tuya_uart.set_wifi_uart_byte(send_len, dpid);
    send_len = tuya_uart.set_wifi_uart_byte(send_len, DP_TYPE_ENUM);
    //
    send_len = tuya_uart.set_wifi_uart_byte(send_len, 0);
    send_len = tuya_uart.set_wifi_uart_byte(send_len, 1);
    //
    send_len = tuya_uart.set_wifi_uart_byte(send_len, value);

    tuya_uart.wifi_uart_write_frame(STATE_UPLOAD_CMD, MCU_TX_VER, send_len);

    return SUCCESS;
}

unsigned char TuyaDataPoint::mcu_dp_fault_update(unsigned char dpid, unsigned long value)
{
    unsigned short send_len = 0;

    if (stop_update_flag == ENABLE)
        return SUCCESS;

    send_len = tuya_uart.set_wifi_uart_byte(send_len, dpid);
    send_len = tuya_uart.set_wifi_uart_byte(send_len, DP_TYPE_BITMAP);
    //
    send_len = tuya_uart.set_wifi_uart_byte(send_len, 0);

    if ((value | 0xff) == 0xff)
    {
        send_len = tuya_uart.set_wifi_uart_byte(send_len, 1);
        send_len = tuya_uart.set_wifi_uart_byte(send_len, value);
    }
    else if ((value | 0xffff) == 0xffff)
    {
        send_len = tuya_uart.set_wifi_uart_byte(send_len, 2);
        send_len = tuya_uart.set_wifi_uart_byte(send_len, value >> 8);
        send_len = tuya_uart.set_wifi_uart_byte(send_len, value & 0xff);
    }
    else
    {
        send_len = tuya_uart.set_wifi_uart_byte(send_len, 4);
        send_len = tuya_uart.set_wifi_uart_byte(send_len, value >> 24);
        send_len = tuya_uart.set_wifi_uart_byte(send_len, value >> 16);
        send_len = tuya_uart.set_wifi_uart_byte(send_len, value >> 8);
        send_len = tuya_uart.set_wifi_uart_byte(send_len, value & 0xff);
    }

    tuya_uart.wifi_uart_write_frame(STATE_UPLOAD_CMD, MCU_TX_VER, send_len);

    return SUCCESS;
}
