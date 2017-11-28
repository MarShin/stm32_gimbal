#include "led.h"

void LED_CONFIG (void)
{		
	/* Task 1: Configure the RGB LED as output */
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // Pin B5 - R
    // Pin B0 - G
    // Pin B1 - B
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = Red | Green | Blue;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		LED_off(Red);
		LED_off(Green);
		LED_off(Blue);
}

void LED_on (uint16_t pin){
	GPIOB->BRR = pin;
}

void LED_off (uint16_t pin){
	GPIOB->BSRR = pin;

}

