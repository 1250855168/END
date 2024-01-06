#ifndef W25Q64_H
#define W25Q64_H

/*******SPI初始化函数******/
void spi_init(void);

/******通过SPI总线发送一个字节的数据******/
static uint8_t spi_flash_send_byte(uint8_t byte);

/********SPI_FLASH进入掉电模式********/
void spi_flash_powerdown(void);   

/********SPI_FLASH从掉电模式唤醒********/
void spi_flash_wakeup(void);   

/********读取FLASH器件ID******/
uint32_t spi_flash_read_deviceid(void);

/*******读取jedecid*******/
uint32_t spi_flash_read_jedecid(void);
/********写使能函数*********/
static void spi_flash_enable_write(void);

/********flash判忙函数*******/
static void spi_flash_waitforwriteend(void);

/*********flash读取*******/
void spi_flash_bufferread(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);

/*********flash写入*******/ 
void spi_flash_bufferwrite(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite); 

#endif