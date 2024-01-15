#include"bsp_pid.h"

// 定义PID控制器参数
float kp = 0.5;  // 比例系数
float ki = 0.2;  // 积分系数
float kd = 0.1;  // 微分系数

extern u16 adc_temp;

// 定义目标温度和初始温度
float targetTemperature = 45.0;  // 目标温度
float currentTemperature = 0.0;  // 当前温度
float PWM_Min = 50;
float PWM_Max = 950;

// 定义PWM输出参数
//uint16_t pwmPeriod = 1000;  // PWM周期
uint16_t pwmDutyCycle = 0;  // PWM占空比

// ADC读取温度值
float readTemperature()
{
    // 在此处编写读取ADC传感器并转换为温度值的代码
    // 返回读取到的温度值
    currentTemperature =((1.43 - adc_temp)/0.0043+25.0);

    return currentTemperature;

}

// 设置PWM输出
void setPWM(uint16_t dutyCycle)
{
    // 在此处编写设置PWM输出的代码
   currentTemperature =  readTemperature();

    pwmDutyCycle = pwmDutyCycle + (pidController(targetTemperature,currentTemperature))/1000;

   if (pwmDutyCycle<150)
   {
        pwmDutyCycle = PWM_Min;

        TIM_OCInitStructure.TIM_Pulse = pwmDutyCycle;
   }
   
    if (pwmDutyCycle>1000)
   {
        pwmDutyCycle = PWM_Max;

        TIM_OCInitStructure.TIM_Pulse = pwmDutyCycle;
   }
}

// PID控制器计算
float pidController(float target, float current)
{
    static float integral = 0.0;
    static float previousError = 0.0;
    float error = target - current;
    float output;

    integral += error;
    float derivative = error - previousError;
    output = kp * error + ki * integral + kd * derivative;

    previousError = error;

    return output;
}
