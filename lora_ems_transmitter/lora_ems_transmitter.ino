#include <Wire.h>
#include <RadioLib.h>
#include "bme680_sensor.h"
#include "mpu6050_sensor.h"

/* LoRa pins */
#define LORA_NSS   25
#define LORA_DIO0  26
#define LORA_RST   27

Module loraMod(LORA_NSS, LORA_DIO0, LORA_RST);
SX1278 radio(&loraMod);

/* Telemetry frame */
typedef struct __attribute__((packed)) {

  uint16_t counter;

  int16_t temp;
  uint16_t hum;
  uint16_t pres;
  uint16_t gas;

  int16_t accX;
  int16_t accY;
  int16_t accZ;

  int16_t gyroX;
  int16_t gyroY;
  int16_t gyroZ;

} TelemetryPacket;

TelemetryPacket pkt;

uint16_t packetCounter = 0;

void setup() {

  Serial.begin(115200);
  delay(500);

  Wire.begin(21, 22);

  delay(1000);
  initBME680();
  initMPU6050();

 /*
   - freq: 433.5 MHz (Shifted slightly off 433.0 to avoid local interference)
   - bw:   125.0 kHz
   - sf:   10  (Better penetration through walls/trees, Time-on-Air ~144ms)
   - cr:   7  (4/7 error correction for corrupted packets)
   - sync: 0x12
   - pwr:  20 dBm (ABSOLUTE MAXIMUM POWER)
   - pre:  8
  */
  int state = radio.begin(433.0, 125.0, 10, 5, 0x12, 17, 8);

  if (state != RADIOLIB_ERR_NONE) {
    Serial.println("LoRa init failed");
    while (true);
  }


}

void loop() {

  unsigned long start = millis();

  readBME680();
  readMPU6050();

  pkt.counter = packetCounter++;

  //float to uint16_t scaling 
  pkt.temp = bmeTemp * 100;
  pkt.hum  = bmeHum  * 100;
  pkt.pres = bmePres * 10;
  pkt.gas  = bmeGas  * 10;

  pkt.accX = accX * 100;
  pkt.accY = accY * 100;
  pkt.accZ = accZ * 100;

  pkt.gyroX = gyroX * 100;
  pkt.gyroY = gyroY * 100;
  pkt.gyroZ = gyroZ * 100;

  int state = radio.transmit((uint8_t*)&pkt, sizeof(pkt));

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("TX OK");
  }

  //ensure packet doesnot sent before 1second 
  //while (millis() - start < 1000);
}