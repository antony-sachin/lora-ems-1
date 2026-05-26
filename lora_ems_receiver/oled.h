#ifndef OLED_H
#define OLED_H

#include <Arduino.h>

void oled_init();
void oled_update(float temp, float hum, float pres, float gas, float rssi);

#endif