# start 
[English](./README.md) | [中文](./README_zh.md) 

This library only supports the Tuya WiFi+BLE, WiFi module with the generic firmware burned in and verified on the Arduino UNO board.

**Note: The default Serial serial port in Arduino has been taken over by the Tuya mcu sdk, please do not do anything with the default Serial (pins 0 , 1).** 



## I. Introduction to the demo 

The connection between the Tuya module and the Arduino board will interfere with the burning of the Arduino board. The Arduino board should be disconnected from the Tuya communication board or the Tuya communication board when burning. After the routine is successfully burned, connect the serial port (Serial) on the Arduino board to the `RXD` and `TXD` on the Tuya module, then pull down the `Pin7` pin on the UNO board for about 1s (simulating a key press), the device will enter the connection network mode, the LED on the Arduino board will flash during the connection network state and connection network process After successful connection, the LED on the Arduino board will stop flashing and the device can be controlled by the APP.

This example uses Arduino board + Tuya wifi communication board, which can be controlled by the Arduino board after connection network through the Tuya Smart App.



## II. Function Implementation 

### 1. Create product 

Go to [Tuya IoT Platform](https://iot.tuya.com/?_source=97c44038fafc20e9c8dd5fdb508cc9c2) to create a product.

![creat_produce1](https://images.tuyacn.com/smart/shiliu_zone/Tuya_Arduino_library/creat_produce1.png) 

Select category, programme.

![creat_produce2](https://images.tuyacn.com/smart/shiliu_zone/Tuya_Arduino_library/creat_produce2.png)

Complete the product information and click on create product: !

![creat_produce3](https://images.tuyacn.com/smart/shiliu_zone/Tuya_Arduino_library/creat_produce3.png)

Select the relevant functions according to your needs and click on Confirm: !

![create_produce4](https://images.tuyacn.com/smart/shiliu_zone/Tuya_Arduino_library/creat_produce4.png) 

After selecting the device panel, go to Hardware Development, select the Tuya Standard Module MCU SDK development method and select the corresponding module: 

![create_produce6](https://images.tuyacn.com/smart/shiliu_zone/Tuya_Arduino_library/creat_produce6.png)

Select the firmware.

![create_produce7](https://images.tuyacn.com/smart/shiliu_zone/Tuya_Arduino_library/creat_produce7.png) 

### 2. Important function descriptions

Initialise the serial port, the generic firmware of the Tuya WIFi module supports 115200, 9600 baud rate adaptive, so the serial port should be initialised to 115200 or 9600 when initialising the serial port. in `setup()`.

```c++
Serial.begin(9600);
```

mcu needs to send the created PID and the mcu's software version number to the Tuya Cloud module. In `setup()`.

```c++
  //Enter the PID and MCU software version
  my_device.init("ma67l9sgmdyg3d2k", "1.0.0");
```

We need to pass the ID and type of the DP selected when creating the product into the MCU SDK to use.

```c++
TuyaWifi my_device;

#define DPID_SWITCH 20 //DP ID

unsigned char dp_array[][2] =
{
  //This product was created with only one DP point selected, so there is only one piece of information in this array
  {DPID_SWITCH, DP_TYPE_BOOL}, // the first one is filled with the DP point ID, the second one is the data type of the DP point
};

void setup()
{
    ...
    my_device.set_dp_cmd_total(dp_array, 1); //dp_array: is an array of DP point IDs and data types, 1: is the number of DPs defined in the array
    ...
}

```

The initialization also requires registration of the DP point down processing function and the upload all DP points function.

```c++
unsigned char led_state = 0;

void setup() 
{
...
  //register DP download processing callback function
  my_device.dp_process_func_register(led_dp_process); //register DP download processing callback function
  //register upload all DP callback function
  my_device.dp_update_all_func_register(dp_update_all); //register device status function, upload all DP points
...
}

void loop()
{
    ...
    my_device.uart_service();
    ...
}

/**
 * @description: DP download callback function.
 * @param {unsigned char} dpid
 * @param {const unsigned char} value
 * @param {unsigned short} length
 * @return {unsigned char}
 */
unsigned char led_dp_process(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  switch(dpid) {
    case DPID_SWITCH:
      led_state = my_device.mcu_get_dp_download_data(dpid, value, length);//get the data of this DP in the downstream data frame
      if (led_state) {
        digitalWrite(LED_BUILTIN, HIGH);
      } else {
        digitalWrite(LED_BUILTIN, LOW);
      }
      my_device.mcu_dp_update(dpid, value, length);
    break;

    default:break;
  }
  return 1;
}

/**
 * @description: Upload all DP status of the current device.
 * @param {*}
 * @return {*}
 */
void dp_update_all(void)
{
  my_device.mcu_dp_update(DPID_SWITCH, &led_state, 1);
}
```



## III. Technical Support

You can get support for Tuya by using the following methods:

- Developer Centre: https://developer.tuya.com
- Help Centre: https://support.tuya.com/help
- Technical Support Work Order Centre: https://service.console.tuya.com 