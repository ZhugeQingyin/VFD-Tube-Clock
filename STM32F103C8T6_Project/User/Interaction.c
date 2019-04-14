#include "Interaction.h"
#include "74HC595.h"
#include "DS1302.h"
#include "RGBLED.h"
#include "SysTick.h"

#define EN_SET          GPIO_SetBits(GPIOB, GPIO_Pin_9)        //PowerCtrl On
#define EN_RESET        GPIO_ResetBits(GPIOB, GPIO_Pin_9)      //PowerCtrl Off


uint16_t SYS_Value[4] = {0};


const uint8_t Num_Table[11] = {0x12,0xFA,0x23,0x62,0xCA,0x46,0x06,0xF2,0x02,0x42,0xff};

extern unsigned char DSsecond, DSminute, DShour, DSweek, DSday, DSmonth, DSyear;

int Intsecond, Intminute, Inthour, Intweek, Intday, Intmonth, Intyear;

static void Delay_ms(uint16_t ms)
{
    uint16_t a;
    while(ms)
    {
        a = 16000;
        while(a--);
        ms--;
    }
    return;
}

void BCD2HEX(void)
{
    Intsecond = bcd_to_hex(DSsecond);
    Intminute = bcd_to_hex(DSminute);
    Inthour = bcd_to_hex(DShour);
    Intweek = bcd_to_hex(DSweek);
    Intday = bcd_to_hex(DSday);
    Intmonth = bcd_to_hex(DSmonth);
    Intyear = bcd_to_hex(DSyear);
}


void VFD_Test(void)
{
    int Num = 0;
    for(Num = 0;Num<10;Num++)
    {
        HC595SendData(Num_Table[Num]);
        HC595_shift_to_storage();
        Delay_ms(600); 
        Num %= 9;
    }

}

void VFD_Display(int N1, int N2, int N3, int N4)
{
    HC595_OE(0);
    HC595SendData(Num_Table[N4]);
    HC595SendData(Num_Table[N3]);
    HC595SendData(Num_Table[N2]);
    HC595SendData(Num_Table[N1]);
    HC595_shift_to_storage();
}

void VFD_Light(void)
{
    TIM_Cmd(TIM2, ENABLE);
    EN_SET;
}

void VFD_Slake(void)
{
    TIM_Cmd(TIM2, DISABLE);
    RGBLED(0, 0, 0);
    VFD_Display(10, 10, 10, 10);
    EN_RESET;
}


/*
按键功能部分
KEY1 ：整体开(时分) - 年 - 月 日 - 整体关
KEY2 ：氛围灯 100%亮度 - 75%亮度 - 50%亮度 - 25%亮度 - 0%亮度 亮度值记忆
KEY3 ：氛围灯 模式1 - 模式2 ........模式1 模式循环选择 模式记忆
KEY4 ：整点报时 开关
*/

void KEY_Process(uint16_t * key_value)
{
    static uint16_t key_value_last[4] = {0};
    static uint8_t count = 1;
    static uint8_t count2 = 1;
    int i = 0;
    uint8_t change_flag = 0;
    
    /*整点报时功能*/
    if(1 == SYS_Value[3])
    {
        if(0 == Intminute && 0 == Intsecond)
        {
            key_value[0] = 1;  
        }   
    }
    
    
    
    if(key_value[0] != 0)
    {
        if(1 == count2)
        {
            VFD_Light();
            count2 = 0;
        }

        count = 1;
        
        if(key_value_last[0] != key_value[0])
        {
            switch(key_value[0])
            {
                case 1:
                    Time_Mode1();
                    break;
                case 2: 
                    Time_Mode2();
                    break;
                case 3: 
                    Time_Mode3();
                    break;
                default: 
                    break;
            }
        }
        
        if(key_value_last[1] != key_value[1])
        {
            switch(key_value[1])
            {
                case 0: 
                    SYS_Value[1] = 100;
                    VFD_Display(0, 2, 9, 9);
                    break;
                case 1: 
                    SYS_Value[1] = 75;
                    VFD_Display(0, 2, 7, 5);
                    break;
                case 2: 
                    SYS_Value[1] = 50;
                    VFD_Display(0, 2, 5, 0);
                    break;
                case 3: 
                    SYS_Value[1] = 25;
                    VFD_Display(0, 2, 2, 5);
                    break;
                case 4: 
                    SYS_Value[1] = 0;
                    VFD_Display(0, 2, 0, 0);
                    break;
                default:break;
            }
        }
        
        if(key_value_last[2] != key_value[2])
        {
            switch(key_value[2])
            {
                case 0: 
                    SYS_Value[2] = 0;
                    VFD_Display(0, 3, 0, 0);
                    break;
                case 1:
                    SYS_Value[2] = 1;
                    VFD_Display(0, 3, 0, 1);
                    break;
                case 2: 
                    SYS_Value[2] = 2;
                    VFD_Display(0, 3, 0, 2);
                    break;
                case 3: 
                    SYS_Value[2] = 3;
                    VFD_Display(0, 3, 0, 3);
                    break;
                case 4: 
                    SYS_Value[2] = 4;
                    VFD_Display(0, 3, 0, 4);
                    break;
                case 5: 
                    SYS_Value[2] = 5;
                    VFD_Display(0, 3, 0, 5);
                    break;
                case 6: 
                    SYS_Value[2] = 6;
                    VFD_Display(0, 3, 0, 6);
                    break;
                case 7: 
                    SYS_Value[2] = 7;
                    VFD_Display(0, 3, 0, 7);
                    break;
                default:break;
            }
        }
        
        if(key_value_last[3] != key_value[3])
        {
            switch(key_value[3])
            {
                case 0:
                    SYS_Value[3] = 0;
                    VFD_Display(0, 4, 0, 0);
                    break;
                case 1: 
                    SYS_Value[3] = 1;
                    VFD_Display(0, 4, 0, 1);
                    break;
                default:break;
            }
        }
        
        
        
        //数据是否发生变化 
        for(i = 1; i < 4; i++) 
        {
            if(key_value_last[i] != key_value[i])
            {
                change_flag = 1;
            }
        }
        //若数据发生变化，则保存至Flash
        if(1 == change_flag)
        {
            STMFLASH_Write(Flash_Save_Addr,&key_value[1],3);
        }
        
    }
    else
    {
        if(1 == count)
        {
            VFD_Slake();
            count = 0;
            count2 = 1;
        }
    }
    
    
    
    for(i = 0; i < 4; i++)
    {
        key_value_last[i] = key_value[i];
    }
    
}

/*
时间换算
Time_Mode1 时 分
Time_Mode2 月 日
Time_Mode3 年

*/

void Time_Mode1(void)
{
    int temp = 0;
    uint8_t Number[4] = {0};
    temp = 100 * Inthour + Intminute;
    Number[0] = temp / 1000;
    temp %= 1000;
    Number[1] = temp / 100;
    temp %= 100;
    Number[2] = temp / 10;
    temp %= 10;
    Number[3] = temp;
    VFD_Display(Number[0], Number[1], Number[2], Number[3]);
}

void Time_Mode2(void)
{
    int temp = 0;
    uint8_t Number[4] = {0};
    temp = Intmonth * 100 + Intday;
    Number[0] = temp / 1000;
    temp %= 1000;
    Number[1] = temp / 100;
    temp %= 100;
    Number[2] = temp / 10;
    temp %= 10;
    Number[3] = temp;
    VFD_Display(Number[0], Number[1], Number[2], Number[3]);
}

void Time_Mode3(void)
{
    int temp = 0;
    uint8_t Number[4] = {0};
    temp = Intyear + 2000;
    Number[0] = temp / 1000;
    temp %= 1000;
    Number[1] = temp / 100;
    temp %= 100;
    Number[2] = temp / 10;
    temp %= 10;
    Number[3] = temp;
    VFD_Display(Number[0], Number[1], Number[2], Number[3]);
}





/*
读取数据
*/
void Read_Flash(uint16_t * temp)
{
    uint16_t Flash_value[3] = {0};
    
    STMFLASH_Read(Flash_Save_Addr, &Flash_value[0], 3);
    
    if(Flash_value[0] != 0xFFFF)
    {
        temp[1] = Flash_value[0];
        temp[2] = Flash_value[1];
        temp[3] = Flash_value[2];
    }
    else
    {
        Flash_value[0] = Flash_value[0] = Flash_value[0] = 0;
        STMFLASH_Write(Flash_Save_Addr,&Flash_value[0],3);
    }  
}




/*
Flash驱动部分

*/

//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr)
{
	return *(volatile uint16_t*)faddr; 
}
#if STM32_FLASH_WREN	//如果使能了写   
//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数   
void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)   
{ 			 		 
	uint16_t i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//地址增加2.
	}  
} 
//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else 
#define STM_SECTOR_SIZE	2048
#endif		 

void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)	
{
	uint32_t secpos;	   //扇区地址
	uint16_t secoff;	   //扇区内偏移地址(16位字计算)
	uint16_t secremain; //扇区内剩余地址(16位字计算)	   
 	uint16_t i;    
	uint32_t offaddr;   //去掉0X08000000后的地址
    uint16_t STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节
    
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//非法地址
	FLASH_Unlock();						//解锁
	offaddr=WriteAddr-STM32_FLASH_BASE;		//实际偏移地址.
	secpos=offaddr/STM_SECTOR_SIZE;			//扇区地址  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//扇区剩余空间大小   
	if(NumToWrite<=secremain)secremain=NumToWrite;//不大于该扇区范围
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区
			for(i=0;i<secremain;i++)//复制
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//写入整个扇区  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;				//扇区地址增1
			secoff=0;				//偏移位置为0 	 
		   	pBuffer+=secremain;  	//指针偏移
			WriteAddr+=secremain;	//写地址偏移	   
		   	NumToWrite-=secremain;	//字节(16位)数递减
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//下一个扇区还是写不完
			else secremain=NumToWrite;//下一个扇区可以写完了
		}	 
	};	
	FLASH_Lock();//上锁
}
#endif

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   	
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//WriteAddr:起始地址
//WriteData:要写入的数据
void Test_Write(uint32_t WriteAddr,uint16_t WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//写入一个字 
}




