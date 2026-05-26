#include "oled.h"
#include <Wire.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

const char* estimateAQI(float gas_kohm)
{
    if (gas_kohm > 50) return "GOOD";
    if (gas_kohm > 20) return "MOD";
    if (gas_kohm > 10) return "POOR";
    return "BAD";
}

void oled_init()
{
    u8g2.begin();
    u8g2.setFont(u8g2_font_6x12_tr);

    u8g2.clearBuffer();
    u8g2.drawStr(25,20,"LoRa RX Ready");
    u8g2.sendBuffer();
}

void oled_update(float temp, float hum, float pres, float gas, float rssi)
{
    char buf[40];

    const char* aqi = estimateAQI(gas);

    u8g2.clearBuffer();

    sprintf(buf,"Temp: %.2f C", temp);
    u8g2.drawStr(0,12,buf);

    sprintf(buf,"Hum : %.2f %%", hum);
    u8g2.drawStr(0,24,buf);

    sprintf(buf,"Pres: %.1f hPa", pres);
    u8g2.drawStr(0,36,buf);

    sprintf(buf,"VOC: %.1fkOhm AQI:%s", gas, aqi);
    u8g2.drawStr(0,48,buf);

    sprintf(buf,"RSSI: %.0f dBm", rssi);
    u8g2.drawStr(0,60,buf);

    u8g2.sendBuffer();
}