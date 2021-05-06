/*
 * @FileName: TuyaDefs.h 
 * @Author: Tuya
 * @Email: 
 * @LastEditors: Tuya
 * @Date: 2021-04-10 11:28:40
 * @LastEditTime: 2021-04-28 19:48:57
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Description: Some necessary constant definitions. 
 */
#ifndef __TUYA_DEFS_H__
#define __TUYA_DEFS_H__

/* Define constant */
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL   ((void *)0)
#endif 
#endif

#ifndef SUCCESS
#define SUCCESS 1
#endif

#ifndef ERROR
#define ERROR 0
#endif

#ifndef INVALID
#define INVALID 0xFF
#endif

#ifndef ENABLE
#define ENABLE 1
#endif

#ifndef DISABLE
#define DISABLE 0
#endif

#define MCU_RX_VER      0x00    //Module send frame protocol version number
#define MCU_TX_VER      0x03    //MCU send frame protocol version number(default)
#define PROTOCOL_HEAD   0x07    //Fixed protocol header length
#define FRAME_FIRST     0x55    //Frame header first byte
#define FRAME_SECOND    0xaa    //Frame header second byte

//=============================================================================
//Byte order of the frame
//=============================================================================
#define HEAD_FIRST          0
#define HEAD_SECOND         1
#define PROTOCOL_VERSION    2
#define FRAME_TYPE          3
#define LENGTH_HIGH         4
#define LENGTH_LOW          5
#define DATA_START          6

//=============================================================================
//Data frame type
//=============================================================================
#define         HEAT_BEAT_CMD                   0                               //Heartbeat package
#define         PRODUCT_INFO_CMD                1                               //Product information
#define         WORK_MODE_CMD                   2                               //Query the module working mode set by the MCU
#define         WIFI_STATE_CMD                  3                               //Wifi working status
#define         WIFI_RESET_CMD                  4                               //Reset wifi
#define         WIFI_MODE_CMD                   5                               //Select smartconfig/AP mode
#define         DATA_QUERT_CMD                  6                               //Order send
#define         STATE_UPLOAD_CMD                7                               //Status upload	 
#define         STATE_QUERY_CMD                 8                               //Status query   
#define         UPDATE_START_CMD                0x0a                            //Upgrade start
#define         UPDATE_TRANS_CMD                0x0b                            //Upgrade transfer
#define         GET_ONLINE_TIME_CMD             0x0c                            //Get system time (Greenwich Mean Time)
#define         FACTORY_MODE_CMD                0x0d                            //Enter production test mode    
#define         WIFI_TEST_CMD                   0x0e                            //Wifi function test
#define         GET_LOCAL_TIME_CMD              0x1c                            //Get local time
#define         WEATHER_OPEN_CMD                0x20                            //Turn on the weather          
#define         WEATHER_DATA_CMD                0x21                            //Weather data
#define         STATE_UPLOAD_SYN_CMD            0x22                            //Status upload (synchronization)
#define         STATE_UPLOAD_SYN_RECV_CMD       0x23                            //Status upload results(synchronization)
#define         HEAT_BEAT_STOP                  0x25                            //Turn off the WIFI module heartbeat
#define         STREAM_TRANS_CMD                0x28                            //Stream data transmission
#define         GET_WIFI_STATUS_CMD             0x2b                            //Gets the wifi networking status
#define         WIFI_CONNECT_TEST_CMD           0x2c                            //Wifi function test(connection designated route)
#define         GET_MAC_CMD                     0x2d                            //Get module mac
#define         GET_IR_STATUS_CMD               0x2e                            //IR status notification
#define         IR_TX_RX_TEST_CMD               0x2f                            //IR into send-receive test
#define         MAPS_STREAM_TRANS_CMD           0x30                            //streams trans(Support for multiple maps)
#define         FILE_DOWNLOAD_START_CMD         0x31                            //File download startup
#define         FILE_DOWNLOAD_TRANS_CMD         0x32                            //File download data transfer
#define         MODULE_EXTEND_FUN_CMD           0x34                            //Open the module time service notification
#define         BLE_TEST_CMD                    0x35                            //Bluetooth functional test（Scan designated bluetooth beacon）
#define         GET_VOICE_STATE_CMD             0x60                            //Gets the voice status code
#define         MIC_SILENCE_CMD                 0x61                            //MIC mute Settings
#define         SET_SPEAKER_VOLUME_CMD          0x62                            //speaker volume set
#define         VOICE_TEST_CMD                  0x63                            //Audio production test
#define         VOICE_AWAKEN_TEST_CMD           0x64                            //Wake up production test
#define         VOICE_EXTEND_FUN_CMD            0x65                            //Voice module extension function

//=============================================================================
//WIFI work status
//=============================================================================
#define         SMART_CONFIG_STATE              0x00
#define         AP_STATE                        0x01
#define         WIFI_NOT_CONNECTED              0x02
#define         WIFI_CONNECTED                  0x03
#define         WIFI_CONN_CLOUD                 0x04
#define         WIFI_LOW_POWER                  0x05
#define         SMART_AND_AP_STATE              0x06
#define         WIFI_SATE_UNKNOW                0xff
//=============================================================================
//wifi reset status
//=============================================================================
#define         RESET_WIFI_ERROR                0
#define         RESET_WIFI_SUCCESS              1

//=============================================================================
//wifi reset result
//=============================================================================
#define         SET_WIFICONFIG_ERROR            0
#define         SET_WIFICONFIG_SUCCESS          1

//=============================================================================
//dp data point type
//=============================================================================
#define         DP_TYPE_RAW                     0x00        //RAW type
#define         DP_TYPE_BOOL                    0x01        //bool type
#define         DP_TYPE_VALUE                   0x02        //value type
#define         DP_TYPE_STRING                  0x03        //string type
#define         DP_TYPE_ENUM                    0x04        //enum type
#define         DP_TYPE_BITMAP                  0x05        //fault type

//=============================================================================
//wifi distribution network
//=============================================================================
#define         SMART_CONFIG                    0x0  
#define         AP_CONFIG                       0x1   

//=============================================================================
//Choose network access mode
//=============================================================================
#define         CONFIG_MODE_DEFAULT             "0"                             //Default mode
#define         CONFIG_MODE_LOWPOWER            "1"                             //low power mode
#define         CONFIG_MODE_SPECIAL             "2"                             //special mode

#endif /* __TUYA_DEFS_H__ */