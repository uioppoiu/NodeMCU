#include "WaterElectricalConductivity.h"

DFRobot_EC WaterElectricalConductivity::ec;

void WaterElectricalConductivity::init()
{
    ec.begin();
    Serial.printf("Water Electrical conductivity initialized\n");
}

float WaterElectricalConductivity::getEC(float waterTemperature)
{
    // TODO analogRead 부분을 변경해야 함 (ADC가 아니라 I2C로 읽음)
    float voltage = analogRead(EC_PIN) / 1024.0 * 5000; // read the voltage
    float ecValue = ec.readEC(voltage, waterTemperature);    // convert voltage to EC with temperature compensation
    Serial.print("^C  EC:");
    Serial.printf("EC:%f ms/cm\n", ecValue);

    return ecValue;
}

void WaterElectricalConductivity::calibration(float waterTemperature, char *cmd)
{
    // Serial Commands:
    // enterec -> enter the calibration mode
    // calec -> calibrate with the standard buffer solution, two buffer solutions(1413us/cm and 12.88ms/cm) will be automaticlly recognized
    // exitec -> save the calibrated parameters and exit from calibration mode

    float voltage = analogRead(EC_PIN) / 1024.0 * 5000; // read the voltage
    ec.calibration(voltage, waterTemperature, cmd);     // calibration process by Serail CMD
}
