/*
 * @FileName: TuyaUart.cpp
 * @Author: Tuya
 * @Email: 
 * @LastEditors: shiliu
 * @Date: 2021-04-10 15:34:37
 * @LastEditTime: 2021-11-04 11:03:14
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Description: Tuya mcu sdk Arduino library about uart buffer, data receiving and sending.
 */
#include "TuyaWifi.h"
#include "TuyaUart.h"
#include "TuyaTools.h"

extern TuyaTools tuya_tools;

TuyaUart::TuyaUart(void)
{
    wifi_protocol_init();
}

TuyaUart::~TuyaUart(void)
{
}

void TuyaUart::wifi_protocol_init(void)
{
    rx_buf_in = (unsigned char *)wifi_uart_rx_buf;
    rx_buf_out = (unsigned char *)wifi_uart_rx_buf;

    stop_update_flag = TY_DISABLE;
}

void TuyaUart::uart_transmit_output(unsigned char value)
{
    write(value);
}

unsigned char TuyaUart::uart_receive_input(unsigned char data)
{
    if (1 == rx_buf_out - rx_buf_in)
    {
        //UART receive buffer is full
        return TY_ERROR;
    }
    else if ((rx_buf_in > rx_buf_out) && ((unsigned long)(rx_buf_in - rx_buf_out) >= sizeof(wifi_uart_rx_buf)))
    {
        //UART receive buffer is full
        return TY_ERROR;
    }
    else
    {
        //UART receive buffer is not full
        if (rx_buf_in >= (unsigned char *)(wifi_uart_rx_buf + sizeof(wifi_uart_rx_buf)))
        {
            rx_buf_in = (unsigned char *)(wifi_uart_rx_buf);
        }

        *rx_buf_in++ = data;
    }
    return TY_SUCCESS;
}

void TuyaUart::uart_receive_buff_input(unsigned char value[], unsigned short data_len)
{
    unsigned short i = 0;
    for (i = 0; i < data_len; i++)
    {
        uart_receive_input(value[i]);
    }
}

unsigned char TuyaUart::take_byte_rxbuff(void)
{
    unsigned char date = 0;

    if (rx_buf_out != rx_buf_in)
    {
        //With data
        if (rx_buf_out >= (unsigned char *)(wifi_uart_rx_buf + sizeof(wifi_uart_rx_buf)))
        {
            //The data has reached the end
            rx_buf_out = (unsigned char *)(wifi_uart_rx_buf);
        }

        date = *rx_buf_out++;
    }

    return date;
}

unsigned char TuyaUart::with_data_rxbuff(void)
{
    if (rx_buf_in != rx_buf_out)
        return 1;
    else
        return 0;
}

void TuyaUart::wifi_uart_write_data(unsigned char *in, unsigned short len)
{
    if ((TY_NULL == in) || (0 == len))
    {
        return;
    }

    while (len--)
    {
        uart_transmit_output(*in);
        in++;
    }
}

void TuyaUart::wifi_uart_write_frame(unsigned char fr_type, unsigned char fr_ver, unsigned short len)
{
    unsigned char check_sum = 0;

    wifi_uart_tx_buf[HEAD_FIRST] = 0x55;
    wifi_uart_tx_buf[HEAD_SECOND] = 0xaa;
    wifi_uart_tx_buf[PROTOCOL_VERSION] = fr_ver;
    wifi_uart_tx_buf[FRAME_TYPE] = fr_type;
    wifi_uart_tx_buf[LENGTH_HIGH] = len >> 8;
    wifi_uart_tx_buf[LENGTH_LOW] = len & 0xff;

    len += PROTOCOL_HEAD;
    check_sum = tuya_tools.get_check_sum((unsigned char *)wifi_uart_tx_buf, len - 1);
    wifi_uart_tx_buf[len - 1] = check_sum;

    wifi_uart_write_data((unsigned char *)wifi_uart_tx_buf, len);
}

unsigned short TuyaUart::set_wifi_uart_byte(unsigned short dest, unsigned char byte)
{
    unsigned char *obj = (unsigned char *)wifi_uart_tx_buf + DATA_START + dest;

    *obj = byte;
    dest += 1;

    return dest;
}

unsigned short TuyaUart::set_wifi_uart_buffer(unsigned short dest, const unsigned char *src, unsigned short len)
{
    unsigned char *obj = (unsigned char *)wifi_uart_tx_buf + DATA_START + dest;

    tuya_tools.my_memcpy(obj, src, len);

    dest += len;
    return dest;
}

void TuyaUart::set_serial(TY_UART *serial)
{
    _serial_port = serial;
}

void TuyaUart::begin(unsigned long baud_rate)
{
    _serial_port->begin(baud_rate);
}

int TuyaUart::read(void)
{
    return _serial_port->read();
}

size_t TuyaUart::write(uint8_t c)
{
    return _serial_port->write(c);
}

int TuyaUart::available(void)
{
    return _serial_port->available();
}
