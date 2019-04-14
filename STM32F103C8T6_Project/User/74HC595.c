#include "74HC595.h"

#define    HC595_DS_SET         GPIO_SetBits(GPIOA,GPIO_Pin_10)
#define    HC595_DS_RESET       GPIO_ResetBits(GPIOA,GPIO_Pin_10)
#define    HC595_SHCP_SET       GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define    HC595_SHCP_RESET     GPIO_ResetBits(GPIOA,GPIO_Pin_11)
#define    HC595_STCP_SET       GPIO_SetBits(GPIOA,GPIO_Pin_9)
#define    HC595_STCP_RESET     GPIO_ResetBits(GPIOA,GPIO_Pin_9)
#define    HC595_OE_SET         GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define    HC595_OE_RESET       GPIO_ResetBits(GPIOA,GPIO_Pin_8)
/*
static void Delay_us(uint16_t us)
{
    uint16_t a;
	while(us)
	{
	 	a=16;
	 	while(a--);
	 	us--;
	}
	return;
}
*/

void HC595_Pin_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//关闭JTAG
    
    //DS Pin
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //SHCP Pin
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //STCP Pin
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //OE Pin
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void HC595SendData(unsigned char OutData)
{
    unsigned char i; //发送数据时做循环使用临时变量
    for(i = 0; i < 8; i++) //将8位数据按位发送,先发送高字节后发送低字节
    {
        HC595_SHCP_RESET;//时钟线低电平
        if( (OutData & 0x80) == 0x80)//判断数据高低位
        {
            HC595_DS_SET;                   //数据线高电平
        }
        else
        {
            HC595_DS_RESET;          //数据线低电平
        }
        OutData = OutData << 1;         //数据左移1位
        HC595_SHCP_SET;                 //时钟线高电平
    }

}

void HC595_shift_to_storage(void)
{
    //上升沿输出数据
    HC595_STCP_RESET;
    HC595_STCP_SET;
    HC595_STCP_RESET;
}

void HC595_OE(unsigned char value)
{
    if(0 == value)
    {
        HC595_OE_RESET;
    }
    else
    {
        HC595_OE_SET;
    }
}

