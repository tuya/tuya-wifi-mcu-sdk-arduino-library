# Module control self
[English](./README.md) | [中文](./README_zh.md) 

该库仅支持烧录了通用固件的涂鸦 WiFi+BLE 、WiFi模组，在Arduino UNO板子上验证通过。
</br>
**注意：Arduino 中的 默认Serial 串口已被Tuya mcu sdk 接管，请不要对默认Serial（引脚 0 ，1）做任何操作。** 
</br>
## 一、demo 介绍 

涂鸦模组和Arduino板连接时会干扰到对Arduino板的烧录，烧录时应断开Arduino板与涂鸦通信板或涂鸦通信板的连接。

本例程硬件使用了Arduino板+涂鸦wifi通信板，在 start demo 的基础上进行了修改。本 demo 主要用于演示模组自处理功能如何使用。
### 1.如何开启模组自处理功能
在 `../../config.h` 中将 `#define WIFI_CONTROL_SELF_MODE  0` 更改为：
```c
#define WIFI_CONTROL_SELF_MODE  1
```
就开启了模组自处理功能。
**该功能默认关闭，打开后除本 demo 外，其他 demo 会出现编译错误的情况。因为在模组自处理下部分功能无法使用，如获取当前网络连接状态，mcu 发送重新进入配网模式等功能无法使用。**
</br>

### 2.模组自处理功能有什么作用
模组自处理功能，主要作用是将配网指示灯和配网按键的功能交给了模组去处理，所以使用的也就是模组上的 pin 脚。开发者不用在 MCU 上实现配网按键和配网指示灯的功能了。长按配网按键 5s 开始进入模组进入配网状态。
</br>

### 3.如何使用模组自处理功能
首先要开启模组自处理功能。然后需要在 `setup()` 内调用下面的函数配置对应模组的引脚作为配网指示灯和配网按键的引脚。
不调用函数修改，将会使用默认参数进行设置。默认 `pin 16` 为配网指示灯的引脚， `pin 28` 为配网按键的引脚。
```c
void set_state_pin(unsigned char led_pin, unsigned char key_pin);
```
</br>

## 二、技术支持

您可以通过以下方法获得涂鸦的支持:

- 开发者中心：https://developer.tuya.com
- 帮助中心: https://support.tuya.com/help
- 技术支持工单中心: https://service.console.tuya.com 