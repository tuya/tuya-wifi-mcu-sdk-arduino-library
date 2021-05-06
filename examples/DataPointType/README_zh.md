# DataPointType

[English](./README.md) | [中文](./README_zh.md) 

该库仅支持烧录了通用固件的涂鸦 WiFi+BLE 、WiFi模组，在Arduino UNO板子上验证通过。

**注意：Arduino 中的 默认Serial 串口已被Tuya mcu sdk 接管，请不要对默认Serial（引脚 0 ，1）做任何操作。** 

## 一、demo 介绍 

涂鸦模组和Arduino板连接时会干扰到对Arduino板的烧录，烧录时应断开Arduino板与涂鸦通信板或涂鸦通信板的连接。例程烧录成功后将Arduino板上串口(Serial) 与 涂鸦模组上的 `RXD` 和 `TXD` 连接起来，然后将UNO板子上的 `Pin7` 脚拉低1s左右（模拟按键按下），设备将会进入配网模式，在进入配网状态和配网过程中Arduino板上LED将会闪烁，打开涂鸦智能APP对设备（Arduino_DataPointType）进行配网操作，连接成功后Arduino板上LED将会停止闪烁，配网成功后即可通过APP对设备进行控制。

本demo对涂鸦 IoT 平台的6种数据类型进项了介绍和演示。

## 二、功能介绍 

涂鸦IoT平台的数据类型主要分为6种，分别是`Bool` , `value` , `enum` , `string` , `raw` , `fault(BITMAP)` 。除 `fault(BITMAP)` 仅能上传外，其他每个类型都可选择`可上报可下发` ，`仅上报` ， `仅下发` 三种数据传输方式。 

### `Bool`和`ENUM`

`Bool` 和 `enum` 这两种类型和C语言中的使用方式一致，比较简单。但是值得一提的是`enum` ，`enum` 型中的枚举值是根据你在[涂鸦IoT平台](https://iot.tuya.com/?_source=97c44038fafc20e9c8dd5fdb508cc9c2)中创建的顺序进行排序的，从0开始，从左到右+1。如下图：

![DP_type_enum](https://images.tuyacn.com/smart/shiliu_zone/Tuya_Arduino_library/DataPointType/DP_type_enum.png)

### value 

`value` 型是我们比较常用的类型之一，存储大小为4字节，即数据范围为`0 ~ 4,294,967,295` 或 `-2,147,483,648 ~ 2,147,483,647`。

`value` 类型中需要注意的是间距与倍数：

间距：假设数据范围是0-100，间距为1时：数据值为0，1，2，3…；间距为2：数据值应0，2，4，6…；间距为3时：数据值为0，3，6，9…

倍数：默认为0，数值为整数，当需要小数时改变倍数值数据将以10的指数形式进行传输。如：当你输入10000时，倍数配置为0，那么显示数值为10000；倍数配置为1，那么显示数值为1000.0；倍数配置为2，那么显示数值为100.00；倍数配置为3，那么显示数值为10.000，以此类推。

![DP_type_value](https://images.tuyacn.com/smart/shiliu_zone/Tuya_Arduino_library/DataPointType/DP_type_value.png)

### string 

`string` 类型也就是字符串类型，最大长度不超过255个字节。

### fault 
`fault(BITMAP)`  也就是故障型，该类型只上报，按照对应的字节位来标识的，依次为bit0-bitx，多个字节位代表多个故障可同时发生，最多支持30位。
### raw 
`raw` 类型是当上述几种类型都没有办法满足你的需求时，使用该类型。`raw` 型的数据传输也就是透明传输。



## 三、技术支持

您可以通过以下方法获得涂鸦的支持:

- 开发者中心：https://developer.tuya.com
- 帮助中心: https://support.tuya.com/cn/help
- 技术支持工单中心: https://service.console.tuya.com 