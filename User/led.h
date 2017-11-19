#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"

#define ADC1_DR_addr ((uint32_t)0x4001244C)
#define arraySize 2

#define Red GPIO_Pin_5
#define Green GPIO_Pin_0
#define Blue GPIO_Pin_1

void LED_CONFIG (void);

void LED_on(uint16_t pin);
void LED_off(uint16_t pin);

#endif
