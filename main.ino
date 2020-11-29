#include <ArduinoJson.h>
#include <Arduino.h>
#include "src/Button/Button.h"
#include "src/Led/Led.h"
#include "src/UartInterface/UartMessageInterface.h"
#include "src/UartInterface/UartMessageCallbackManagement.h"
#include "src/UartInterface/UartMessageSender.h"
#include "src/UartInterface/UartMessageReceiver.h"

void onRequest(UartMessageInterface::eDataType type, const String &name)
{
    Serial.println("onRequest");
    Serial.println(enum2Str(type));
    Serial.println(name);
}

void onResponse(UartMessageInterface::eDataType type, const String &name, const UartMessageInterface::Value &value)
{
    Serial.println("onResponse");
    Serial.println(enum2Str(type));
    Serial.println(name);
    Serial.println(value.type);

    if (value.type == UartMessageInterface::Float)
    {
        Serial.println(value.value.val_float);
    }
    else
    {
        Serial.println(value.value.val_int);
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Serial OK...");

    setupLED();

    setup3ColorLED();

    Button_D6.setup();
    Button_D7.setup();
    Button_D8.setup();

    Serial.println("Initialize done...");

    // Example
    // Callback 등록 (Get Request)
    UartMessageInterface::UartMessageCallbackManagement::registerRequestGetCallBack(onRequest);

    // Callback 등록 (Get Response)
    UartMessageInterface::UartMessageCallbackManagement::registerResponseGetCallBack(onResponse);
}

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(2000);                     // wait for a second
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(2000);                     // wait for a second

    {
        // Get Request Message 전달
        UartMessageInterface::UartMessageSender reqTemp(UartMessageInterface::Request, UartMessageInterface::Get);
        reqTemp.appendRequest(UartMessageInterface::SensorTemperature, "ROOM");
        reqTemp.appendRequest(UartMessageInterface::SensorTemperature, "WATER");
        reqTemp.appendRequest(UartMessageInterface::SensorCO2, "ROOM");
        reqTemp.sendMessage();
        // Serial.println(String(msgReq.c_str()));

        // // Get Request Message 처리
        // UartMessageInterface::UartMessageReceiver rcvReq(msgReq);
        // rcvReq.processMessage();
    }
    
    // if (Serial.available())
    // {
    //     String str = Serial.readString();
    //     Serial.println(str);

    //     // Get Response Message 처리
    //     UartMessageInterface::UartMessageReceiver rcvRsp(str);
    //     rcvRsp.processMessage();
    // }
}

