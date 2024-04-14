#ifndef PWM_TIMER_H
#define PWM_TIMER_H
#include "stm32f10x.h"

extern TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
extern 	TIM_OCInitTypeDef       TIM_OCInitStructure;;

void bsp_pwm_init(void);

#endif
