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

#define PI 3.1412

/* Private function prototypes -----------------------------------------------*/

void Delayus(int duration);
char buff[64];
char buff2[64];

int main(void)
{
	/*Init System*/
	LED_CONFIG();
	DELAY_Configuration();
	I2C_Configuration();
	mpu_6050_init();
	uart_init(COM1, 256000);
	joystickInit();		//Unknown error: LCD dependence on joystick init
	DMA_init();

  //LCD_INIT(); 						// LCD_INIT
	//Data = Acce_Gyro_Value();
	//LCD_DrawString(5,10,"elec3300");
  while (1) {
		mpu6050_get_value();
		get_gyro_rate();
		get_acc_value();
		MadgwickAHRSupdateIMU(gyro_x_rate*M_PI/180,gyro_y_rate*M_PI/180,gyro_z_rate*M_PI/180,\
		acc_x_temp,acc_y_temp,acc_z_temp);
		LED_on(Blue);
		delay_ms(100);
		LED_off(Blue);
		delay_ms(100);
		//LCD_DrawString(5,25,"OK");
		//Delayus(5000); //Do not alter becoz is on9
  }
}
