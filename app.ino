#include <ArduinoJson.h>
#include "src/aws_cert.h"
#include "src/TimeModule.h"
#include "src/WifiModule.h"
#include "src/MqttModule.h"
#include "src/Sensor/RoomTempHumidity.h"
#include "src/Sensor/WaterTemperature.h"
#include "src/Sensor/WaterElectricalConductivity.h"

float Room_Temperature = 0;
float Room_Humidity = 0;
float Water_Temperature = 0;
float Water_EC = 0;

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

    // Sensor - DHT (Room temperature/humidity)
    RoomTempHumiditySensor::init();

    // timer1 interrupt (1 sec)
    timer1_attachInterrupt(global_scheduler);
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
    timer1_write(5000000); //1000ms
}

bool globalTimeOneSecFlag = 0;
bool globalTimeFiveSecFlag = 0;
void global_scheduler()
{
    static int countFive = 0;
    timer1_write(5000000); //1000ms
    countFive++;
    if (countFive == 5)
    {
        countFive = 0;
        globalTimeFiveSecFlag = 1;
    }
    globalTimeOneSecFlag = 1;
}

void loop()
{
    defaultLoop();

    if (globalTimeOneSecFlag == 1) // get into every 1sec
    {
        globalTimeOneSecFlag = 0;

        static bool ledState = false;
        digitalWrite(LED_BUILTIN, ledState);
        ledState = !ledState;
    }

    if (globalTimeFiveSecFlag == 1 && globalTimeOneSecFlag == 0) // get into every 5sec
    {
        globalTimeFiveSecFlag = 0;
        
        updateDHTSensorData();

        TimeModule::printTime();
        sendSensorDataTest();
    }
}

void defaultLoop()
{
    MqttModule::loop();
    delay(1);
}

void updateDHTSensorData()
{
    Room_Temperature = RoomTempHumiditySensor::getTemperature();
    Room_Humidity = RoomTempHumiditySensor::getHumidity();
}

void updateDS18B20Data()
{
    Water_Temperature = WaterTemperatureSensor::getTemperature();
}

void updateDFR0300Data()
{
    Water_EC = WaterElectricalConductivity::getEC(Water_Temperature);
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
    doc["water_temperature"] = Water_Temperature;
    doc["water_conductivity"] = Water_EC;
    doc["water_ph"] = 50000 + testValue;
    doc["water_co2"] = 60000 + testValue;

    serializeJson(doc, output);

    MqttModule::pubsubClient.publish("update/sensor", output.c_str());

    Serial.printf("Publish sensor data. testValue:%d\n", testValue);
    Serial.printf("Message : \"%s\"\n", output.c_str());
    
    testValue = (testValue + 1) % 10000;
}
