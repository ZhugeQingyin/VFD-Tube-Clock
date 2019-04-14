#ifndef __Interaction_H
#define __Interaction_H

#include "stm32f10x.h"
#include <stdint.h>

#define STM32_FLASH_SIZE 64 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0不使能;1使能)


#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
#define Flash_Save_Addr  0x0800F000     //Flash 储存地址


void BCD2HEX(void);

void VFD_Test(void);
void VFD_Display(int N1, int N2, int N3, int N4);
void VFD_Light(void);
void VFD_Slake(void);

void KEY_Process(uint16_t * key_value);

void Time_Mode1(void);
void Time_Mode2(void);
void Time_Mode3(void);


void Read_Flash(uint16_t * temp);

uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);		  //读出半字  
void STMFLASH_WriteLenByte(uint32_t WriteAddr, uint32_t DataToWrite, uint16_t Len);	//指定地址开始写入指定长度的数据
uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr, uint16_t Len);						//指定地址开始读取指定长度数据
void STMFLASH_Write(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead);   		//从指定地址开始读出指定长度的数据
//测试写入
void Test_Write(u32 WriteAddr,u16 WriteData);								   


#endif
