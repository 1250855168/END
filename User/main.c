
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

#include "bsp_led.h"
#include "bsp_usart.h"
#include "delay.h"
#include "bsp_oled.h"
#include "w25q64.h"

int main(void)
{

	LED_GPIO_Config();

	USART_Config();

	//	printf("%s(%d)-<%s>\r\n", __FILE__, __LINE__, __FUNCTION__);

	I2C_Configuration();

	OLED_Init();

	OLED_OPEN();

	OLED_Fill(0);

	unsigned char ch[] = "1234567";

	OLED_ShowStr(1, 1, ch, 1);



	while (1)
	{
	}
}

/*********************************************END OF FILE**********************/
