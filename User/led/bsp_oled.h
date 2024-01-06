#ifndef BSP_OLED_H
#define BSP_OLED_H

#include "stm32f10x.h"

void I2C_Configuration(void);

//IIC写字节函数
void I2C_WriteByte(uint8_t addr,uint8_t data);		

//IIC写指令函数
void WriteCmd(unsigned char I2C_Command);

void WriteDat(unsigned char I2C_Data);

//OLED初始化函数（厂家提供）
void OLED_Init(void);

//OLED初始化坐标位置，参数x y分别代表列和页
void OLED_Setpoint(unsigned char x,unsigned char y);

//全屏处理函数
void OLED_Fill(unsigned char Fill_Data);

//清屏函数，在全屏处理函数上进行的修改
void OLED_Clear(void);

//OLED开启函数，前面有提到
void OLED_OPEN(void);

//OLED关闭函数
void OLED_CLOSE(void);


//OLED显示字符串
void OLED_ShowStr(unsigned char x,unsigned char y,unsigned char ch[],unsigned char TextSize);

#endif
