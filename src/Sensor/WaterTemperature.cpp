#include "WaterTemperature.h"

OneWire WaterTemperatureSensor::oneWire(ONE_WIRE_BUS);
DallasTemperature WaterTemperatureSensor::sensors(&oneWire);
DeviceAddress WaterTemperatureSensor::insideThermometer;

void WaterTemperatureSensor::init()
{
    //1-wire 버스 초기화
    sensors.begin();

    //발견한 디바이스 갯수
    Serial.print("Found ");
    Serial.print(sensors.getDeviceCount(), DEC);
    Serial.println(" devices.");

    // parasite power 모드일 때에는  2핀(GND와 DQ 핀)만 연결하면 됨.
    Serial.print("Parasite power is: ");
    if (sensors.isParasitePowerMode())
        Serial.println("ON");
    else
        Serial.println("OFF");

    //버스에서 첫번째 장치의 주소를 가져온다.
    if (!sensors.getAddress(insideThermometer, 0))
        Serial.println("Unable to find address for Device 0");

    //버스에서 발견한 첫번째 장치의 주소 출력
    Serial.print("Device 0 Address: ");
    printAddress(insideThermometer);
    Serial.println();

    //데이터시트에서 확인결과 9~12비트까지 설정 가능
    sensors.setResolution(insideThermometer, 10);

    Serial.print("Device 0 Resolution: ");
    Serial.print(sensors.getResolution(insideThermometer), DEC);
    Serial.println();

    Serial.printf("Water Temperature sensor initialized. Pin:%u\n", ONE_WIRE_BUS);
}

void WaterTemperatureSensor::printAddress(DeviceAddress deviceAddress)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (deviceAddress[i] < 16)
            Serial.print("0");
        Serial.print(deviceAddress[i], HEX);
    }
}

float WaterTemperatureSensor::getTemperature()
{
    Serial.print("Requesting temperatures...");
    
    //sensors.requestTemperatures();   //연결되어 있는 전체 센서의 온도 값을 읽어옴
    sensors.requestTemperaturesByIndex(0); //첫번째 센서의 온도값 읽어옴

    float tempC = sensors.getTempC(insideThermometer); 
    Serial.printf("Temp : %f\n", tempC);

    return tempC;
}