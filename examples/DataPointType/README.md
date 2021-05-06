# DataPointType

[English](./README.md) | [中文](./README_zh.md) 

  This library only supports Tuya WiFi+BLE, WiFi modules with generic firmware burned in and verified on Arduino UNO boards.

**Note: The default Serial serial port in Arduino has been taken over by the Tuya mcu sdk, please do not do anything with the default Serial (pins 0 , 1).**

  ## I. Introduction to the demo 

The connection between the Tuya module and the Arduino board will interfere with the burning of the Arduino board. The Arduino board should be disconnected from the Tuya communication board or the Tuya communication board when burning. After the routine is successfully burned, connect the serial port (Serial) on the Arduino board to the `RXD` and `TXD` on the Tuya module, then pull down the `Pin7` pin on the UNO board for about 1s (simulating a key press), the device will enter the connection network mode, the LED on the Arduino board will flash during the connection network state and connection network process After successful connection, the LED on the Arduino board will stop flashing and the device can be controlled by the APP.

This demo introduces and demonstrates the 6 data types of the Tuya IoT platform.

  ## II. 

The data types of the Tuya IoT platform are divided into 6 types, namely `Bool` , `value` , `enum` , `string` , `raw` , `fault(BITMAP)` . Each type has the option of `uploadable and downlinkable`, `upload only` and ` downlink only`, except for `fault(BITMAP)` which can only be uploaded. 

  ### `Bool` and `ENUM`

The `Bool` and `enum` types are used in the same way as in C and are relatively simple to use. But it is worth mentioning `enum` , the enum values in the `enum` type are sorted according to the order you create them in the [Tuya IoT platform](https://iot.tuya.com/?_source=97c44038fafc20e9c8dd5fdb508cc9c2), starting from 0 and going from left to right + 1. The following diagram.

![DP_type_enum](https://images.tuyacn.com/smart/shiliu_zone/Tuya_Arduino_library/DataPointType/DP_type_enum.png)

  ### value 

  The `value` type is one of our more common types, storing 4 bytes, i.e. data in the range `0 ~ 4,294,967,295` or `-2,147,483,648 ~ 2,147,483,647`.

  The `value` type requires attention to spacing and multiplicity.

  Spacing: assuming the data range is 0-100, with a spacing of 1: the data values should be 0, 1, 2, 3...; with a spacing of 2: the data values should be 0, 2, 4, 6...; with a spacing of 3: the data values should be 0, 3, 6, 9...

Multiplier: default is 0, the value is an integer, when a decimal is required changing the multiplier value the data will be transmitted as an exponent of 10. For example: when you enter 10000, the multiplier is configured to 0, then the value displayed is 10000; the multiplier is configured to 1, then the value displayed is 1000.0; the multiplier is configured to 2, then the value displayed is 100.00; the multiplier is configured to 3, then the value displayed is 10.000, and so on.

![DP_type_value](https://images.tuyacn.com/smart/shiliu_zone/Tuya_Arduino_library/DataPointType/DP_type_value.png)

  ### string 

The `string` type is also known as a string type, with a maximum length of 255 bytes.

  ### fault 

`fault(BITMAP)` also known as fault type, this type is only reported, and is identified by the corresponding byte bits, in order of bit0-bitx, multiple byte bits means multiple faults can occur at the same time, up to 30 bits are supported.

  ### raw 

The `raw` type is used when none of the above types will meet your needs. The `raw` type of data transfer is also known as transparent transfer.

  

  ## III. Technical support

You can get support for Tuya by using the following methods:

- Developer Centre: https://developer.tuya.com
- Help Centre: https://support.tuya.com/help
- Technical Support Work Order Centre: https://service.console.tuya.com 