#include "KEY.h"


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


void Key_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启按键端口的时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    //选择按键的引脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    // 设置按键的引脚为浮空输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    //使用结构体初始化按键
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //选择按键的引脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    //设置按键的引脚为浮空输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    //使用结构体初始化按键
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /*检测是否有按键按下 */
    if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 0 )
    {
        Delay_ms(20);
        /*等待按键释放 */
        while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 0);
        return 	1;
    }
    else
        return 0;
}




