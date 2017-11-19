#ifndef _joystick_H_
#define _joystick_H_

#include "stm32f10x.h"

#define ADC1_DR_addr ((uint32_t)0x4001244C)
#define arraySize 2

void DMA_init(void);
void joystickInit(void);

extern volatile u16 ADC_val[arraySize];

#endif
