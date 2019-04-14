/****************************************************************************
VFD Tube Clock
无限远点的弧光灯
2019/4/13
Zhong_zhibin@outlook.com
****************************************************************************/
#include "stm32f10x.h"
#include "stdio.h"
#include "SysTick.h"
#include "USART.h"
#include "DS1302.h"
#include "RGBLED.h"
#include "MCP9700.h"
#include "timer.h"
#include "74HC595.h"
#include "KEY.h"
#include "Interaction.h"

/*PowerCtrl  PC  I/O:PB9*/
void PowerCtrl_Init(void);
void Seed_Data(int value);

extern unsigned char DSsecond, DSminute, DShour, DSweek, DSday, DSmonth, DSyear;

uint16_t flag[4] = {0};
extern uint16_t SYS_Value[4];
//=============================================================================
//文件名称：main
//功能概要：主函数
//参数说明：无
//函数返回：int
//=============================================================================
int main(void)
{   
    PowerCtrl_Init();/*PowerCtrl   I/O:PB9*/
    RGBLED_Init();
    USART_Config();
    DS1302_init();
    HC595_Pin_Init();
    Key_GPIO_Init();
    MCP9700_Init();
    Read_Flash(&flag[0]);
    Read_Flash(&SYS_Value[0]);
    TIM3_Int_Init();
    TIM2_Int_Init();

    
    while(1)
    {    
        if(1 == Key_Scan(GPIOA,GPIO_Pin_6))
        {
            flag[0]++;
            flag[0] %= 4;
            Seed_Data(flag[0]);
            Usart_SendString(USART3, "KEY1\r\n"); 
        }
        
        if(1 == Key_Scan(GPIOA,GPIO_Pin_7) && flag[0] != 0)
        {
            flag[1]++;  
            flag[1] %= 5;
            Seed_Data(flag[1]);
            Usart_SendString(USART3, "KEY2\r\n");
            
        }
        
        if(1 == Key_Scan(GPIOB,GPIO_Pin_1) && flag[0] != 0)
        {
            flag[2]++;
            flag[2] %= 8;
            Seed_Data(flag[2]);
            Usart_SendString(USART3, "KEY3\r\n");
            
        }
        
        if(1 == Key_Scan(GPIOB,GPIO_Pin_0) && flag[0] != 0)
        {
            flag[3]++;
            flag[3] %= 2;
            Seed_Data(flag[3]);
            Usart_SendString(USART3, "KEY4\r\n"); 
        }
        KEY_Process(&flag[0]);
    }
    
}


/*PowerCtrl
I/O PB9  
使能时 输出30V 1.5V两路电压即Boost电路与Buck电路均启动
不使能时 30V输出为12V电源电压 1.5V为0V
主要目的 减少待机功耗
*/
void PowerCtrl_Init(void)
{
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;
    /*开启GPIO外设时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    /*选择要控制的GPIO引脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    /*设置引脚模式为通用推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    /*设置引脚速率为50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*调用库函数，初始化GPIO*/
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}



void Seed_Data(int value)
{
    char temp_value[3] = {0};
    sprintf(temp_value, "%d", value);
    Usart_SendString(USART3, temp_value);
}



