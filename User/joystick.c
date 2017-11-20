#include "joystick.h"

volatile u16 ADC_val[arraySize] = {0};
volatile u16 stick_val[arraySize] = {0};

void DMA_init(void){
	DMA_InitTypeDef DMA_InitStrcuture;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStrcuture.DMA_PeripheralBaseAddr = ADC1_DR_addr;
	DMA_InitStrcuture.DMA_MemoryBaseAddr = (uint32_t)ADC_val;
	DMA_InitStrcuture.DMA_BufferSize = arraySize;
	DMA_InitStrcuture.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStrcuture.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStrcuture.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStrcuture.DMA_PeripheralDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStrcuture.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStrcuture.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStrcuture.DMA_Priority = DMA_Priority_High;
	DMA_InitStrcuture.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStrcuture);
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	
	ADC_DMACmd(ADC1,ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

void getJoystick(void){
	uint8_t i = 0;
	for (i=0;i<2;i++){
		stick_val[i] = ADC_val[i];
	}
}

void joystickInit(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = arraySize;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 2, ADC_SampleTime_71Cycles5);
	ADC_Cmd(ADC1, ENABLE);
	/* Enable ADC1 reset calibration register */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
}

void buttonInit(void){
	GPIO_InitTypeDef GPIO_InitStructure;
    
  //Pin A0 - Key1
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	//Pin C13 - Key2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
