#include<bsp_oled.h>
#include "stm32f10x.h"
#include<bsp_delay.h>
#include "codetab.h"			//字库头文件

#define  OLED_Address   0X78

void I2C_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_initstruct;		//配置GPIO口结构体  -PB6 -PB7
  I2C_InitTypeDef	I2C_initstruct;			//配置硬件IIC结构体
	

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);	//使能对应时钟
	
//PB6 SCL  PB7 SDA
	GPIO_initstruct.GPIO_Mode = GPIO_Mode_AF_OD;		//复用开漏
	GPIO_initstruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_initstruct);
	
	I2C_DeInit(I2C1);
	I2C_initstruct.I2C_Ack =I2C_Ack_Enable;			//IIC应答信号使能
	I2C_initstruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	//设置从机地址长度为7位
	I2C_initstruct.I2C_ClockSpeed = 400000;			//设置目标周期 400khz
	I2C_initstruct.I2C_DutyCycle = I2C_DutyCycle_2;	//设置占空比为2：1
	I2C_initstruct.I2C_Mode = I2C_Mode_I2C;			//设置为IIC模式
	I2C_initstruct.I2C_OwnAddress1 = 0x30;			//设置主机地址

	I2C_Init(I2C1, &I2C_initstruct);
  	I2C_Cmd(I2C1, ENABLE);
	
}
//IIC写字节函数
void I2C_WriteByte(uint8_t addr,uint8_t data)		
{	//传参数，代表地址和数据
	
	while(I2C_GetFlagStatus( I2C1,I2C_FLAG_BUSY));		//等待总线空闲
	I2C_GenerateSTART(I2C1,ENABLE);						//IIC发送起始信号
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));//等待EV5=1,具体可以参考手册上面EV5事件
	I2C_Send7bitAddress(I2C1, OLED_Address, I2C_Direction_Transmitter);//发送从机7位地址
	while(!I2C_CheckEvent(I2C1,  I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//等待EV6=1
	I2C_SendData(I2C1,addr);		//发送地址字节
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));	//等待事件EV8
	I2C_SendData(I2C1,data);		//发送数据字节
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING));	//等待事件EV8

	I2C_GenerateSTOP(I2C1,ENABLE);	//IIC发送停止信号
}
//IIC写指令函数
void WriteCmd(unsigned char I2C_Command)
{
	I2C_WriteByte(0x00,I2C_Command);	//从0x00开始写指令	
}
void WriteDat(unsigned char I2C_Data)
{
	I2C_WriteByte(0x40,I2C_Data);	//从0x40开始写数据
}
//OLED初始化函数（厂家提供）
void OLED_Init(void)
{
	delay_us(100);
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); // 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel

}
//OLED初始化坐标位置，参数x y分别代表列和页
void OLED_Setpoint(unsigned char x,unsigned char y)
{
	WriteCmd(0xB0 + y);			//设置页 0xB0代表页为第1页
	WriteCmd((x&0xf0)>>4|0x10);	//设置列 高四位  获取高四位数据右移动4位并保存，且让第5位为1
	WriteCmd(x&0x0f|0x01);		//设置列 低四位	保留最后一位数据，并让最后一位数据置1
}
//全屏处理函数
void OLED_Fill(unsigned char Fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)		//页循环
	{
		WriteCmd(0xB0+m);	
		WriteCmd(0X00);		
		WriteCmd(0X10);		//设置列命令 高四位低四位全部写0，具体效果可查看oled指令表
		for(n=0;n<128;n++)	
		{//列循环，写如数据
			WriteDat(Fill_Data);
		}
	}
}
//清屏函数，在全屏处理函数上进行的修改
void OLED_Clear(void)
{
	OLED_Fill(0x00);		
}
//OLED开启函数，前面有提到
void OLED_OPEN(void)
{
	WriteCmd(0XAF);	//开启显示
	WriteCmd(0x8D);	//电荷泵设置
	WriteCmd(0x14);//打开电荷泵
}
//OLED关闭函数
void OLED_CLOSE(void)
{
	WriteCmd(0XAE);	//关闭显示
	WriteCmd(0x8D);	//电荷泵设置
	WriteCmd(0x10);	//关闭电荷泵
}

//OLED显示字符串
void OLED_ShowStr(unsigned char x,unsigned char y,unsigned char ch[],unsigned char TextSize)	
{//传参，x y 分别代表列和页  ch[] 代表字符串  TextSize代表显示字符串大小
	unsigned char c = 0,i=0,j=0;
	switch(TextSize)
	{
		case 1: 		//6x8大小的字符串
		{
			while(ch[j]!='\0')		//遍历到字符串尾
			{
				c = ch[j]-32;		//这句话具体参考字库字母的位置以及对应ASCII码表
				if(x>126)		//即将写满这一页
				{
					x=0;		//回到第一列
					y++;		//页往下翻
				}
				OLED_Setpoint(x,y);//重新置点
				for(i=0;i<6;i++)
					WriteDat( F6x8[c][i] );		//写入字库数据
				x+=6;		
				j++;		
			}
		}break;
		case 2:				//8x16大小的字符串
		{
				while(ch[j]!='\0')
				{
					c = ch[j]-32;
					if(x>120)
					{
						x=0;		
						y++;		
					}
					OLED_Setpoint(x,y);
					for(i=0;i<8;i++)	
						WriteDat(F8X16[c*16+i]);	//上一页写一半
					OLED_Setpoint(x,y+1);			//将页置一行后继续写，如果没有这句话，效果直接只有字的一半
					for(i=8;i<16;i++)				//下一页写一半
						WriteDat(F8X16[c*16+i]);
					x+=8;
					j++;
				}
		}break;
	}
}

// //OLED显示汉字
// void OLED_ShowCharactor(unsigned char x,unsigned char y,unsigned char N)
// {
// 	unsigned char i = 0;
// 	unsigned int addr = 32*N;		//字库汉字的地址
// 	OLED_Setpoint(x,y);
// 	for(i=0;i<16;i++)
// 	{
// 		WriteDat(F16X16[addr]);
// 		addr+=1;
// 	}
// 	OLED_Setpoint(x,y+1);
// 	for(i=0;i<16;i++)
// 	{
// 		WriteDat(F16X16[addr]);
// 		addr+=1;
// 	}
// 	//调用字库一般是要什么汉字就放什么汉字的码进去
// }
// //oled显示图片
// void OLED_ShowPicture(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
// {
// 	unsigned int j=0;
// 	unsigned char x,y;
// 	if(y1%8==0)		//分页，一页8行
// 		y1 = y1/8;
// 	else
// 		y1 = y1/8 + 1;
// 	for(y=y0;y<y1;y++)		//画点函数
// 	{
// 		OLED_Setpoint(x0,y);
// 		for(x=x0;x<x1;x++)
// 		{
// 			WriteDat(BMP[j++]);
// 		}
// 	}

// }		

