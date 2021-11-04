# Module control self
[English](./README.md) | [中文](./README_zh.md) 

This library only supports the Tuya WiFi+BLE, WiFi module with the generic firmware burned in and verified on the Arduino UNO board.

**Note: The default Serial serial port in Arduino has been taken over by the Tuya mcu sdk, please do not do anything with the default Serial (pins 0 , 1).** 



## I. Introduction to the demo 

The connection between the Tuya module and the Arduino board will interfere with the burning of the Arduino board. The Arduino board should be disconnected from the Tuya communication board or the Tuya communication board when burning.

This example uses Arduino board + Tuya wifi communication board and is modified from the start demo. This demo is mainly used to show how to use the module self-processing function.
</br>

### 1. How to open the module self-processing function?
In `.. /.. /config.h`, change `#define WIFI_CONTROL_SELF_MODE 0` to

```c
#define WIFI_CONTROL_SELF_MODE 1
```
It turns on the module self-processing function.
**This function is turned off by default. After turning it on, other demos will have compilation errors except this demo. Because some functions can not be used under module self-processing, such as getting the current network connection status, mcu sending re-entering network mode and other functions can not be used.**
</br>

### 2. What does the module self-processing function do?
The main function of the module self-processing function is to leave the function of the wiring indicator and wiring button to the module, so the pin pin on the module is used. Developers do not need to implement the function of the wiring button and wiring indicator on the MCU. Press and hold the wiring button for 5s to start the module into the wiring state.
</br>

### 3. How to use the module self-processing function?
First, you need to enable the module self-processing function. Then you need to call the function in `setup()` to configure the pins of the module as the led pin and button pin.
If you do not call the function, the default parameters will be used. The default `pin 16` is the pin for the wiring indicator and `pin 28` is the pin for the wiring button.

```c
void set_state_pin(unsigned char led_pin, unsigned char key_pin);
```
</br>

## III. Technical Support

You can get support for Tuya by using the following methods:

- Developer Centre: https://developer.tuya.com
- Help Centre: https://support.tuya.com/help
- Technical Support Work Order Centre: https://service.console.tuya.com 