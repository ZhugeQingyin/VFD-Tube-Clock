#ifndef __DS1302_H
#define __DS1302_H

#include "stm32f10x.h"
#include <stdint.h>

//写年月日时间寄存器
#define  WriteSecond  0x80
#define  WriteMinute  0x82
#define  WriteHour    0x84
#define  WriteDay     0x86
#define  WriteMonth   0x88
#define  writeWeek    0x8a
#define  writeYear    0x8c
//读年月日时间寄存器
#define  ReadSecond  0x81
#define  ReadMinute  0x83
#define  ReadHour    0x85
#define  ReadDay     0x87
#define  ReadMonth   0x89
#define  ReadWeek    0x8b
#define  ReadYear    0x8d

void DS1302_init(void);
void DS_IO_IN(void);
void DS_IO_OUT(void);
void write_one_byte(unsigned char data);
unsigned char read_one_byte(void);
void ds1302_write_data(unsigned char reg, unsigned char data);
unsigned char ds1302_read_data(unsigned char reg);
unsigned char hex_to_bcd(unsigned char hex_data);
unsigned char bcd_to_hex(unsigned char bcd_data);
void time_read(unsigned char *DSyear, unsigned char *DSweek, unsigned char *DSmonth, unsigned char *DSday, unsigned char *DShour, unsigned char *DSminute, unsigned char *DSsecond);
void time_set(unsigned char DSyear, unsigned char DSweek, unsigned char DSmonth, unsigned char DSday, unsigned char DShour, unsigned char DSminute, unsigned char DSsecond);
#endif

