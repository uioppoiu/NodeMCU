#ifndef __ROOM_TEMP_HUMIDITY_SENSOR_H__
#define __ROOM_TEMP_HUMIDITY_SENSOR_H__

#include <Arduino.h>
#include <DHT.h>

// Uncomment one of the lines below for whatever DHT sensor type you're using!
//#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DHTTYPE DHT11 // DHT 22  (AM2302), AM2321

namespace RoomTempHumiditySensor
{
    void init();
    float getTemperature();
    float getHumidity();
}; // namespace RoomTempHumiditySensor

#endif