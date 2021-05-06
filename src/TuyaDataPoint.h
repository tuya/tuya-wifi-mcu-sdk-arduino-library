/*
 * @FileName: TuyaDataPoint.h 
 * @Author: Tuya
 * @Email: 
 * @LastEditors: Tuya
 * @Date: 2021-04-10 15:34:17
 * @LastEditTime: 2021-04-28 19:48:45
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Description: 
 */
#ifndef __TUYA_DATA_POINT_H__
#define __TUYA_DATA_POINT_H__

class TuyaDataPoint
{
public:
    // TuyaDataPoint(void);
    // ~TuyaDataPoint(void);

    unsigned char mcu_get_dp_download_bool(const unsigned char value[], unsigned short len);
    unsigned char mcu_get_dp_download_enum(const unsigned char value[], unsigned short len);
    unsigned long mcu_get_dp_download_value(const unsigned char value[], unsigned short len);

    unsigned char mcu_dp_raw_update(unsigned char dpid, const unsigned char value[], unsigned short len);
    unsigned char mcu_dp_bool_update(unsigned char dpid, unsigned char value);
    unsigned char mcu_dp_value_update(unsigned char dpid, unsigned long value);
    unsigned char mcu_dp_string_update(unsigned char dpid, const unsigned char value[], unsigned short len);
    unsigned char mcu_dp_enum_update(unsigned char dpid, unsigned char value);
    unsigned char mcu_dp_fault_update(unsigned char dpid, unsigned long value);

private:
};

#endif /* __TUYA_DATA_POINT_H__ */
