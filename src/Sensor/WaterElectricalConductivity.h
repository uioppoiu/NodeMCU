#ifndef __WATER_ELECTRICAL_CONDUCTIVITY_H__
#define __WATER_ELECTRICAL_CONDUCTIVITY_H__

#include <Arduino.h>
#include <DFRobot_EC.h>

#define EC_PIN A0

class WaterElectricalConductivity
{
public:
    static DFRobot_EC ec;

    static void init();
    static float getEC(float waterTemperature);
    static void calibration(float waterTemperature, char *cmd);
};

#endif