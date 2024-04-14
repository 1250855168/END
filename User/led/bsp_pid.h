#ifndef BSP_PID_H
#define BSP_PID_H

#include "stm32f10x.h"

extern 

// ADC读取温度值
float readTemperature(void);

// 设置PWM输出
void setPWM(uint16_t dutyCycle);

// PID控制器计算
float pidController(float target, float current);

#endif
