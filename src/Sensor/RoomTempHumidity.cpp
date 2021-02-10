#include "RoomTempHumidity.h"

namespace RoomTempHumiditySensor
{
    // DHT Sensor
    uint8_t DHTPin = D5;

    // Initialize DHT sensor.
    DHT dht(DHTPin, DHTTYPE);

    void init()
    {
        dht.begin();
        Serial.printf("DHT initialized. Pin:%u\n", DHTPin);
    }

    float getTemperature()
    {
        return dht.readTemperature();
    }

    float getHumidity()
    {
        return dht.readHumidity();
    }
} // namespace RoomTempHumiditySensor