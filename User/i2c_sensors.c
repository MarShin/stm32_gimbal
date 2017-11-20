#include "i2c_sensors.h"

#define I2C_SPEED 100000//HZ


//------- define data for MPU6050 setup ------------
/*
#define   ACCEL_XOUT_H	59	// R  ACCEL_XOUT[15:8] 
#define   ACCEL_XOUT_L	60	// R  ACCEL_XOUT[7:0] 
#define   ACCEL_YOUT_H	61	// R  ACCEL_YOUT[15:8] 
#define   ACCEL_YOUT_L	62	// R  ACCEL_YOUT[7:0] 
#define   ACCEL_ZOUT_H	63	// R  ACCEL_ZOUT[15:8] 
#define   ACCEL_ZOUT_L	64	// R  ACCEL_ZOUT[7:0] 
#define   TEMP_OUT_H	65	// R  TEMP_OUT[15:8] 
#define   TEMP_OUT_L	66	// R  TEMP_OUT[7:0] 
#define   GYRO_XOUT_H	67	// R  GYRO_XOUT[15:8] 
#define   GYRO_XOUT_L	68	// R  GYRO_XOUT[7:0] 
#define   GYRO_YOUT_H	69	// R  GYRO_YOUT[15:8] 
#define   GYRO_YOUT_L	70	// R  GYRO_YOUT[7:0] 
#define   GYRO_ZOUT_H	71	// R  GYRO_ZOUT[15:8] 
#define   GYRO_ZOUT_L	72	// R  GYRO_ZOUT[7:0] 
*/

#define M_PI	3.14159265358979323846	/* pi */
#define MPU6050_SLA_ADR 0x68 
#define MPU6050_WADR MPU6050_SLA_ADR<<1
#define MPU6050_RADR (MPU6050_SLA_ADR<<1)|0x01
#define Res25_value 0x07	// Sample rate divider
#define Res26_value 0x00	// Configuration		    
#define Res27_value 0x00	// Gyroscope configuration: gyro_scale= +-250 degree/s; sen=131 LSB/degree/s
#define Res28_value 0x00  // Accelerometter configuration  : accelero_scale=+-2g ; sen=16384 LSB/g       
#define Res55_value 0x02	// By pass enable configuration
#define Res106_value 0x00 // user control
#define Res107_value 0x00 // do not reset all the device //0x80

#define Res25 25 //register address
#define Res26 26
#define Res27 27
#define Res28 28
#define Res55 55
#define Res106 106
#define Res107 107

#define start_read_address 59
#define gyro_zero_x 55
#define gyro_zero_y 8
#define gyro_zero_z -13

#define gyro_sensitivity 131
#define acce_sensitivity 16384
#define acc_x_gain 16418
#define acc_y_gain 16403
#define acc_z_gain 16600

//---------------- end of data for MPU 6050 ---------

//--------- variables for mpu6050 ----------
int16_t gyro_x,gyro_y,gyro_z;
int16_t acc_x,acc_y,acc_z;
float last_gyro_x_rate,last_gyro_y_rate,last_gyro_z_rate;
float gyro_x_rate,gyro_y_rate,gyro_z_rate;
float acc_x_angle,acc_y_angle;
float acc_x_temp,acc_y_temp,acc_z_temp;
float dt,temp;
uint8_t i2c_sdata[20],i2c_rdata[20];
//------------------------------------------

//--------- variables for hmc5883l ---------
uint8_t hmc5883_wdata[3];
uint8_t hmc5883_rdata[6];
float raw_x,raw_y,raw_z;
int16_t raw_x_temp,raw_y_temp,raw_z_temp;
uint16_t mp;//mag_pointer

float mag_x,mag_y,mag_z;
float heading,heading_temp,headingDegrees;

//float heading2,heading2Degrees;
float x_h,y_h;//for tilt process
//------------------------------------------

void I2C_Configuration(void)
{
	GPIO_InitTypeDef GPIO_I2C_Config;
	I2C_InitTypeDef I2C_Config;
	
	//--enable clock source for i2c and gpiob --
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//------------------------------------------
	
	//-- configuration pinb6 and pinb7 as out_put_od --
	GPIO_I2C_Config.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_I2C_Config.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_I2C_Config.GPIO_Mode = GPIO_Mode_AF_OD; 
	GPIO_Init(GPIOB, &GPIO_I2C_Config);
	//-------------------------------------------------
	
	//-- configuration i2c ----------------------------
	I2C_Config.I2C_Ack=I2C_Ack_Enable;
	I2C_Config.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
	I2C_Config.I2C_ClockSpeed=I2C_SPEED;
	I2C_Config.I2C_DutyCycle=I2C_DutyCycle_2;
	I2C_Config.I2C_Mode=I2C_Mode_I2C;
	I2C_Config.I2C_OwnAddress1=0x01;
	I2C_Init(I2C2,&I2C_Config);
	I2C_Cmd(I2C2,ENABLE);
	//-------------------------------------------------
}

void mpu6050_write_block(uint8_t adr, uint8_t data[], uint8_t data_len)//okie
{
	uint8_t i;
	// Send START condition 
  I2C_GenerateSTART(I2C2, ENABLE);
	//loop until match the 'start condition' flag
	while(!I2C_GetFlagStatus(I2C2, I2C_FLAG_SB));
	
	// send mpu6050 addres and check until it's done
	I2C_Send7bitAddress(I2C2, MPU6050_WADR, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	// send mpu6050 the begining address where we want to send data, check until it's done
	I2C_SendData(I2C2,adr);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	//then send continous data in data[] array
	for(i=0;i<data_len;i++)
	{
		I2C_SendData(I2C2,data[i]);
		while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		delay_ms(1);
	}
	
	// send stop condition and wait until done 
	I2C_GenerateSTOP(I2C2, ENABLE);
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF));
}

void mpu6050_read_block(uint8_t adr, uint8_t data[], uint8_t data_len)
//adr: addres of the first reg what we read from
//data[]: array of data, which we store read data in
//data_len: lenght of reading data
{
	uint8_t i;
	
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY)){};
	// Send START condition 
  I2C_GenerateSTART(I2C2, ENABLE);
	//loop until match the 'start condition' flag
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
	
	
	// send mpu6050 7 bit address and a Write bit, check until done
	I2C_Send7bitAddress(I2C2, MPU6050_WADR, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){};
	//I2C_Cmd(I2C1, ENABLE);// clear all the event	
	
	
	// send mpu6050 the addres of the reg we want to read data from 
	I2C_SendData(I2C2,adr);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){};

	
	// send stop condition and wait until done 
	I2C_GenerateSTOP(I2C2, ENABLE);
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF)){};
	
	//----- send a repeat start to start reading data -----
	
	// Send START condition 
  I2C_GenerateSTART(I2C2, ENABLE);
	//loop until match the 'start condition' flag
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
	
	
	// send mpu6050 7 bit addres and Read command
	I2C_Send7bitAddress(I2C2, MPU6050_RADR, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){};

	
  i=0;
  while(data_len)
	{
		if(data_len==1)
		{
      I2C_AcknowledgeConfig(I2C2, DISABLE);
      I2C_GenerateSTOP(I2C2, ENABLE);
		}
		  if(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
     
      i2c_rdata[i] = I2C_ReceiveData(I2C2);
			i++;
      data_len--;
		}
	}
	I2C_AcknowledgeConfig(I2C2, ENABLE);
}

void mpu6050_get_value(void)
{	
	mpu6050_read_block(start_read_address,i2c_rdata,14);	
	acc_x=(int16_t)(i2c_rdata[0]<<8|i2c_rdata[1]);
	acc_y=(int16_t)(i2c_rdata[2]<<8|i2c_rdata[3]);
	acc_z=(int16_t)(i2c_rdata[4]<<8|i2c_rdata[5]);
	temp=(float)((int16_t)(i2c_rdata[6]<<8|i2c_rdata[7]))/340+36.53;
	gyro_x=(int16_t)(i2c_rdata[8]<<8|i2c_rdata[9]);
	gyro_y=(int16_t)(i2c_rdata[10]<<8|i2c_rdata[11]);
	gyro_z=(int16_t)(i2c_rdata[12]<<8|i2c_rdata[13]);
}

void mpu_6050_init(void)
{
	//reset mpu6050
	delay_ms(10);
	i2c_sdata[0]=0x80;
	mpu6050_write_block(Res107,i2c_sdata,1);
	delay_ms(10);
	
	//use internal clock
	i2c_sdata[0]=Res107_value;
	mpu6050_write_block(Res107,i2c_sdata,1);
	delay_ms(10);
	
	// enable i2c master mode, arm uc will contrl the mpu6050 bypass auxilary bus
	i2c_sdata[0]=Res106_value;
	mpu6050_write_block(Res106,i2c_sdata,1);
	delay_ms(10);
	
	//enable bypass i2c bus
	i2c_sdata[0]=Res55_value;
	mpu6050_write_block(Res55,i2c_sdata,1);
	delay_ms(10);
	
	//config our meure mode, read the specific res for more info
	i2c_sdata[0]=Res25_value;
	i2c_sdata[1]=Res26_value;
	i2c_sdata[2]=Res27_value;
	i2c_sdata[3]=Res28_value;
	mpu6050_write_block(Res25,i2c_sdata,4);
	delay_ms(10);
	GPIOB->ODR|=1<<0;
}

void get_gyro_rate(void)
{
	gyro_x_rate=(float) (gyro_x-gyro_zero_x)/gyro_sensitivity;	
	gyro_y_rate=(float) (gyro_y-gyro_zero_y)/gyro_sensitivity;	
	gyro_z_rate=(float) (gyro_z-gyro_zero_z)/gyro_sensitivity;	
}

void get_acc_value(void)
{
	acc_x_temp= (float)acc_x/acc_x_gain;
	acc_y_temp= (float)acc_y/acc_y_gain;
	acc_z_temp= (float)acc_z/acc_z_gain;// [acc_z]=g=9.8 m/s.s
}
/************************************************************************/
/* change acceleron value to degree                                     */
/************************************************************************/
/*
void get_acce_angle(void)
{
	acc_x_temp= (float)acc_x/acc_x_gain;
	acc_y_temp= (float)acc_y/acc_y_gain;
	acc_z_temp= (float)acc_z/acc_z_gain;// [acc_z]=g=9.8 m/s.s
	acc_x_angle=(180/M_PI)*atan(acc_y_temp/sqrt(pow(acc_x_temp,2)+pow(acc_z_temp,2)));
	acc_y_angle=(180/M_PI)*atan(-acc_x_temp/sqrt(pow(acc_y_temp,2)+pow(acc_z_temp,2)));
	
	if(acc_z<0)//kit bi dao nguoc: dung de tinh toan goc 0~(+-180) degrees
	{
		if(acc_x_angle<0)
		{
			acc_x_angle=-180-acc_x_angle;// 0>= acc_x_angle> -180
		}
		else
		{
			acc_x_angle=180-acc_x_angle;//0 =< acc_x_angle =< 180
		}
		
		if(acc_y_angle<0)
		{
			acc_y_angle=-180-acc_y_angle;// 0>= acc_x_angle> -180
		}
		else
		{
			acc_y_angle=180-acc_y_angle;//0 =< acc_x_angle =< 180
		}
	}
}
*/

void i2c_sensors_send_prcessing_data(float x_angle, float y_angle, float z_angle)
{
	int16_t pr_sdata[9];
	uint8_t i;
	pr_sdata[0]=(int16_t)x_angle;
	pr_sdata[1]=(int16_t)y_angle;
	pr_sdata[2]=(int16_t)z_angle;
	pr_sdata[3]=gyro_x;
	pr_sdata[4]=gyro_y;
	pr_sdata[5]=gyro_z;
	pr_sdata[6]=acc_x;
	pr_sdata[7]=acc_y;
	pr_sdata[8]=acc_z;
	for(i=0;i<8;i++)
	{
		printf("%d ,",pr_sdata[i]);
		delay_us(10);
	}
	printf("%d \r",pr_sdata[8]);
}
