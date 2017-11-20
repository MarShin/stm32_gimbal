#include "servo.h"

void servoInit(void){
	
	//3 pwm: PA1 - OC2, PA2 - OC3, PA3 - OC4
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	
		
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
  TIM_TimeBaseStructure.TIM_Period = 14339; //<- TIMx_ARR register
  TIM_TimeBaseStructure.TIM_Prescaler = 100;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = PWM_NORM;  //<- TIMx_CCRx register
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC2Init(TIM2, &TIM_OCInitStructure); //PA1
	TIM_OC3Init(TIM2, &TIM_OCInitStructure); //PA2
	TIM_OC4Init(TIM2, &TIM_OCInitStructure); //PA3
	
	//Allow auto reload for OC_reg and ARR_reg
	//TIM_ARRPreloadConfig(TIM2,ENABLE); 
	TIM_OC2PreloadConfig(TIM2,ENABLE);
	TIM_OC3PreloadConfig(TIM2,ENABLE);
	TIM_OC4PreloadConfig(TIM2,ENABLE);
	
	TIM_Cmd(TIM2,ENABLE);

}

void servoStop(void){
	TIM2->CCR2 = 0;
	TIM2->CCR3 = 0;
	TIM2->CCR4 = 0;
	
}


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
}
