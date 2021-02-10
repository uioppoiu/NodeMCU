#include <Arduino.h>
#include "TimeModule.h"

void TimeModule::setClock()
{
    configTime(TZ_Asia_Seoul, 0, "pool.ntp.org", "time.nist.gov");

    Serial.print("Waiting for NTP time sync: ");
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2)
    {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }
    printTime();
}

void TimeModule::printTime()
{
    time_t now = time(nullptr);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    Serial.print("Current time: ");
    Serial.print(asctime(&timeinfo));
}