#include <ArduinoJson.h>
#include "src/aws_cert.h"
#include "src/TimeModule.h"
#include "src/WifiModule.h"
#include "src/MqttModule.h"
#include "src/Sensor/RoomTempHumidity.h"

float Room_Temperature = 0;
float Room_Humidity = 0;

void setup()
{
    // Serial
    Serial.begin(115200);
    Serial.println("\r\nSerial Initialized");

    // LED
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.printf("LED Initialized. Pin:%d\n", LED_BUILTIN);

    // WiFi
    WifiModule::init(cert_str, key_str, ca_str);

    // MQTT
    MqttModule::init(AWS_URL, AWS_PORT, WifiModule::wifiClient);
    MqttModule::setCallback(subscriptionCallback);

    // Time sync
    TimeModule::setClock();

    // Sensor
    RoomTempHumiditySensor::init();
}

int loopCount_T1 = 0;
int loopCount_T2 = 0;
int loopCount_T3 = 0;
void loop()
{
    defaultLoop();

    if (loopCount_T1 == 0)
    {
        static bool ledState = false;
        digitalWrite(LED_BUILTIN, ledState);
        ledState = !ledState;
    }

    switch (loopCount_T2)
    {
    case 0:
        updateDHTSensorData();
        break;
    case 400:
        TimeModule::printTime();
        sendSensorDataTest();
        break;
    }
}

void defaultLoop()
{
    MqttModule::loop();

    loopCount_T1 = (loopCount_T1 + 1) % 50;
    loopCount_T2 = (loopCount_T2 + 1) % 500;
    loopCount_T3 = (loopCount_T3 + 1) % 20;
    delay(10);
}

void updateDHTSensorData()
{
    Room_Temperature = RoomTempHumiditySensor::getTemperature();
    Room_Humidity = RoomTempHumiditySensor::getHumidity();
}

void subscriptionCallback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();

    StaticJsonDocument<128> doc;
    deserializeJson(doc, payload, length);

    Serial.printf("Control3 Value:%d\n", (int)doc["control3"]);
    Serial.printf("Control2 Value:%d\n", (int)doc["control2"]);
    Serial.printf("Control1 Value:%d\n", (int)doc["control1"]);
    Serial.printf("Control0 Value:%d\n", (int)doc["control0"]);
}

void sendSensorDataTest()
{
    static int testValue = 0;

    String output;
    StaticJsonDocument<256> doc;

    doc["device_name"] = "NodeMCU_001";
    doc["room_temperature"] = Room_Temperature;
    doc["room_humidity"] = Room_Humidity;
    doc["water_temperature"] = 30000 + testValue;
    doc["water_conductivity"] = 40000 + testValue;
    doc["water_ph"] = 50000 + testValue;
    doc["water_co2"] = 60000 + testValue;

    serializeJson(doc, output);

    MqttModule::pubsubClient.publish("update/sensor", output.c_str());

    Serial.printf("Publish sensor data. testValue:%d\n", testValue);
    testValue = (testValue + 1) % 10000;
}
