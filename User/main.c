#include "stm32f10x.h"
#include "lcd.h"

#include "math.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*-----------------------------------------------------------*/
#include "joystick.h"
#include "i2c_sensors.h"
#include "systick.h"
#include "uart.h"
#include "led.h"
#include "AHRS.h"
#include "servo.h"

#define PI 3.1412

/* Private function prototypes -----------------------------------------------*/

void Delayus(int duration);
char buff[64];
char buff2[64];
//uint8_t count = 0;

int main(void)
{
	/*Init System*/
	LED_CONFIG();
	DELAY_Configuration();
	I2C_Configuration();
	mpu_6050_init();
	uart_init(COM1, 115200);
	joystickInit();		//Unknown error: LCD dependence on joystick init
	DMA_init();
	servoInit();
	buttonInit();
	
  //LCD_INIT(); 						// LCD_INIT
	//Data = Acce_Gyro_Value();
	//LCD_DrawString(5,10,"elec3300");
  LED_on(Blue);
	while (1) {
		mpu6050_get_value();
		get_gyro_rate();
		get_acc_value();
		getJoystick();
		MadgwickAHRSupdateIMU(gyro_x_rate*M_PI/180,gyro_y_rate*M_PI/180,gyro_z_rate*M_PI/180,\
		acc_x_temp,acc_y_temp,acc_z_temp);
		getRollPitchYaw();
		//printf("R:%f P:%f Y:%f\r\n",rpy[0],rpy[1],rpy[2]);
		//printf("x:%u",stick_val[0]);
		//printf("y:%u\r\n",stick_val[1]);
		//delay_ms(100);
		//LED_off(Blue);
		//delay_ms(100);
		delay_ms(10);
		//LCD_DrawString(5,25,"OK");
		//Delayus(5000); //Do not alter becoz is on9
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
			while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));
			//servoPWM1(MIN_PWM);
			TIM2->CCR2 = TIM2->CCR2 + 5;
			delay_ms(5);
			LED_on(Red);
			LED_off(Green);
			LED_off(Blue);
		}
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)){
			while (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13));
			TIM2->CCR2 = TIM2->CCR2 - 5;
			delay_ms(5);
			LED_on(Green);
			LED_off(Red);
			LED_off(Blue);		
		}
		printf("%d\r\n",TIM2->CCR2);
		
		
  }
}
