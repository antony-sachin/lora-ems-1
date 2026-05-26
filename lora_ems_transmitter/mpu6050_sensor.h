#ifndef MPU6050_SENSOR_H
#define MPU6050_SENSOR_H

#include <Arduino.h>
#include <Adafruit_MPU6050.h>

// Global variables to hold actual values
extern float accX, accY, accZ;
extern float gyroX, gyroY, gyroZ;

void initMPU6050();
void readMPU6050();
void setMPU6050Sleep(bool sleepState);

#endif