#include "bsp_Adc.h"

uint16_t AD_Value[2];
 
void My_ADC_Init()
{
 	 GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//1、使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA1时钟使能 
	
	//2、选择ADC分频
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//3、初始化GPIO
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//4、选择ADC通道
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	
	//5、初始化ADC
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;//ADC工作模式。独立工作模式
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;  //扫描模式（多个通道）	
	ADC_InitStruct.ADC_ContinuousConvMode =ENABLE;//连续转换
	//不使用外部触发，使用内部软件触发
    //ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	 ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1; // 使用外部触发模式（TIM1通道1触发源）
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStruct.ADC_NbrOfChannel =2;//需要扫描的通道数
	ADC_Init(ADC1, &ADC_InitStruct);//ADC初始化
	
	//6、初始化DMA
	//DMA外设地址,ADC数据储存地址
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	//外设非增量模式，只读取ADC储存地址里面的
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//外设数据长度:16位
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;//存储器地址
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
	//接收到数据长度:16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//外设到储存器(数据传输方向)
	DMA_InitStructure.DMA_BufferSize = 2;//数据传输量 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// 循环扫描
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//初始化DMA 

	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); // 使能传输完成中断
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    //  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	//6、使能DMA
	DMA_Cmd(DMA1_Channel1,ENABLE);
	//7、使能ADC触发DMA
	ADC_DMACmd(ADC1,ENABLE);
	//8、使能ADC
	ADC_Cmd(ADC1, ENABLE);
	
	//9、校准ADC数据
	ADC_ResetCalibration(ADC1);//开始复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);//获取ADC重置校准寄存器的状态
	ADC_StartCalibration(ADC1);//开始指定ADC的校准状态
	while(ADC_GetCalibrationStatus(ADC1) == SET);//获取指定ADC的校准状态
	
	//使能指定的ADC1的软件转换启动功能	（连续转换时，初始化一次就行了）
    //ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	ADC_ExternalTrigConvCmd(ADC1, ENABLE); // 设置外部触发模式使能
}

// 数据滤波 10个数据 平均值滤波



//接收完ADC信号进入DMA中断，转换电压打印输出
void DMA1_Channel1_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA1_IT_TC1) != RESET)
    {

        DMA_ClearITPendingBit(DMA1_IT_TC1);
    }
}
