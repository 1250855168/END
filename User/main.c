	
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

#include "bsp_led.h"
#include "bsp_usart.h"
#include	"delay.h"

int main(void)
{	

	LED_GPIO_Config();	

	USART_Config();

	while (1)
	{	
			LED1(ON);
			delay_ms(1000);
			LED1(OFF);
		
		printf("xxx\r\n");
	}
}

/*********************************************END OF FILE**********************/
