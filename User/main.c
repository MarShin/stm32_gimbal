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
u16 ticks_img = 0;
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
	ticks_init();
	buttonInit();
  LED_on(Blue);
	printf("halo");
	while (1) {
		u16 diff = abs(get_ticks() - ticks_img);
		float roll, pitch, yaw;
		if ( !(diff > 10)){ //10
			continue;
		}
		ticks_img = get_ticks();
		mpu6050_get_value();
		get_gyro_rate();
		get_acc_value();
		getJoystick();
		MadgwickAHRSupdateIMU(gyro_x_rate*M_PI/180,gyro_y_rate*M_PI/180,gyro_z_rate*M_PI/180,\
		acc_x_temp,acc_y_temp,acc_z_temp);
		getRollPitchYaw();
		printf("ticks: %u\r\n",diff);

		roll = (rpy[2]-90)/180.0 *2000.0 +2500;
		pitch = (rpy[1]-90)/180.0 *2000.0 +2500;
		yaw = (rpy[0]-90)/180.0 *2000.0 +2500;
		
		//roll = (rpy[2]+90)/180.0;
		//pitch = (rpy[1]+90)/180.0;
		//yaw = (rpy[0]+90)/180.0;
		printf("R:%f P:%f Y:%f\r\n",rpy[0],rpy[1],rpy[2]);
	  printf("ANGLE R:%f P:%f Y:%f\r\n",roll,pitch,yaw);
		printf("\n");
		
		servoMove(1,pitch);
		servoMove(0,-roll);
		
//		if ((stick_val[0] > 10.0) || (stick_val[0] < -10.0 ) ){
//			printf("move X %f\r\n", TIM5->CCR2+(stick_val[0]));
//			servoMove(0,TIM5->CCR2+stick_val[0]);
//		}
//		if ((stick_val[1] > 10.0 ) || (stick_val[1] < -10.0 )){
//			printf("move Y %f\r\n", TIM5->CCR3+(stick_val[1]));
//			servoMove(1,TIM5->CCR3+stick_val[1]);
//		} 
		
		if (!(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2))){
			while (!(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)));
			servoMove(0,PWM_norm[0]);
			servoMove(1,PWM_norm[1]);
			servoMove(2,PWM_norm[2]);
			LED_on(Red);
			LED_off(Green);
			LED_on(Blue);
		}
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
			while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));

			
			servoMove(0,PWM_max[0]);
			servoMove(1,PWM_max[1]);
			servoMove(2,PWM_max[2]);
			//servoPWM1(MIN_PWM);
			//TIM2->CCR2 = 2350;
			//TIM2->CCR3 = 2350;
			//TIM2->CCR4 = 2350;
			//delay_ms(5);
			LED_on(Red);
			LED_off(Green);
			LED_off(Blue);
		}
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)){
			while (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13));
			

		
		//	servoMove(0,PWM_min[0]);
		//	servoMove(1,PWM_min[1]);
		//	servoMove(2,PWM_min[2]);
			//enable = true;
			LED_on(Green);
			LED_off(Red);
			LED_off(Blue);	

			while(1) {
				if ((stick_val[0] > 10.0) || (stick_val[0] < -10.0 ) ){
					printf("move X %f\r\n", TIM5->CCR2+(stick_val[0]));
					servoMove(0,TIM5->CCR2+stick_val[0]);
				}
			
				if ((stick_val[1] > 10.0 ) || (stick_val[1] < -10.0 )){
					printf("move Y %f\r\n", TIM5->CCR3+(stick_val[1]));
					servoMove(1,TIM5->CCR3+stick_val[1]);
				} 
			}
		}
		//printf("%d\r\n",TIM2->CCR2);
		
		
  }
}