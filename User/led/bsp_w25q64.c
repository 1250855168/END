#include"bsp_w25q64.h"

#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 
#define WIP_Flag                  0x01  /* 忙标志 */
#define Dummy_Byte                0xFF

/*******SPI初始化函数******/
void spi_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;  //CLK管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	//CS管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	 SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	 SPI_InitStructure.SPI_Mode = SPI_Mode_Master;					//SPI主机模式
	 SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;			//8bit数据位
	 SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						//CPOL为高  SCK空闲为高
	 SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;					//偶数边沿采集		
	 SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//CS软件控制
	 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//fpclk 2分频
	 SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;		//高位先行
	 SPI_InitStructure.SPI_CRCPolynomial = 7;
	 SPI_Init(SPI2, &SPI_InitStructure);
	
	 SPI_Cmd(SPI2, ENABLE);//使能SPI2												
}
/******通过SPI总线发送一个字节的数据******/
static uint8_t spi_flash_send_byte(uint8_t byte)
{
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
  SPI_I2S_SendData(SPI2, byte);
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
  return SPI_I2S_ReceiveData(SPI2);
}
/********SPI_FLASH进入掉电模式********/
void spi_flash_powerdown(void)   
{ 
  SPI_FLASH_CS_LOW();
  spi_flash_send_byte(W25X_PowerDown);//发送掉电命令
  SPI_FLASH_CS_HIGH();
}   
/********SPI_FLASH从掉电模式唤醒********/
void spi_flash_wakeup(void)   
{
  SPI_FLASH_CS_LOW();
  spi_flash_send_byte(W25X_ReleasePowerDown);//发送唤醒命令
  SPI_FLASH_CS_HIGH();      
}
/********读取FLASH器件ID******/
uint32_t spi_flash_read_deviceid(void)
{
  uint32_t Temp = 0;

  SPI_FLASH_CS_LOW();

  spi_flash_send_byte(W25X_DeviceID);
  spi_flash_send_byte(Dummy_Byte);
  spi_flash_send_byte(Dummy_Byte);
  spi_flash_send_byte(Dummy_Byte);
  
  Temp = spi_flash_send_byte(Dummy_Byte);

  SPI_FLASH_CS_HIGH();
  return Temp;
}
/*******读取jedecid*******/
uint32_t spi_flash_read_jedecid(void)
{
  uint32_t temp = 0, temp0 = 0, temp1 = 0, temp2 = 0;

  SPI_FLASH_CS_LOW();
  spi_flash_send_byte(W25X_JedecDeviceID);
  temp0 = spi_flash_send_byte(Dummy_Byte);
  temp1 = spi_flash_send_byte(Dummy_Byte);
  temp2 = spi_flash_send_byte(Dummy_Byte);
  SPI_FLASH_CS_HIGH();

  temp = (temp0 << 16) | (temp1 << 8) | temp2;

  return temp;
}
/********写使能函数*********/
static void spi_flash_enable_write(void)
{
  SPI_FLASH_CS_LOW();
  spi_flash_send_byte(W25X_WriteEnable);//写使能命令
  SPI_FLASH_CS_HIGH();
}
/********flash判忙函数*******/
static void spi_flash_waitforwriteend(void)
{
  uint8_t flash_status = 0;

  SPI_FLASH_CS_LOW();

  spi_flash_send_byte(W25X_ReadStatusReg);
  do
  {
    flash_status = spi_flash_send_byte(Dummy_Byte);	 
  }
  while ((flash_status & WIP_Flag) == SET); /* flash忙 */

  SPI_FLASH_CS_HIGH();
}
/*********flash读取*******/
void spi_flash_bufferread(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  SPI_FLASH_CS_LOW();

  spi_flash_send_byte(W25X_ReadData);
  spi_flash_send_byte((ReadAddr & 0xFF0000) >> 16);
  spi_flash_send_byte((ReadAddr& 0xFF00) >> 8);
  spi_flash_send_byte(ReadAddr & 0xFF);

  while (NumByteToRead--) 
  {
    *pBuffer = spi_flash_send_byte(Dummy_Byte);//读出
    pBuffer++;
  }

  SPI_FLASH_CS_HIGH();
}
/*********flash写入*******/ 
void spi_flash_bufferwrite(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF = W25QXX_BUFFER;
   
 	secpos=WriteAddr>>12;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 
 	if(NumByteToWrite<=secremain)
	{
		secremain=NumByteToWrite;//不大于4096个字节
	}
	while(1) 
	{	
		spi_flash_bufferread(W25QXX_BUF,secpos<<12,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)
			{
				break;//需要擦除 
			} 	  
		}
		if(i<secremain)//需要擦除
		{
			spi_flash_sectorerase(secpos);		//擦除这个扇区
			for(i=0;i<secremain;i++)	   		//复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			spi_flash_write_nocheck(W25QXX_BUF,secpos<<12,4096);//写入整个扇区  
		}
		else 
		{
			spi_flash_write_nocheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 
		}				   
		if(NumByteToWrite==secremain)
		{
			break;//写入结束了
		}
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

			pBuffer+=secremain;  				//指针偏移
			WriteAddr+=secremain;				//写地址偏移	   
			NumByteToWrite-=secremain;			//字节数递减
			if(NumByteToWrite>4096)
			{
				secremain=4096;//下一个扇区还是写不完
			}
			else 
			{
				secremain=NumByteToWrite;		//下一个扇区可以写完了
			}
		}	 
	}	 
}

