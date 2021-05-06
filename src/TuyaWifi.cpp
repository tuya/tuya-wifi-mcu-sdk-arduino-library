/*
 * @FileName: Tuya.cpp
 * @Author: Tuya
 * @Email: 
 * @LastEditors: Tuya
 * @Date: 2021-04-10 11:25:17
 * @LastEditTime: 2021-04-28 19:50:21
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Description: The functions that the user needs to actively call are in this file.
 */

#define TUYA_GLOBAL

#include <Arduino.h>
#include <TuyaWifi.h>
#include "TuyaTools.h"
#include "TuyaDataPoint.h"

TuyaTools tuya_tools;
TuyaUart tuya_uart;
TuyaDataPoint tuya_dp;

/* Constants required to report product information */
unsigned char pid_key[] = {"{\"p\":\""}; 
unsigned char mcu_ver_key[] = {"\",\"v\":\""};
unsigned char mode_key[] = {"\",\"m\":"};
unsigned char product_info_end[] = {"}"};

/* Protocol serial port initialization */
TuyaWifi::TuyaWifi(void)
{
    tuya_uart.set_serial(&Serial);
}

TuyaWifi::TuyaWifi(HardwareSerial *serial)
{
    tuya_uart.set_serial(serial);
}

TuyaWifi::TuyaWifi(SoftwareSerial *serial)
{
    tuya_uart.set_serial(serial);
}

/**
 * @description: Initialize product information
 * @param {unsigned char} *pid : Product ID(Create products on the Tuya IoT platform to get)
 * @param {unsigned char} *mcu_ver : MCU Software Version Number
 * @return {*}
 */
unsigned char TuyaWifi::init(unsigned char *pid, unsigned char *mcu_ver)
{
    if (pid == NULL || mcu_ver == NULL)
    {
        return ERROR;
    }

    if (tuya_tools.my_strlen(pid) <= PID_LEN)
    {
        tuya_tools.my_memcpy(product_id, pid, tuya_tools.my_strlen(pid));
    }
    else
    {
        tuya_tools.my_memcpy(product_id, pid, PID_LEN);
        return ERROR;
    }

    if (tuya_tools.my_strlen(mcu_ver) <= VER_LEN)
    {
        tuya_tools.my_memcpy(mcu_ver_value, mcu_ver, tuya_tools.my_strlen(mcu_ver));
    }
    else
    {
        tuya_tools.my_memcpy(mcu_ver_value, mcu_ver, VER_LEN);
        return ERROR;
    }

    return SUCCESS;
}

/**
 * @description: Wifi serial port processing service
 * @param {*}
 * @return {*}
 */
void TuyaWifi::uart_service(void)
{
    unsigned char ret;
    static unsigned short rx_in = 0;
    unsigned short offset = 0;
    unsigned short rx_value_len = 0;

    /* extract serial data */
    while(tuya_uart.available()) {
        ret = tuya_uart.uart_receive_input(tuya_uart.read());
        if (ret != SUCCESS) {
            break;
        }
    }

    while ((rx_in < sizeof(tuya_uart.wifi_data_process_buf)) && tuya_uart.with_data_rxbuff() > 0)
    {
        tuya_uart.wifi_data_process_buf[rx_in++] = tuya_uart.take_byte_rxbuff();
    }

    if (rx_in < PROTOCOL_HEAD)
        return;

    while ((rx_in - offset) >= PROTOCOL_HEAD)
    {
        if (tuya_uart.wifi_data_process_buf[offset + HEAD_FIRST] != FRAME_FIRST)
        {
            offset++;
            continue;
        }

        if (tuya_uart.wifi_data_process_buf[offset + HEAD_SECOND] != FRAME_SECOND)
        {
            offset++;
            continue;
        }

        if (tuya_uart.wifi_data_process_buf[offset + PROTOCOL_VERSION] != MCU_RX_VER)
        {
            offset += 2;
            continue;
        }

        rx_value_len = tuya_uart.wifi_data_process_buf[offset + LENGTH_HIGH] * 0x100;
        rx_value_len += (tuya_uart.wifi_data_process_buf[offset + LENGTH_LOW] + PROTOCOL_HEAD);
        if (rx_value_len > sizeof(tuya_uart.wifi_data_process_buf) + PROTOCOL_HEAD)
        {
            offset += 3;
            continue;
        }

        if ((rx_in - offset) < rx_value_len)
        {
            break;
        }

        //数据接收完成
        if (tuya_tools.get_check_sum((unsigned char *)tuya_uart.wifi_data_process_buf + offset, rx_value_len - 1) != tuya_uart.wifi_data_process_buf[offset + rx_value_len - 1])
        {
            //校验出错
            //printf("crc error (crc:0x%X  but data:0x%X)\r\n",get_check_sum((unsigned char *)wifi_data_process_buf + offset,rx_value_len - 1),wifi_data_process_buf[offset + rx_value_len - 1]);
            offset += 3;
            continue;
        }

        data_handle(offset);
        offset += rx_value_len;
    } //end while

    rx_in -= offset;
    if (rx_in > 0)
    {
        tuya_tools.my_memcpy((char *)tuya_uart.wifi_data_process_buf, (const char *)tuya_uart.wifi_data_process_buf + offset, rx_in);
    }

}

/**
 * @description: Data frame processing
 * @param {unsigned short} offset : Data start position
 * @return {*}
 */
void TuyaWifi::data_handle(unsigned short offset)
{
#ifdef SUPPORT_MCU_FIRM_UPDATE
    unsigned char *firmware_addr = NULL;
    static unsigned short firm_size;           //Upgrade package size
    static unsigned long firm_length;          //MCU upgrade file length
    static unsigned char firm_update_flag = 0; //MCU upgrade flag
    unsigned long dp_len;
    unsigned char firm_flag; //Upgrade package size flag
#else
    unsigned short dp_len;
#endif

    unsigned char ret;
    unsigned short i, total_len;
    unsigned char cmd_type = tuya_uart.wifi_data_process_buf[offset + FRAME_TYPE];
    
#if ((defined VOICE_MODULE_PROTOCOL_ENABLE) || (defined BLE_RELATED_FUNCTION_ENABLE) || (defined MODULE_EXPANDING_SERVICE_ENABLE) || \
    (defined IR_TX_RX_TEST_ENABLE) || (defined GET_IR_STATUS_ENABLE) || (defined MCU_DP_UPLOAD_SYN) || (defined GET_WIFI_STATUS_ENABLE) || \
    (defined WIFI_CONNECT_TEST_ENABLE) || (defined WIFI_STREAM_ENABLE) || (defined WIFI_TEST_ENABLE))
    unsigned char result;
#endif

#ifdef WEATHER_ENABLE
    static unsigned char isWoSend = 0; //Whether the weather data has been opened, 0:no  1:yes
#endif

#ifdef WIFI_TEST_ENABLE
    unsigned char rssi;
#endif

#ifdef FILE_DOWNLOAD_ENABLE
    unsigned char *file_data_addr = NULL;
    static unsigned short file_package_size = 0; //File packet size
    static unsigned char file_download_flag = 0; //File download flag
    unsigned int file_download_size = 0;
#endif

    switch (cmd_type)
    {
    case HEAT_BEAT_CMD: //Heartbeat package
        heat_beat_check();
        break;

    case PRODUCT_INFO_CMD: //Product information
        product_info_update();
        break;

    case WORK_MODE_CMD: //Query the module working mode set by the MCU
        get_mcu_wifi_mode();
        break;

#ifndef WIFI_CONTROL_SELF_MODE
    case WIFI_STATE_CMD: //Wifi working status
        wifi_work_state = tuya_uart.wifi_data_process_buf[offset + DATA_START];
        tuya_uart.wifi_uart_write_frame(WIFI_STATE_CMD, MCU_TX_VER, 0);
#ifdef WEATHER_ENABLE
        if (wifi_work_state == WIFI_CONNECTED && isWoSend == 0) //When the WIFI connection is successful, open the weather data only once
        { 
            mcu_open_weather();
            isWoSend = 1;
        }
#endif
        break;

    case WIFI_RESET_CMD: //Reset wifi
        reset_wifi_flag = RESET_WIFI_SUCCESS;
        break;

    case WIFI_MODE_CMD: //Select smartconfig/AP mode
        set_wifimode_flag = SET_WIFICONFIG_SUCCESS;
        break;
#endif

    case DATA_QUERT_CMD: //Order send
        total_len = (tuya_uart.wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | tuya_uart.wifi_data_process_buf[offset + LENGTH_LOW];

        for (i = 0; i < total_len;)
        {
            dp_len = tuya_uart.wifi_data_process_buf[offset + DATA_START + i + 2] * 0x100;
            dp_len += tuya_uart.wifi_data_process_buf[offset + DATA_START + i + 3];
            //
            ret = data_point_handle((unsigned char *)tuya_uart.wifi_data_process_buf + offset + DATA_START + i);

            if (SUCCESS == ret)
            {
                //Send success
            }
            else
            {
                //Send fault
            }

            i += (dp_len + 4);
        }
        break;

    case STATE_QUERY_CMD: //Status query
        all_data_update();
        break;

#ifdef SUPPORT_MCU_FIRM_UPDATE
    case UPDATE_START_CMD: //Upgrade start
        //Get global variable of upgrade package size
        firm_flag = PACKAGE_SIZE;
        if (firm_flag == 0)
        {
            firm_size = 256;
        }
        else if (firm_flag == 1)
        {
            firm_size = 512;
        }
        else if (firm_flag == 2)
        {
            firm_size = 1024;
        }

        firm_length = wifi_data_process_buf[offset + DATA_START];
        firm_length <<= 8;
        firm_length |= wifi_data_process_buf[offset + DATA_START + 1];
        firm_length <<= 8;
        firm_length |= wifi_data_process_buf[offset + DATA_START + 2];
        firm_length <<= 8;
        firm_length |= wifi_data_process_buf[offset + DATA_START + 3];

        upgrade_package_choose(PACKAGE_SIZE);
        firm_update_flag = UPDATE_START_CMD;
        break;

    case UPDATE_TRANS_CMD: //Upgrade transfer
        if (firm_update_flag == UPDATE_START_CMD)
        {
            //Stop all data reporting
            stop_update_flag = ENABLE;

            total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];

            dp_len = wifi_data_process_buf[offset + DATA_START];
            dp_len <<= 8;
            dp_len |= wifi_data_process_buf[offset + DATA_START + 1];
            dp_len <<= 8;
            dp_len |= wifi_data_process_buf[offset + DATA_START + 2];
            dp_len <<= 8;
            dp_len |= wifi_data_process_buf[offset + DATA_START + 3];

            firmware_addr = (unsigned char *)wifi_data_process_buf;
            firmware_addr += (offset + DATA_START + 4);

            if ((total_len == 4) && (dp_len == firm_length))
            {
                //Last pack
                ret = mcu_firm_update_handle(firmware_addr, dp_len, 0);
                firm_update_flag = 0;
            }
            else if ((total_len - 4) <= firm_size)
            {
                ret = mcu_firm_update_handle(firmware_addr, dp_len, total_len - 4);
            }
            else
            {
                firm_update_flag = 0;
                ret = ERROR;
            }

            if (ret == SUCCESS)
            {
                wifi_uart_write_frame(UPDATE_TRANS_CMD, MCU_TX_VER, 0);
            }
            //Restore all data reported
            stop_update_flag = DISABLE;
        }
        break;
#endif

#ifdef SUPPORT_GREEN_TIME
    case GET_ONLINE_TIME_CMD: //Get system time (Greenwich Mean Time)
        mcu_get_greentime((unsigned char *)(wifi_data_process_buf + offset + DATA_START));
        break;
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
    case GET_LOCAL_TIME_CMD: //Get local time
        mcu_write_rtctime((unsigned char *)(wifi_data_process_buf + offset + DATA_START));
        break;
#endif

#ifdef WIFI_TEST_ENABLE
    case WIFI_TEST_CMD: //Wifi function test
        result = wifi_data_process_buf[offset + DATA_START];
        rssi = wifi_data_process_buf[offset + DATA_START + 1];
        wifi_test_result(result, rssi);
        break;
#endif

#ifdef WEATHER_ENABLE
    case WEATHER_OPEN_CMD: //Turn on the weather
        weather_open_return_handle(wifi_data_process_buf[offset + DATA_START], wifi_data_process_buf[offset + DATA_START + 1]);
        break;

    case WEATHER_DATA_CMD: //Weather data
        total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];
        weather_data_raw_handle((unsigned char *)wifi_data_process_buf + offset + DATA_START, total_len);
        break;
#endif

#ifdef WIFI_STREAM_ENABLE
    case STREAM_TRANS_CMD:                                          //Stream data transmission
        stream_status = wifi_data_process_buf[offset + DATA_START]; //Service transmission back to reception
        stream_trans_send_result(stream_status);
        break;

    case MAPS_STREAM_TRANS_CMD:                                          //streams trans(Support for multiple maps)
        maps_stream_status = wifi_data_process_buf[offset + DATA_START]; //Service transmission back to reception
        maps_stream_trans_send_result(maps_stream_status);
        break;
#endif

#ifdef WIFI_CONNECT_TEST_ENABLE
    case WIFI_CONNECT_TEST_CMD: //Wifi function test(connection designated route)
        result = wifi_data_process_buf[offset + DATA_START];
        wifi_connect_test_result(result);
        break;
#endif

#ifdef GET_MODULE_MAC_ENABLE
    case GET_MAC_CMD: //Get module mac
        mcu_get_mac((unsigned char *)(wifi_data_process_buf + offset + DATA_START));
        break;
#endif

#ifdef GET_WIFI_STATUS_ENABLE
    case GET_WIFI_STATUS_CMD: //Gets the wifi networking status
        result = wifi_data_process_buf[offset + DATA_START];
        get_wifi_status(result);
        break;
#endif

#ifdef MCU_DP_UPLOAD_SYN
    case STATE_UPLOAD_SYN_RECV_CMD: //Status upload results(synchronization)
        result = wifi_data_process_buf[offset + DATA_START];
        get_upload_syn_result(result);
        break;
#endif

#ifdef GET_IR_STATUS_ENABLE
    case GET_IR_STATUS_CMD: //IR status notification
        result = wifi_data_process_buf[offset + DATA_START];
        get_ir_status(result);
        break;
#endif

#ifdef IR_TX_RX_TEST_ENABLE
    case IR_TX_RX_TEST_CMD: //IR into send-receive test
        result = wifi_data_process_buf[offset + DATA_START];
        ir_tx_rx_test_result(result);
        break;
#endif

#ifdef FILE_DOWNLOAD_ENABLE
    case FILE_DOWNLOAD_START_CMD: //File download startup
        //Get file package size selection
        if (FILE_DOWNLOAD_PACKAGE_SIZE == 0)
        {
            file_package_size = 256;
        }
        else if (FILE_DOWNLOAD_PACKAGE_SIZE == 1)
        {
            file_package_size = 512;
        }
        else if (FILE_DOWNLOAD_PACKAGE_SIZE == 2)
        {
            file_package_size = 1024;
        }

        file_download_size = wifi_data_process_buf[offset + DATA_START];
        file_download_size = (file_download_size << 8) | wifi_data_process_buf[offset + DATA_START + 1];
        file_download_size = (file_download_size << 8) | wifi_data_process_buf[offset + DATA_START + 2];
        file_download_size = (file_download_size << 8) | wifi_data_process_buf[offset + DATA_START + 3];

        file_download_package_choose(FILE_DOWNLOAD_PACKAGE_SIZE);
        file_download_flag = FILE_DOWNLOAD_START_CMD;
        break;

    case FILE_DOWNLOAD_TRANS_CMD: //File download data transfer
        if (file_download_flag == FILE_DOWNLOAD_START_CMD)
        {
            total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];

            dp_len = wifi_data_process_buf[offset + DATA_START];
            dp_len <<= 8;
            dp_len |= wifi_data_process_buf[offset + DATA_START + 1];
            dp_len <<= 8;
            dp_len |= wifi_data_process_buf[offset + DATA_START + 2];
            dp_len <<= 8;
            dp_len |= wifi_data_process_buf[offset + DATA_START + 3];

            file_data_addr = (unsigned char *)wifi_data_process_buf;
            file_data_addr += (offset + DATA_START + 4);

            if ((total_len == 4) && (dp_len == file_download_size))
            {
                //Last pack
                ret = file_download_handle(file_data_addr, dp_len, 0);
                file_download_flag = 0;
            }
            else if ((total_len - 4) <= file_package_size)
            {
                ret = file_download_handle(file_data_addr, dp_len, total_len - 4);
            }
            else
            {
                file_download_flag = 0;
                ret = ERROR;
            }

            if (ret == SUCCESS)
            {
                wifi_uart_write_frame(FILE_DOWNLOAD_TRANS_CMD, MCU_TX_VER, 0);
            }
        }
        break;
#endif

#ifdef MODULE_EXPANDING_SERVICE_ENABLE
    case MODULE_EXTEND_FUN_CMD: //Module expansion service
        total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];
        open_module_time_serve_result((unsigned char *)(wifi_data_process_buf + offset + DATA_START), total_len);
        break;
#endif

#ifdef BLE_RELATED_FUNCTION_ENABLE
    case BLE_TEST_CMD: //Bluetooth functional test（Scan designated bluetooth beacon）
        total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];
        BLE_test_result((unsigned char *)(wifi_data_process_buf + offset + DATA_START), total_len);
        break;
#endif

#ifdef VOICE_MODULE_PROTOCOL_ENABLE
    case GET_VOICE_STATE_CMD: //Gets the voice status code
        result = wifi_data_process_buf[offset + DATA_START];
        get_voice_state_result(result);
        break;
    case MIC_SILENCE_CMD: //MIC mute Settings
        result = wifi_data_process_buf[offset + DATA_START];
        set_voice_MIC_silence_result(result);
        break;
    case SET_SPEAKER_VOLUME_CMD: //speaker volume set
        result = wifi_data_process_buf[offset + DATA_START];
        set_speaker_voice_result(result);
        break;
    case VOICE_TEST_CMD: //Audio production test
        result = wifi_data_process_buf[offset + DATA_START];
        voice_test_result(result);
        break;
    case VOICE_AWAKEN_TEST_CMD: //Wake up production test
        result = wifi_data_process_buf[offset + DATA_START];
        voice_awaken_test_result(result);
        break;
    case VOICE_EXTEND_FUN_CMD: //Voice module extension function
        total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];
        voice_module_extend_fun((unsigned char *)(wifi_data_process_buf + offset + DATA_START), total_len);
        break;
#endif

    default:
        break;
    }
}


/**
 * @description: Input product All DP ID, Type, total number of DPs
 * @param {unsigned char} dp_cmd_array : DP array. array[][0] : DP ID, 
 *                                                 array[][1] : DP Type(DP_TYPE_RAW, DP_TYPE_BOOL, DP_TYPE_VALUE, DP_TYPE_STRING, DP_TYPE_ENUM, DP_TYPE_BITMAP)
 * @param {unsigned char} dp_cmd_num : total number of DPs
 * @return {*}
 */
void TuyaWifi::set_dp_cmd_total(unsigned char dp_cmd_array[][2], unsigned char dp_cmd_num)
{
    download_dp_number = dp_cmd_num;
    download_cmd = dp_cmd_array;
}

/**
 * @description: Get the serial number of the DPID in the array.
 * @param {unsigned char} dpid
 * @return {unsigned char} index : The index of the input dpid in the array
 */
unsigned char TuyaWifi::get_dowmload_dpid_index(unsigned char dpid)
{
    unsigned char index;

    for (index = 0; index < download_dp_number; index++)
    {
        if (download_cmd[index][0] == dpid)
        {
            break;
        }
    }
    return index;
}

/**
 * @description: Delivery data processing
 * @param {const unsigned char} value : Send data source pointer
 * @return Return data processing result
 */
unsigned char TuyaWifi::data_point_handle(const unsigned char value[])
{
    unsigned char dp_id, index;
    unsigned char dp_type;
    unsigned char ret;
    unsigned short dp_len;

    dp_id = value[0];
    dp_type = value[1];
    dp_len = value[2] * 0x100;
    dp_len += value[3];

    index = get_dowmload_dpid_index(dp_id);

    if (dp_type != download_cmd[index][1])
    {
        //Error message
        return FALSE;
    }
    else
    {
        ret = dp_download_handle(dp_id, value + 4, dp_len);
    }

    return ret;
}

/**
 * @description: DP command processing callback function
 * @param {tuya_callback_dp_download} _func
 * @return {*}
 */
void TuyaWifi::dp_process_func_register(tuya_callback_dp_download _func)
{
    dp_download_handle = _func;
}

/**
 * @description: Reply to the current device status callback function
 * @param {tuya_callback_dp_update_all} _func
 * @return {*}
 */
void TuyaWifi::dp_update_all_func_register(tuya_callback_dp_update_all _func)
{
    all_data_update = _func;
}

/**
 * @description: Heartbeat packet detection
 * @param {*}
 * @return {*}
 */
void TuyaWifi::heat_beat_check(void)
{
    unsigned char length = 0;
    static unsigned char mcu_reset_state = FALSE;

    if (FALSE == mcu_reset_state)
    {
        length = tuya_uart.set_wifi_uart_byte(length, FALSE);
        mcu_reset_state = TRUE;
    }
    else
    {
        length = tuya_uart.set_wifi_uart_byte(length, TRUE);
    }

    tuya_uart.wifi_uart_write_frame(HEAT_BEAT_CMD, MCU_TX_VER, length);
}

/**
 * @description: Product information upload
 * @param {*}
 * @return {*}
 */
void TuyaWifi::product_info_update(void)
{
    unsigned char length = 0;

#if ((defined CONFIG_MODE_DELAY_TIME) || (defined CONFIG_MODE_CHOOSE) || (defined ENABLE_MODULE_IR_FUN) || (defined LONG_CONN_LOWPOWER))
    unsigned char str[10] = {0};
#endif

    length = tuya_uart.set_wifi_uart_buffer(length, pid_key, (unsigned short)(tuya_tools.my_strlen(pid_key)));
    length = tuya_uart.set_wifi_uart_buffer(length, product_id, PID_LEN);
    length = tuya_uart.set_wifi_uart_buffer(length, mcu_ver_key, (unsigned short)(tuya_tools.my_strlen(mcu_ver_key)));
    length = tuya_uart.set_wifi_uart_buffer(length, mcu_ver_value, VER_LEN);
    length = tuya_uart.set_wifi_uart_buffer(length, mode_key, (unsigned short)(tuya_tools.my_strlen(mode_key)));
    length = tuya_uart.set_wifi_uart_buffer(length, (const unsigned char *)CONFIG_MODE, (unsigned short)(tuya_tools.my_strlen((unsigned char *)CONFIG_MODE)));

#ifdef CONFIG_MODE_DELAY_TIME
    sprintf((char *)str, ",\"mt\":%d", CONFIG_MODE_DELAY_TIME);
    length = tuya_uart.set_wifi_uart_buffer(length, str, tuya_tools.my_strlen(str));
#endif
#ifdef CONFIG_MODE_CHOOSE
    sprintf((char *)str, ",\"n\":%d", CONFIG_MODE_CHOOSE);
    length = tuya_uart.set_wifi_uart_buffer(length, str, tuya_tools.my_strlen(str));
#endif
#ifdef ENABLE_MODULE_IR_FUN
    sprintf((char *)str, ",\"ir\":\"%d.%d\"", MODULE_IR_PIN_TX, MODULE_IR_PIN_RX);
    length = tuya_uart.set_wifi_uart_buffer(length, str, tuya_tools.my_strlen(str));
#endif
#ifdef LONG_CONN_LOWPOWER
    sprintf((char *)str, ",\"low\":%d", LONG_CONN_LOWPOWER);
    length = tuya_uart.set_wifi_uart_buffer(length, str, tuya_tools.my_strlen(str));
#endif

    length = tuya_uart.set_wifi_uart_buffer(length, product_info_end, tuya_tools.my_strlen(product_info_end));

    tuya_uart.wifi_uart_write_frame(PRODUCT_INFO_CMD, MCU_TX_VER, length);
}

/**
 * @description: Query the working mode of mcu and wifi
 * @param {*}
 * @return {*}
 */
void TuyaWifi::get_mcu_wifi_mode(void)
{
    unsigned char length = 0;

#ifdef WIFI_CONTROL_SELF_MODE //Module self-processing
    length = tuya_uart.set_wifi_uart_byte(length, WF_STATE_KEY);
    length = tuya_uart.set_wifi_uart_byte(length, WF_RESERT_KEY);
#else
    //No need to process data
#endif

    tuya_uart.wifi_uart_write_frame(WORK_MODE_CMD, MCU_TX_VER, length);
}

/**
 * @description: mcu gets bool,value,enum type to send dp value. (raw, string type needs to be handled at the user's discretion. fault only report)
 * @param {unsigned char} dpid : data point ID 
 * @param {const unsigned char} value : dp data buffer address 
 * @param {unsigned short} len : data length
 * @return {unsigned char} Parsed data
 */
unsigned long TuyaWifi::mcu_get_dp_download_data(unsigned char dpid, const unsigned char value[], unsigned short len)
{
    unsigned long ret;
    switch (download_cmd[get_dowmload_dpid_index(dpid)][1])
    {
    case DP_TYPE_BOOL:
        ret = tuya_dp.mcu_get_dp_download_bool(value, len);
        break;

    case DP_TYPE_VALUE:
        ret = tuya_dp.mcu_get_dp_download_value(value, len);
        break;

    case DP_TYPE_ENUM:
        ret = tuya_dp.mcu_get_dp_download_enum(value, len);
        break;

    default:
        break;
    }
    return ret;
}

/**
 * @description: dp data upload
 * @param {unsigned char} dpid
 * @param {const unsigned char} value
 * @param {unsigned short} len
 * @return {*}
 */
unsigned char TuyaWifi::mcu_dp_update(unsigned char dpid, const unsigned char value[], unsigned short len)
{
    unsigned char ret;
    switch (download_cmd[get_dowmload_dpid_index(dpid)][1])
    {
        case DP_TYPE_RAW:
            ret = tuya_dp.mcu_dp_raw_update(dpid, value, len);
        break;

        case DP_TYPE_BOOL:
            ret = tuya_dp.mcu_dp_bool_update(dpid, *value);
        break;

        case DP_TYPE_VALUE:
            ret = tuya_dp.mcu_dp_value_update(dpid, *value);
        break;

        case DP_TYPE_STRING:
            ret = tuya_dp.mcu_dp_string_update(dpid, value, len);
        break;

        case DP_TYPE_ENUM:
            ret = tuya_dp.mcu_dp_enum_update(dpid, *value);
        break;

        case DP_TYPE_BITMAP:
            ret = tuya_dp.mcu_dp_fault_update(dpid, *value);
        break;


        default:
            break;
    }
    return ret;
}

unsigned char TuyaWifi::mcu_dp_update(unsigned char dpid, unsigned char value, unsigned short len)
{
    unsigned char ret;
    switch (download_cmd[get_dowmload_dpid_index(dpid)][1])
    {
        case DP_TYPE_BOOL:
            ret = tuya_dp.mcu_dp_bool_update(dpid, value);
        break;

        case DP_TYPE_ENUM:
            ret = tuya_dp.mcu_dp_enum_update(dpid, value);
        break;

        case DP_TYPE_VALUE:
            ret = tuya_dp.mcu_dp_value_update(dpid, value);
        break;

        case DP_TYPE_BITMAP:
            ret = tuya_dp.mcu_dp_fault_update(dpid, value);
        break;

        default:
            break;
    }
    return ret;
}

unsigned char TuyaWifi::mcu_dp_update(unsigned char dpid, char value, unsigned short len)
{
    unsigned char ret;
    switch (download_cmd[get_dowmload_dpid_index(dpid)][1])
    {
        case DP_TYPE_BOOL:
            ret = tuya_dp.mcu_dp_bool_update(dpid, value);
        break;

        case DP_TYPE_ENUM:
            ret = tuya_dp.mcu_dp_enum_update(dpid, value);
        break;

        case DP_TYPE_VALUE:
            ret = tuya_dp.mcu_dp_value_update(dpid, value);
        break;

        case DP_TYPE_BITMAP:
            ret = tuya_dp.mcu_dp_fault_update(dpid, value);
        break;

        default:
            break;
    }
    return ret;
}

unsigned char TuyaWifi::mcu_dp_update(unsigned char dpid, unsigned long value, unsigned short len)
{
    unsigned char ret;
    switch (download_cmd[get_dowmload_dpid_index(dpid)][1])
    {
        case DP_TYPE_BOOL:
            ret = tuya_dp.mcu_dp_bool_update(dpid, value);
        break;

        case DP_TYPE_ENUM:
            ret = tuya_dp.mcu_dp_enum_update(dpid, value);
        break;

        case DP_TYPE_VALUE:
            ret = tuya_dp.mcu_dp_value_update(dpid, value);
        break;

        case DP_TYPE_BITMAP:
            ret = tuya_dp.mcu_dp_fault_update(dpid, value);
        break;

        default:
            break;
    }
    return ret;
}

unsigned char TuyaWifi::mcu_dp_update(unsigned char dpid, long value, unsigned short len)
{
    unsigned char ret;
    switch (download_cmd[get_dowmload_dpid_index(dpid)][1])
    {
        case DP_TYPE_BOOL:
            ret = tuya_dp.mcu_dp_bool_update(dpid, value);
        break;

        case DP_TYPE_ENUM:
            ret = tuya_dp.mcu_dp_enum_update(dpid, value);
        break;

        case DP_TYPE_VALUE:
            ret = tuya_dp.mcu_dp_value_update(dpid, value);
        break;

        case DP_TYPE_BITMAP:
            ret = tuya_dp.mcu_dp_fault_update(dpid, value);
        break;

        default:
            break;
    }
    return ret;
}

unsigned char TuyaWifi::mcu_dp_update(unsigned char dpid, unsigned int value, unsigned short len)
{
    unsigned char ret;
    switch (download_cmd[get_dowmload_dpid_index(dpid)][1])
    {
        case DP_TYPE_BOOL:
            ret = tuya_dp.mcu_dp_bool_update(dpid, value);
        break;

        case DP_TYPE_ENUM:
            ret = tuya_dp.mcu_dp_enum_update(dpid, value);
        break;

        case DP_TYPE_VALUE:
            ret = tuya_dp.mcu_dp_value_update(dpid, value);
        break;

        case DP_TYPE_BITMAP:
            ret = tuya_dp.mcu_dp_fault_update(dpid, value);
        break;

        default:
            break;
    }
    return ret;
}

unsigned char TuyaWifi::mcu_dp_update(unsigned char dpid, int value, unsigned short len)
{
    unsigned char ret;
    switch (download_cmd[get_dowmload_dpid_index(dpid)][1])
    {
        case DP_TYPE_BOOL:
            ret = tuya_dp.mcu_dp_bool_update(dpid, value);
        break;

        case DP_TYPE_ENUM:
            ret = tuya_dp.mcu_dp_enum_update(dpid, value);
        break;

        case DP_TYPE_VALUE:
            ret = tuya_dp.mcu_dp_value_update(dpid, value);
        break;

        case DP_TYPE_BITMAP:
            ret = tuya_dp.mcu_dp_fault_update(dpid, value);
        break;

        default:
            break;
    }
    return ret;
}



/**
 * @description: MCU set wifi working mode
 * @param {unsigned char} mode : enter mode 
 *                               0(SMART_CONFIG):enter smartconfig mode
 *                               1(AP_CONFIG):enter AP mode
 * @return {*}
 */
void TuyaWifi::mcu_set_wifi_mode(unsigned char mode)
{
    unsigned char length = 0;

    set_wifimode_flag = SET_WIFICONFIG_ERROR;

    length = tuya_uart.set_wifi_uart_byte(length, mode);

    tuya_uart.wifi_uart_write_frame(WIFI_MODE_CMD, MCU_TX_VER, length);
}

/**
 * @description: The MCU actively obtains the current wifi working status.
 * @param {*}
 * @return {unsigned char} wifi work state
 *                          SMART_CONFIG_STATE:smartconfig config status
 *                          AP_STATE:AP config status
 *                          WIFI_NOT_CONNECTED:WIFI configuration succeeded but not connected to the router
 *                          WIFI_CONNECTED:WIFI configuration is successful and connected to the router
 *                          WIFI_CONN_CLOUD:WIFI is connected to the cloud server
 *                          WIFI_LOW_POWER:WIFI is in low power mode
 *                          SMART_AND_AP_STATE: WIFI smartconfig&AP mode
 * @note   1:If the module is in self-processing mode, the MCU does not need to call this function.
 */
unsigned char TuyaWifi::mcu_get_wifi_work_state(void)
{
    return wifi_work_state;
}


/**
 * @description: MCU actively resets wifi working mode
 * @param {*}
 * @return {*}
 * @note   1:The MCU actively calls to obtain whether the reset wifi is successful through the mcu_get_reset_wifi_flag() function.
 *         2:If the module is in self-processing mode, the MCU does not need to call this function.
 */
void TuyaWifi::mcu_reset_wifi(void)
{
    reset_wifi_flag = RESET_WIFI_ERROR;
    
    tuya_uart.wifi_uart_write_frame(WIFI_RESET_CMD, MCU_TX_VER, 0);
}
