#include "mpu6050_sensor.h"
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

// Define the global variables
float accX = 0, accY = 0, accZ = 0;
float gyroX = 0, gyroY = 0, gyroZ = 0;

void initMPU6050() {
  if (!mpu.begin(0x68)) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) { delay(10); }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("MPU6050 Initialized!");
}

void readMPU6050() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Store actual values in global variables
  accX = a.acceleration.x;
  accY = a.acceleration.y;
  accZ = a.acceleration.z;
  gyroX = g.gyro.x;
  gyroY = g.gyro.y;
  gyroZ = g.gyro.z;

  /* ---------- LOG MPU6050 READINGS ---------- */
  Serial.println("\n[MPU6050 Sensor]");
  Serial.print("  Accel (m/s^2): X: "); Serial.print(accX);
  Serial.print(" | Y: "); Serial.print(accY);
  Serial.print(" | Z: "); Serial.println(accZ);

  Serial.print("  Gyro (rad/s):  X: "); Serial.print(gyroX);
  Serial.print(" | Y: "); Serial.print(gyroY);
  Serial.print(" | Z: "); Serial.println(gyroZ);
}

void setMPU6050Sleep(bool sleepState) {
  mpu.enableSleep(sleepState);
}