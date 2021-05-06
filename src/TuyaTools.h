/*
 * @FileName: TuyaTools.h
 * @Author: Tuya
 * @Email: 
 * @LastEditors: Tuya
 * @Date: 2021-04-25 17:44:25
 * @LastEditTime: 2021-04-28 19:49:36
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Description: 
 */
#ifndef __TUYA_TOOLS_H__
#define __TUYA_TOOLS_H__

class TuyaTools
{
public:
    TuyaTools(void);
    ~TuyaTools(void);

    unsigned char hex_to_bcd(unsigned char Value_H, unsigned char Value_L);
    unsigned long my_strlen(unsigned char *str);
    void *my_memset(void *src, unsigned char ch, unsigned short count);
    void *my_memcpy(void *dest, const void *src, unsigned short count);
    char *my_strcpy(char *dest, const char *src);
    int my_strcmp(char *s1, char *s2);
    void int_to_byte(unsigned long number, unsigned char value[4]);
    unsigned long byte_to_int(const unsigned char value[4]);
    unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len);

private:
};

#endif /* __TUYA_TOOLS_H__ */
