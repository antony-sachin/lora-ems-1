#ifndef BME680_SENSOR_H
#define BME680_SENSOR_H

#include <Arduino.h>
#include <Adafruit_BME680.h>

// Global variables to hold actual BME values
extern float bmeTemp;
extern float bmeHum;
extern float bmePres;
extern float bmeGas;

void initBME680();
void readBME680();

#endif