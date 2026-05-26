#include <RadioLib.h>
#include "oled.h"
#include "cloud.h"

/* ===================== PIN CONFIGURATION ===================== */
#define LORA_NSS   25
#define LORA_DIO0  26
#define LORA_RST   27

Module loraMod(LORA_NSS, LORA_DIO0, LORA_RST);
SX1278 radio(&loraMod);

const char* ssid = "C BLOCK";
const char* password = "rit#1432";

/* ===================== PAYLOAD STRUCT ===================== */
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

void setup() {
  Serial.begin(115200);
  delay(500);
  oled_init();
  cloud_init(ssid, password);
  // Initialize LoRa
  int state = radio.begin(433.0, 125.0, 10, 5, 0x12, 17, 8);
  if (state != RADIOLIB_ERR_NONE) {
    Serial.println("{\"error\":\"LoRa RX init failed\"}");
    while (true);
  }
}

void loop() {
  int state = radio.receive((uint8_t*)&pkt, sizeof(pkt));

  if (state == RADIOLIB_ERR_NONE) {
    // Decode data
    float temp  = pkt.temp / 100.0;
    float hum   = pkt.hum / 100.0;
    float pres  = pkt.pres / 10.0;
    float gas   = pkt.gas / 10.0;
    float accX  = pkt.accX / 100.0;
    float accY  = pkt.accY / 100.0;
    float accZ  = pkt.accZ / 100.0;
    float gyroX = pkt.gyroX / 100.0;
    float gyroY = pkt.gyroY / 100.0;
    float gyroZ = pkt.gyroZ / 100.0;

    float rssi = radio.getRSSI();

    oled_update(temp, hum, pres, gas, rssi);
    //float alt = 44330.0 * (1.0 - pow(pres / 1013.25, 0.1903));
    // Use your measured local reference pressure as zero altitude
    const float BASE_PRESSURE = 1008.00;   // hPa (average of 1006.80 to 1006.90)

    // Calculate relative altitude in meters
    // At pres = 1006.85 hPa -> alt ≈ 0 m
    // As pressure decreases -> altitude increases
    float alt = 44330.0 * (1.0 - pow(pres / BASE_PRESSURE, 0.1903));
    Serial.println(alt);

    // Optional: suppress tiny fluctuations around zero
    if (fabs(alt) < 0.05) 
    {
      alt = 0.0;
    }

    cloud_send(temp, hum, pres, gas, rssi,
              accX, accY, accZ, alt);

    /* Output strictly valid JSON on one line
    Serial.print("{\"cnt\":"); Serial.print(pkt.counter);
    Serial.print(",\"t\":"); Serial.print(temp);
    Serial.print(",\"h\":"); Serial.print(hum);
    Serial.print(",\"p\":"); Serial.print(pres);
    Serial.print(",\"g\":"); Serial.print(gas);
    Serial.print(",\"ax\":"); Serial.print(accX);
    Serial.print(",\"ay\":"); Serial.print(accY);
    Serial.print(",\"az\":"); Serial.print(accZ);
    Serial.print(",\"gx\":"); Serial.print(gyroX);
    Serial.print(",\"gy\":"); Serial.print(gyroY);
    Serial.print(",\"gz\":"); Serial.print(gyroZ);
    Serial.print(",\"rssi\":"); Serial.print(rssi);
    Serial.println("}");*/
  }
}