
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

#include "bsp_led.h"
#include "bsp_usart.h"
#include "delay.h"
#include "bsp_oled.h"
#include "bsp_w25q64.h"
uint16_t W25Q128_ID=0;
int main(void)
{

	LED_GPIO_Config();

	USART_Config();

	//	printf("%s(%d)-<%s>\r\n", __FILE__, __LINE__, __FUNCTION__);

	I2C_Configuration();

	OLED_Init();

	OLED_OPEN();

	OLED_Fill(0);

	SPI1_Init();
       
 uint16_t  W25Q128_ID=W25Q128_ReadID();

	unsigned char ch[] = "1234567";

	OLED_ShowStr(1, 1, ch, 1);

	while (1)
	{
		printf("\nW25Q128_ID=0x%X\r\n",W25Q128_ID);
	}
}

/*********************************************END OF FILE**********************/
