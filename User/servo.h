#ifndef _servo_H_
#define _servo_H_

#include "stm32f10x.h"
#include <stdbool.h>

#define PWM_MAX 2400
#define PWM_MIN 600
#define PWM_NORM 1500
#define range_PWM 1800


void servoInit(void);

void servoMove(uint16_t id, uint16_t pwm);

void servoPWM1(uint16_t pwm);
void servoPWM2(uint16_t pwm);
void servoPWM3(uint16_t pwm);

extern const uint16_t PWM_max[];
extern const uint16_t PWM_min[];
extern const uint16_t PWM_norm[];

extern _Bool enable;

#endif

