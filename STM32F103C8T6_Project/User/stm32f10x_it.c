/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "DS1302.h"
#include "USART.h"
#include "RGBLED.h"
#include "Interaction.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
unsigned char DSsecond, DSminute, DShour, DSweek, DSday, DSmonth, DSyear;
extern uint16_t flag[4];
uint16_t flag_last[4] = {0};
extern uint16_t SYS_Value[4];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}



void USART3_IRQHandler(void)
{
    uint8_t  temp = 0;
    uint8_t i = 0;
    unsigned char time[7];
    static unsigned char RX_Buffer[50];
    /**
    对时 数据包格式
    fa 19 06 03 30 20 38 00 ed
    包头 年 星期 月 日 时 分 秒 包尾
    */
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        temp = USART_ReceiveData(USART3);
        for(i = 0; i < 49; i++)
        {
            RX_Buffer[i] = RX_Buffer[i + 1];
        }
        RX_Buffer[49] = temp;

        if(0xed == RX_Buffer[49] && 0xfa == RX_Buffer[41])
        {
            time[6] = RX_Buffer[48];
            time[5] = RX_Buffer[47];
            time[4] = RX_Buffer[46];
            time[3] = RX_Buffer[45];
            time[2] = RX_Buffer[44];
            time[1] = RX_Buffer[43];
            time[0] = RX_Buffer[42];
            time_set(time[0], time[1], time[2], time[3], time[4], time[5], time[6]);
            Usart_SendString(USART3, "OK");//OK -> 4f 4b (HEX)
            RX_Buffer[41] = 0;
        }
    }
}

/*
定时器2中断服务程序
TIM2中断 50ms
RGB 呼吸灯效控制
*/
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx更新中断标志
        if(0 == SYS_Value[2])
        {
            RGB_breathing_light();
        }
        else
        {
            RGB_Light(SYS_Value[2],SYS_Value[1]);
        }            
    }
}


//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断 500ms
{
    static uint8_t count = 0;
    static uint8_t change_flag = 0;
    int i = 0;
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志
        /*更新时钟*/
        time_read(&DSyear, &DSweek, &DSmonth, &DSday, &DShour, &DSminute, &DSsecond);
        /*--------*/
        Usart_SendByte(USART3,DShour);
        Usart_SendByte(USART3,DSminute);
        Usart_SendByte(USART3,DSsecond);
        BCD2HEX();
        
        
        
        
        for(i = 0; i < 4; i++)
        {
            if(flag_last[i] != flag[i])
            {
                change_flag = 1;
                count = 0;
            }
        }
        if(1 == change_flag)
        {
            count++;
        }
        if(count >= 60)
        {
            flag[0] = 0;
            count = 0;
        }
        for(i = 0; i < 4; i++)
        {
            flag_last[i] = flag[i];
        }
    }
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
