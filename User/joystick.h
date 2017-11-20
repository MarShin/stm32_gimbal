#ifndef _joystick_H_
#define _joystick_H_

#include "stm32f10x.h"

#define ADC1_DR_addr ((uint32_t)0x4001244C)
#define arraySize 2

/*
ADC1
ADC_val[0] x - PC4 - CH14
ADC_val[1] y - PC5 - CH15
*/

void DMA_init(void);
void joystickInit(void);
void getJoystick(void);

void buttonInit(void);


extern volatile u16 ADC_val[arraySize];
extern volatile u16 stick_val[arraySize];

#endif
