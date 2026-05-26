#include "bme680_sensor.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define SEALEVELPRESSURE_HPA (1013.25)
#define BME680_I2C_ADDRESS 0x77

Adafruit_BME680 bme; 

// Define the global variables
float bmeTemp = 0;
float bmeHum = 0;
float bmePres = 0;
float bmeGas = 0;

void initBME680() {

if (!bme.begin(BME680_I2C_ADDRESS, &Wire) && !bme.begin(0x76, &Wire)) 
  {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);
  
  Serial.println("BME680 Initialized!");
}

void readBME680() {
  if (!bme.performReading()) {
    Serial.println("Failed to perform BME680 reading :(");
    return;
  }
  
  // Update global variables with actual values
  bmeTemp = bme.temperature;
  bmeHum = bme.humidity;
  bmePres = bme.pressure / 100.0; // Pa → hPa
  bmeGas = bme.gas_resistance / 1000.0; // Ω → kΩ

  /* ---------- LOG BME680 READINGS ---------- */
  Serial.println("\n[BME680 Sensor]");
  Serial.print("  Temp:     "); Serial.print(bmeTemp); Serial.println(" °C");
  Serial.print("  Humidity: "); Serial.print(bmeHum);  Serial.println(" %");
  Serial.print("  Pressure: "); Serial.print(bmePres); Serial.println(" hPa");
  Serial.print("  Gas/VOC:  "); Serial.print(bmeGas);  Serial.println(" kΩ");

}