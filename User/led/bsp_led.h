#ifndef __LED_H
#define	__LED_H


#include "stm32f10x.h"    

#define LED2_GPIO_PORT    	GPIOA			              /* GPIO端口 */
#define LED2_GPIO_PIN		GPIO_Pin_2			        

#define LED3_GPIO_PORT    	GPIOA			              /* GPIO端口 */
#define LED3_GPIO_PIN		GPIO_Pin_3			 

/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1


#define LED2(a)	if (a)	\
					GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN)
          
#define LED3(a)	if (a)	\
					GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED3_GPIO_PORT,LED3_GPIO_PIN)


void LED_GPIO_Config(void);

#endif /* __LED_H */
