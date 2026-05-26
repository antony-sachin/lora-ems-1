#include "cloud.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>

// 🔗 Firebase base URL
const char* baseURL = "https://lora-ems-dashboard-default-rtdb.asia-southeast1.firebasedatabase.app/logs/";

const char* ntpServer = "pool.ntp.org";

// ---------------- INIT ----------------
void cloud_init(const char* ssid, const char* password)
{
    WiFi.begin(ssid, password);

    Serial.print("Connecting WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");

    // NTP time sync
    configTime(0, 0, ntpServer);

    Serial.print("Syncing time");
    time_t now = time(NULL);
    while (now < 100000)
    {
        delay(500);
        Serial.print(".");
        now = time(NULL);
    }
    Serial.println("\nTime synced!");
}

// ---------------- DATE ----------------
String getDatePath()
{
    time_t now = time(NULL);
    struct tm* t = gmtime(&now);

    char buf[20];
    sprintf(buf, "%04d-%02d-%02d",
        t->tm_year + 1900,
        t->tm_mon + 1,
        t->tm_mday);

    return String(buf);
}

// ---------------- SEND ----------------
void cloud_send(float temp, float hum, float pres, float gas, float rssi,
                float ax, float ay, float az, float alt)
{
    if (WiFi.status() != WL_CONNECTED) return;

    String url = String(baseURL) + getDatePath() + ".json";

    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    time_t now = time(NULL);

    String payload = "{";
    payload += "\"temp\":" + String(temp) + ",";
    payload += "\"hum\":" + String(hum) + ",";
    payload += "\"pres\":" + String(pres) + ",";
    payload += "\"gas\":" + String(gas) + ",";
    payload += "\"rssi\":" + String(rssi) + ",";
    payload += "\"ax\":" + String(ax) + ",";
    payload += "\"ay\":" + String(ay) + ",";
    payload += "\"az\":" + String(az) + ",";
    payload += "\"alt\":" + String(alt) + ",";
    payload += "\"time\":" + String(now);
    payload += "}";

    int code = http.POST(payload);

    Serial.print("Cloud POST: ");
    Serial.println(code);

    http.end();
}