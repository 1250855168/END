#ifndef BSP_START_KEY_H
#define BSP_START_KEY_H


#define KEY1_GPIO_PIN  GPIO_Pin_0
#define KEY1_GPIO_PORT GPIOB


void Key_GPIO_Config(void);

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#endif