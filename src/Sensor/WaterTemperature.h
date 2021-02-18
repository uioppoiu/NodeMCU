#ifndef __WATER_TEMPERATURE_SENSOR_H__
#define __WATER_TEMPERATURE_SENSOR_H__

#include <Arduino.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

class WaterTemperatureSensor
{
public:
    static OneWire oneWire;
    static DallasTemperature sensors;
    static DeviceAddress insideThermometer;

    static void init();
    static void printAddress(DeviceAddress deviceAddress);
    static float getTemperature();
};

#endif