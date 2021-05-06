/*
 * @FileName: TuyaUart.h
 * @Author: Tuya
 * @Email: 
 * @LastEditors: Tuya
 * @Date: 2021-04-15 17:56:56
 * @LastEditTime: 2021-04-28 19:50:07
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Description: 
 */
#ifndef __TUYA_UART_H__
#define __TUYA_UART_H__

#include "../config.h"
#include "TuyaDefs.h"
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include <Stream.h>

class TuyaUart
{
public:
    volatile unsigned char wifi_uart_rx_buf[PROTOCOL_HEAD + WIFI_UART_RECV_BUF_LMT];     //Serial data processing buffer
    volatile unsigned char wifi_uart_tx_buf[PROTOCOL_HEAD + WIFIR_UART_SEND_BUF_LMT];    //Serial receive buffer
    volatile unsigned char wifi_data_process_buf[PROTOCOL_HEAD + WIFI_DATA_PROCESS_LMT]; //Serial port send buffer

    TuyaUart(void);
    ~TuyaUart(void);
    void wifi_protocol_init(void);

    unsigned char uart_receive_input(unsigned char data);
    void uart_receive_buff_input(unsigned char value[], unsigned short data_len);
    unsigned char take_byte_rxbuff(void);
    unsigned char with_data_rxbuff(void);

    void uart_transmit_output(unsigned char value);
    void wifi_uart_write_data(unsigned char *in, unsigned short len);
    void wifi_uart_write_frame(unsigned char fr_type, unsigned char fr_ver, unsigned short len);
    unsigned short set_wifi_uart_byte(unsigned short dest, unsigned char byte);
    unsigned short set_wifi_uart_buffer(unsigned short dest, const unsigned char *src, unsigned short len);

    /* serial set */
    bool _isHWSerial;

    void set_serial(HardwareSerial *serial);
    void set_serial(SoftwareSerial *serial);
    void begin(long baud_rate);
    char read(void);
    size_t write(char value);
    int available(void);

private:
    volatile unsigned char *rx_buf_in;
    volatile unsigned char *rx_buf_out;
    Stream *_port;
};

#endif /* __TUYA_UART_H__ */
