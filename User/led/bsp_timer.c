#include"bsp_timer.h"
#include<stdio.h>

void TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    // 使能定时器时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 定时器配置
    TIM_InitStruct.TIM_Prescaler = 7200 - 1;  // 预分频器，将时钟分频为10 kHz
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数模式
    TIM_InitStruct.TIM_Period = 10000 - 1;  // 计数周期为10秒，即10 kHz * 10000 = 1秒
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟分频
    TIM_InitStruct.TIM_RepetitionCounter = 0;  // 重复计数器

    TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
    
    // 使能定时器溢出中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
    // 配置中断优先级
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStruct);
    
    // 使能定时器
    TIM_Cmd(TIM2, ENABLE);
}

// 定时器中断处理函数
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        // 在这里执行定时器溢出时的任务或回调函数
	
        // 清除中断标志
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

