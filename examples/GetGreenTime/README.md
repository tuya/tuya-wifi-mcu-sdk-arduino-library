# get green time
[English](./README.md) | [中文](./README_zh.md) 

This library only supports the Tuya WiFi+BLE, WiFi module with the generic firmware burned in and verified on the Arduino UNO board.

**Note: The default Serial serial port in Arduino has been taken over by the Tuya mcu sdk, please do not do anything with the default Serial (pins 0 , 1).** 



## I. Introduction to the demo 

The connection between the Tuya module and the Arduino board will interfere with the burning of the Arduino board. The Arduino board should be disconnected from the Tuya communication board or the Tuya communication board when burning.

This example uses Arduino board + Tuya wifi communication board. This demo is mainly used to show how to get the network time, using Arduino pin 8 of as RX and pin 9 as TX to print the time information.

The function of getting time can only be used after the module is successfully networked.


### 1. Get Green time

**Function**
```c
char TuyaWifi::get_green_time(TUYA_WIFI_TIME *time, const unsigned int timeout);
```
**Parameters:**
 + <font color="#dd0000">`time`: </font> Store the data of the time from the module.
    ```c
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
    ```
 + <font color="#dd0000">`timeout`: </font> Timeout time, unit ms. The timeout time can be extended if the network condition is bad.

**Return:**
+ <font color="#dd0000">`1`: </font> Get success.
+ <font color="#dd0000">`-1`: </font> The input parameter is `NULL`, the input parameter is wrong.
+ <font color="#dd0000">`-2`: </font> Timeout.

</br>

### 1. Get RTC time

**Function**
```c
char TuyaWifi::get_rtc_time(TUYA_WIFI_TIME *time, const unsigned int timeout);
```
**Parameters:**
 + <font color="#dd0000">`time`: </font> Store the data of the time from the module.
    ```c
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
    ```
 + <font color="#dd0000">`timeout`: </font> Timeout time, unit ms. The timeout time can be extended if the network condition is bad.

**Return:**
+ <font color="#dd0000">`1`: </font> Get success.
+ <font color="#dd0000">`-1`: </font> The input parameter is `NULL`, the input parameter is wrong.
+ <font color="#dd0000">`-2`: </font> Timeout.

</br>


## III. Technical Support

You can get support for Tuya by using the following methods:

- Developer Centre: https://developer.tuya.com
- Help Centre: https://support.tuya.com/help
- Technical Support Work Order Centre: https://service.console.tuya.com 