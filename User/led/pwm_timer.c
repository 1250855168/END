#include"pwm_timer.h"

/**
 * 
首先，需要包含STM32头文件 stm32f10x.h。
在 TIM1_PWM_Init 函数中，我们需要初始化相关的GPIO引脚和定时器TIM1。
使用 RCC_APB2PeriphClockCmd 函数使能GPIOA、GPIOB和TIM1的时钟。
对于每个PWM通道，使用 GPIO_InitStructure 结构体初始化相关的GPIO引脚。
设置引脚模式为复用推挽输出，并设置输出速度为50MHz。
使用 TIM_TimeBaseStructInit 函数将 TIM_TimeBaseStructure 结构体的所有成员设置为默认值，
并根据需要修改其中的参数。这里将定时器的时钟分频系数设置为0，计数模式设置为向上计数，
周期设置为1000-1（即频率为定时器时钟频率除以1000），时钟分割设置为不分割，重复计数器设置为0。
使用 TIM_OCStructInit 函数将 TIM_OCInitStructure 结构体的所有成员设置为默认值，
并根据需要修改其中的参数。将通道的工作模式设置为PWM模式1，输出使能和互补输出使能都设置为使能，
占空比设置为500-1（即50%的占空比），输出极性和互补输出极性都设置为高电平有效，
输出空闲状态和互补输出空闲状态都设置为重置。
使用 TIM_OC1Init、TIM_OC2Init 和 TIM_OC3Init 函数分别初始化TIM1的CH1、CH2和CH3通道的PWM输出。
使用 TIM_OC1PreloadConfig、TIM_OC2PreloadConfig 和 TIM_OC3PreloadConfig
 函数使能TIM1的CH1、CH2和CH3通道的预装载功能。
使用 TIM_BDTRStructInit 函数将 TIM_BDTRInitStructure 结构体的所有成员设置为默认值，
并根据需要修改其中的参数。设置刹车使能、刹车极性、死区时间等。
使用 TIM_BDTRConfig 函数配置TIM1的刹车和死区相关参数。
最后，使用 TIM_Cmd 函数使能TIM1定时器和PWM输出，使用 TIM_CtrlPWMOutputs 函数使能TIM1的PWM通道输出
*/

void bsp_pwm_init(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef       TIM_OCInitStructure;
	TIM_BDTRInitTypeDef     TIM_BDTRInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	/* TIM1 PWM CH1 CH2 CH3 --> PA8 PA9 PA10 */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* TIM1 PWM CHN1 CHN2 CHN3 --> PB13 PB14 PB15 */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* TIM1 PWM Break --> PB12 */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* 基本初始化 */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period        = 1000 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler     = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	/* 比较输出初始化 */
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	TIM_OCInitStructure.TIM_Pulse = 500 - 1; /* 占空比50%(与基本初始化中的TIM_Period共同决定) */
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_OCInitStructure.TIM_Pulse = 500 - 1;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_OCInitStructure.TIM_Pulse = 500 - 1;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	/* 刹车和死区初始化 */
	TIM_BDTRStructInit(&TIM_BDTRInitStructure);
	TIM_BDTRInitStructure.TIM_OSSRState       = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState       = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel       = TIM_LOCKLevel_1;
	TIM_BDTRInitStructure.TIM_DeadTime        = 0;
	TIM_BDTRInitStructure.TIM_Break           = TIM_Break_Enable;
	TIM_BDTRInitStructure.TIM_BreakPolarity   = TIM_BreakPolarity_High;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
	TIM_BDTRInitStructure.TIM_DeadTime = 0x8f;
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
	
	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
