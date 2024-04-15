
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

#include "bsp_led.h"
#include "bsp_usart.h"
#include "delay.h"
#include "bsp_oled.h"
#include "bsp_w25q64.h"
#include "bsp_timer.h"
#include "pwm_timer.h"
#include "bsp_Adc.h"
#include "bsp_pid.h"

uint8_t Write_data[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

uint8_t Read_data[20];


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	USART_Config();

	LED_GPIO_Config();

	I2C_Configuration();

	 OLED_Init();

	 OLED_OPEN();

	 OLED_Fill(0);

	SPI1_Init();

	My_ADC_Init();

	TIM2_Init();

	bsp_pwm_init();

	GREEN_LED(ON);

	while (1)
	{

	}
}

/*********************************************END OF FILE**********************/


                 
 
