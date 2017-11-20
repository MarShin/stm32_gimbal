#ifndef _servo_H_
#define _servo_H_

#include "stm32f10x.h"

#define PWM_MAX 144
#define PWM_MIN 72
#define PWM_NORM 108
#define range_PWM 72


void servoInit(void);
void servoPWM1(uint16_t pwm);
void servoPWM2(uint16_t pwm);
void servoPWM3(uint16_t pwm);

#endif

