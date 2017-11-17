#include "stm32f10x.h"
#include "lcd.h"

#include "math.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*-----------------------------------------------------------*/
#include "mpu6050.h"

#define PI 3.1412

/* Private function prototypes -----------------------------------------------*/

void Delayus(int duration);
void joystickInit(void);
void DMA_init(void);
char buff[64];
char buff2[64];
mpudata Data;

volatile u16 ADC_val[2] = {0};

#define ADC1_DR_addr ((uint32_t)0x4001244C)
void DMA_init(void){
	DMA_InitTypeDef DMA_InitStrcuture;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStrcuture.DMA_PeripheralBaseAddr = ADC1_DR_addr;
	DMA_InitStrcuture.DMA_MemoryBaseAddr = (uint32_t)ADC_val;
	DMA_InitStrcuture.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStrcuture.DMA_BufferSize = 2;
	DMA_InitStrcuture.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStrcuture.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStrcuture.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStrcuture.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStrcuture.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStrcuture.DMA_Priority = DMA_Priority_High;
	DMA_InitStrcuture.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStrcuture);
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	
	ADC_DMACmd(ADC1,ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

void joystickInit(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 2;

	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 2, ADC_SampleTime_55Cycles5);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

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

int main(void)
{
	u16 x;
	u16 y;
	u16 i=0;
	
	joystickInit();
	DMA_init();
	init_i2c();
	mpu6050_init();
  LCD_INIT(); 						// LCD_INIT
	//Data = Acce_Gyro_Value();
	LCD_DrawString(5,10,"elec3300");;
	
  while (1) {
	/* Please add code below to complete the LAB6 */
  /* You might want to create your own functions */	
		//initADC(ADC_Channel_14);
		x = ADC_val[0];
		y = ADC_val[1];
		
		Data = Acce_Gyro_Value();
		sprintf(buff,"a_X: %d",Data.acc_x);
		LCD_DrawString(5,60,buff);
		sprintf(buff,"a_Y: %d",Data.acc_y);
		LCD_DrawString(5,75,buff);
		sprintf(buff,"a_Z: %d",Data.acc_z);
		LCD_DrawString(5,90,buff);
		sprintf(buff,"g_X: %d",Data.gyro_x);
		LCD_DrawString(5,105,buff);
		sprintf(buff,"g_Y: %d",Data.gyro_y);
		LCD_DrawString(5,120,buff);
		sprintf(buff,"g_Z: %d",Data.gyro_z);
		LCD_DrawString(5,135,buff);

		sprintf(buff,"x: %u",x);
		sprintf(buff2,"y: %u",y);
		LCD_DrawString(5,30,buff);
		//LCD_DrawString(5,50,"y:");
		LCD_DrawString(5,45,buff2);
		Delayus(5000); //Do not alter becoz is on9
  }
}

void Delayus(int duration)
{
		while(duration--) 
		{
			int i=0x02;
			while(i--)
			__asm("nop");
		}
}
