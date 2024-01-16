// #include "bsp_Adc.h"

// void Adc_Init(void)
// {
//     ADC_InitTypeDef ADC_InitStructure;
//     GPIO_InitTypeDef GPIO_InitStructure;
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能GPIOA时钟
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  // 使能ADC1通道时钟

//     //     // 配置输入引脚，PA0连接ADC1通道0
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2;
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//     GPIO_Init(GPIOA, &GPIO_InitStructure);

//     //     // ADC1初始化
//     ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                    // 独立ADC模式
//     ADC_InitStructure.ADC_ScanConvMode = ENABLE;                          // 打开扫描方式
//     ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                    // 连续转换模式
//     ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1; // 使用外部触发模式（TIM1通道1触发源）
//     ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                // 采集数据右对齐
//     ADC_InitStructure.ADC_NbrOfChannel = 3;                               // 要转换的通道数目
//     ADC_Init(ADC1, &ADC_InitStructure);

//     RCC_ADCCLKConfig(RCC_PCLK2_Div6);                                            // 配置ADC时钟，为PCLK2的6分频，即12Hz
//     ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5); // 配置ADC1通道0为239.5个采样周期
//     ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5); // 配置ADC1通道1为239.5个采样周期
//     ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5); // 配置ADC1通道0为239.5个采样周期

//     // 使能ADC、DMA
//     ADC_DMACmd(ADC1, ENABLE);
//     ADC_Cmd(ADC1, ENABLE);

//     ADC_ResetCalibration(ADC1); // 复位校准寄存器
//     while (ADC_GetResetCalibrationStatus(ADC1))
//         ; // 等待校准寄存器复位完成

//     ADC_StartCalibration(ADC1); // ADC校准
//     while (ADC_GetCalibrationStatus(ADC1))
//         ; // 等待校准完成

//     ADC_ExternalTrigConvCmd(ADC1, ENABLE); // 设置外部触发模式使能
// }

// volatile uint16_t ADC_ConvertedValue[2];  // ADC采样的数据
// #define ADC1_DR_Address ((u32)0x4001244C) // ADC1的地址

// u16 V;
// float adc_light;
// float adc_temp;
// float test;

// // DMA1配置
// void DMA1_Init(void)
// {
//     DMA_InitTypeDef DMA_InitStructure;
//     NVIC_InitTypeDef NVIC_InitStructure;
//     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // 使能ADC1通道时钟

//     // DMA1初始化
//     DMA_DeInit(DMA1_Channel1);
//     DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;                 // ADC1地址
//     DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;       // 内存地址
//     DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          // 方向(从外设到内存)
//     DMA_InitStructure.DMA_BufferSize = 3;                                       // 传输内容的大小
//     DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            // 外设地址固定
//     DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;                    // 内存地址固定
//     DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 外设数据单位
//     DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         // 内存数据单位
//     DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             // DMA模式：循环传输
//     DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         // 优先级：高
//     DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                // 禁止内存到内存的传输
//     DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                // 配置DMA1

//     DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); // 使能传输完成中断
//     NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//     //  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//     NVIC_Init(&NVIC_InitStructure);

//     DMA_Cmd(DMA1_Channel1, ENABLE);
// }

// // 接收完ADC信号进入DMA中断，转换电压打印输出
// void DMA1_Channel1_IRQHandler(void)
// {
//     if (DMA_GetITStatus(DMA1_IT_TC1) != RESET)
//     {
//         V = ADC_ConvertedValue[0];
//         adc_light = (float)V * (3.3 / 4096);

//         V = ADC_ConvertedValue[1];
//         adc_temp = (float)V * (3.3 / 4096);

//         DMA_ClearITPendingBit(DMA1_IT_TC1);
//     }
// }

