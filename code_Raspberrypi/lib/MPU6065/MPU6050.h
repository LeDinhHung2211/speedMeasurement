#ifndef MPU6050_H
#define MPU6050_H

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringPiSPI.h>
#include <math.h>
#define INT	29
//------register address(dec)------//

#define ACC_X 59
#define ACC_Y 61
#define ACC_Z 63
#define TEMP 65
#define GYRO_X 67
#define GYRO_Y 69
#define GYRO_Z 71

int mpu_address;
void initMpu6050(int sample, int DLPF, int gyro, int acc){
	wiringPiI2CWriteReg8(mpu_address,0x19,sample);//set sample rate divider 1ms
	wiringPiI2CWriteReg8(mpu_address,0x1a,DLPF);//configuration
	wiringPiI2CWriteReg8(mpu_address,0x6b,0x01);//Power mode: no sleep, Clock Source: from X axis gyroscope
	wiringPiI2CWriteReg8(mpu_address,0x1b,gyro);//Gyroscope Scale: 500 deg/s
	wiringPiI2CWriteReg8(mpu_address,0x1c,acc);//Accelerometer Scale: 8g
}
int16_t read_data(uint8_t address){
	int16_t data;
	data = wiringPiI2CReadReg8(mpu_address, address)<<8;
	data = data | wiringPiI2CReadReg8(mpu_address, address+1);
	return data;
}

struct mpu{
	float x;
	float y;
	float z;
};

typedef struct mpu MPU6050;

MPU6050 read_sensor(uint8_t address, float sensitivity){
	MPU6050 value;
	value.x = (float)read_data(address)/sensitivity*9.81;
	value.y = (float)read_data(address+2)/sensitivity*9.81;
	value.z = (float)read_data(address+4)/sensitivity*9.81;
	return value;
};


#endif 

