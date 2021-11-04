# get green time
[English](./README.md) | [中文](./README_zh.md) 

该库仅支持烧录了通用固件的涂鸦 WiFi+BLE 、WiFi模组，在Arduino UNO板子上验证通过。
</br>
**注意：Arduino 中的 默认Serial 串口已被Tuya mcu sdk 接管，请不要对默认Serial（引脚 0 ，1）做任何操作。** 
</br>

## 一、demo 介绍 

涂鸦模组和Arduino板连接时会干扰到对Arduino板的烧录，烧录时应断开Arduino板与涂鸦通信板或涂鸦通信板的连接。例程烧录成功后将Arduino板上串口(Serial) 与 涂鸦模组上的 `RXD` 和 `TXD` 连接起来，然后将UNO板子上的 `Pin7` 脚拉低1s左右（模拟按键按下），设备将会进入配网模式，在进入配网状态和配网过程中Arduino板上LED将会闪烁，打开涂鸦智能APP对设备（Arduino_led）进行配网操作，连接成功后Arduino板上LED将会停止闪烁，配网成功后即可通过APP对设备进行控制。

本例程硬件使用了Arduino板+涂鸦wifi通信板，在 start demo 的基础上进行了修改。本 demo 主要用于演示如何获取网络时间，将 Arduino 的 pin 8 作为 RX, pin 9 作为 TX，打印时间信息。

获取时间的功能需要在模组成功联网后才可使用。

### 1. 获取格林时间

**函数原型**
```c
char TuyaWifi::get_green_time(TUYA_WIFI_TIME *time, const unsigned int timeout);
```
**Parameters:**
 + <font color="#dd0000">`time`: </font> 存储从模组上获取的时间的数据。
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
 + <font color="#dd0000">`timeout`: </font> 超时时间，单位 ms。如果网络状况不好可将超时时间适当延长。

**Return:**
+ <font color="#dd0000">`1`: </font> 获取成功。
+ <font color="#dd0000">`-1`: </font> 入参为 `NULL`，入参错误。
+ <font color="#dd0000">`-2`: </font> 获取时间超时。

</br>

### 2.获取模组RTC时间
函数原型：
```c
char TuyaWifi::get_rtc_time(TUYA_WIFI_TIME *time, const unsigned int timeout);
```
**Parameters:**
 + <font color="#dd0000">`time`: </font> 存储从模组上获取的时间的数据。
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
 + <font color="#dd0000">`timeout`: </font> 超时时间，单位 ms。如果网络状况不好可将超时时间适当延长。

**Return:**
+ <font color="#dd0000">`1`: </font> 获取成功。
+ <font color="#dd0000">`-1`: </font> 入参为 `NULL`，入参错误。
+ <font color="#dd0000">`-2`: </font> 获取时间超时。

</br>

## 二、技术支持

您可以通过以下方法获得涂鸦的支持:

- 开发者中心：https://developer.tuya.com
- 帮助中心: https://support.tuya.com/help
- 技术支持工单中心: https://service.console.tuya.com 
