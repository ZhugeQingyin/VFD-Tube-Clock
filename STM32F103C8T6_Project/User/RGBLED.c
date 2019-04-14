#include "RGBLED.h"

void RGBLED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    // 输出比较通道1 GPIO 初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 输出比较通道2 GPIO 初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 输出比较通道3 GPIO 初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /*--------------------时基结构体初始化-------------------------*/
    // 配置周期，这里配置为10K


    // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period = 99;
    // 驱动CNT计数器的时钟 = Fck_int/(psc+1)
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    // 时钟分频因子 ，配置死区时间时需要用到
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    // 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    // 重复计数器的值，没用到不用管
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    // 初始化定时器
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);


    // 配置为PWM模式1
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    // 输出使能
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    // 输出通道电平极性配置
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

    // 输出比较通道 1
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

    // 输出比较通道 2
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

    // 输出比较通道 3
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_Cmd(TIM4, ENABLE);

}

void RGBLED(unsigned int R, unsigned int G, unsigned int B)
{
    TIM_SetCompare1(TIM4, B);
    TIM_SetCompare2(TIM4, R);
    TIM_SetCompare3(TIM4, G);
}

void RGB_breathing_light(void)
{
    /*
    变换顺序
    R -> RG -> G -> GB -> B -> RB -> RGB ->None
    0    1     2    3     4    5     6     7
    */
    static unsigned char mode = 0;
    static unsigned char value_R = 0;
    static unsigned char value_G = 0;
    static unsigned char value_B = 0;
    switch(mode)
    {
        case 0:
            RGBLED(value_R, value_G, value_B);
            value_R++;
            if(value_R >= 100)
            {
                mode = 1;
            }
            break;
        case 1:
            RGBLED(value_R, value_G, value_B);
            value_G++;
            if(value_G >= 100)
            {
                mode = 2;
            }
            break;
        case 2:
            RGBLED(value_R, value_G, value_B);
            value_R--;
            if(value_R <= 0)
            {
                mode = 3;
            }
            break;
        case 3:
            RGBLED(value_R, value_G, value_B);
            value_B++;
            if(value_B >= 100)
            {
                mode = 4;
            }
            break;
        case 4:
            RGBLED(value_R, value_G, value_B);
            value_G--;
            if(value_G <= 0)
            {
                mode = 5;
            }
            break;
        case 5:
            RGBLED(value_R, value_G, value_B);
            value_R++;
            if(value_R >= 100)
            {
                mode = 6;
            }
            break;
        case 6:
            RGBLED(value_R, value_G, value_B);
            value_G++;
            if(value_G >= 100)
            {
                mode = 7;
            }
            break;
        case 7:
            RGBLED(value_R, value_G, value_B);
            value_R--;
            value_G--;
            value_B--;
            if(value_R <= 0 && value_G <= 0 && value_B <= 0)
            {
                mode = 0;
            }            
            break;
        default:
            value_R = 0;
            value_G = 0;
            value_B = 0;
            mode = 0;
            break;
    }
    
    
}
/*
RGB单色灯
7 个模式 亮度可调
颜色方案 自行修改
*/

void RGB_Light(uint16_t mode, uint16_t value)
{
    uint16_t value_R = 0;
    uint16_t value_G = 0;
    uint16_t value_B = 0;
    
    switch(mode)
    {
        case 1: 
            value_R = 100 * value / 100;
            value_G = 0 * value / 100;
            value_B = 0 * value / 100;
            RGBLED(value_R, value_G, value_B);
            break;
        case 2: 
            value_R = 100 * value / 100;
            value_G = 100 * value / 100;
            value_B = 0 * value / 100;
            RGBLED(value_R, value_G, value_B);
            break;
        case 3: 
            value_R = 0 * value / 100;
            value_G = 100 * value / 100;
            value_B = 0 * value / 100;
            RGBLED(value_R, value_G, value_B);
            break;
        case 4: 
            value_R = 0 * value / 100;
            value_G = 100 * value / 100;
            value_B = 100 * value / 100;
            RGBLED(value_R, value_G, value_B);
            break;
        case 5: 
            value_R = 0 * value / 100;
            value_G = 0 * value / 100; 
            value_B = 100 * value / 100;
            RGBLED(value_R, value_G, value_B);
            break;
        case 6: 
            value_R = 100 * value / 100;
            value_G = 0 * value / 100;
            value_B = 100 * value / 100;
            RGBLED(value_R, value_G, value_B);
            break;
        case 7: 
            value_R = 100 * value / 100;
            value_G = 100 * value / 100;
            value_B = 100 * value / 100;
            RGBLED(value_R, value_G, value_B);
            break;
    }
}



