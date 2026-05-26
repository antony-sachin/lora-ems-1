#ifndef CLOUD_H
#define CLOUD_H

#include <Arduino.h>

// Init WiFi + NTP
void cloud_init(const char* ssid, const char* password);

// Send data to Firebase
void cloud_send(float temp, float hum, float pres, float gas, float rssi,
                float ax, float ay, float az, float alt);

#endif