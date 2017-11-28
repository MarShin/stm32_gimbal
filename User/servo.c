#include "servo.h"


const uint16_t PWM_max[] = {2400,2350,2400};
const uint16_t PWM_min[] = {680,710,600};
const uint16_t PWM_norm[]= {1500,1530,1500};

_Bool enable = false;

void servoInit(void){
	
	//3 pwm: PA1 - OC2, PA2 - OC3, PA3 - OC4
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
	
		
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
  //TIM_TimeBaseStructure.TIM_Period = 14339; //<- TIMx_ARR register
  //TIM_TimeBaseStructure.TIM_Prescaler = 100;
	TIM_TimeBaseStructure.TIM_Period = 19999; //<- TIMx_ARR register
  TIM_TimeBaseStructure.TIM_Prescaler = 72;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = PWM_NORM;  //<- TIMx_CCRx register
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC2Init(TIM5, &TIM_OCInitStructure); //PA1
	TIM_OC3Init(TIM5, &TIM_OCInitStructure); //PA2
	TIM_OC4Init(TIM5, &TIM_OCInitStructure); //PA3
	
	//Allow auto reload for OC_reg and ARR_reg
	//TIM_ARRPreloadConfig(TIM2,ENABLE); 
	TIM_OC2PreloadConfig(TIM5,ENABLE);
	TIM_OC3PreloadConfig(TIM5,ENABLE);
	TIM_OC4PreloadConfig(TIM5,ENABLE);
	
	TIM_Cmd(TIM5,ENABLE);
	
	servoMove(0,PWM_norm[0]);
	servoMove(1,PWM_norm[1]);
	servoMove(2,PWM_norm[2]);
	

}

void servoStop(void){
	TIM5->CCR2 = 0;
	TIM5->CCR3 = 0;
	TIM5->CCR4 = 0;
	
}

void servoMove(uint16_t id, uint16_t pwm){
	pwm = (pwm>PWM_max[id]) ? PWM_max[id]:pwm;
	pwm = (pwm<PWM_min[id]) ? PWM_min[id]:pwm;
	//if (!enable){
	//return;
	//}
	switch (id){
		case 0:
			TIM5->CCR2 = pwm; //PA1: X axis
			break;
		case 1:
			TIM5->CCR3 = pwm; //PA2: Y axis
			break;
		case 2:
			TIM5->CCR4 = pwm; //PA3
			break;
		default:
			break;
	}
}
/*
void servoPWM1(uint16_t pwm){
	pwm = (pwm>PWM_MAX)? PWM_MAX: pwm;
	pwm = (pwm<PWM_MIN)? PWM_MIN: pwm;
	TIM2->CCR2 = pwm;
}

void servoPWM2(uint16_t pwm){
	pwm = (pwm>PWM_MAX)? PWM_MAX: pwm;
	pwm = (pwm<PWM_MIN)? PWM_MIN: pwm;
	TIM2->CCR3 = pwm;
}

void servoPWM3(uint16_t pwm){
	pwm = (pwm>PWM_MAX)? PWM_MAX: pwm;
	pwm = (pwm<PWM_MIN)? PWM_MIN: pwm;
	TIM2->CCR4 = pwm;
}*/
