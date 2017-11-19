#ifndef _IC_SENSOR_H_
#define _IC_SENSOR_H_

#include "stm32f10x.h"
#include "systick.h"
#include "stm32f10x_i2c.h"
#include <stdio.h>
#include <math.h>

//--------- variables for mpu6050 ----------
extern int16_t gyro_x,gyro_y,gyro_z;
extern int16_t acc_x,acc_y,acc_z;
extern float last_gyro_x_rate,last_gyro_y_rate,last_gyro_z_rate;
extern float gyro_x_rate,gyro_y_rate,gyro_z_rate;
extern float acc_x_angle,acc_y_angle;
extern float acc_x_temp,acc_y_temp,acc_z_temp;
extern float dt,temp;
extern uint8_t i2c_sdata[20],i2c_rdata[20];
//------------------------------------------

//--------- functions ----------------------
void I2C_Configuration(void);
void mpu6050_write_block(uint8_t adr, uint8_t data[], uint8_t data_len);//okie
void mpu6050_read_block(uint8_t adr, uint8_t data[], uint8_t data_len);
void mpu6050_get_value(void);
void mpu_6050_init(void);
void get_gyro_rate(void);
void get_acc_value(void);
//void get_acce_angle(void);
void i2c_sensors_send_prcessing_data(float x_angle, float y_angle, float z_angle);

//------------------------------------------
#endif
