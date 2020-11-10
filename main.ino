#include <Arduino.h>
#include "src/Button/Button.h"
#include "src/Led/Led.h"
#include "src/UartInterface/UartMessageInterface.h"
#include "src/UartInterface/UartMessageCallbackManagement.h"
#include "src/UartInterface/UartMessageSender.h"
#include "src/UartInterface/UartMessageReceiver.h"

void onRequestTemp1(UartMessageInterface::eDataType type, const std::string &name)
{
    Serial.println("onRequestTemp1");
    Serial.println(type);
}

void onRequestTemp2(UartMessageInterface::eDataType type, const std::string &name)
{
    Serial.println("onRequestTemp2");
    Serial.println(type);
}

void onResponseTemp1(UartMessageInterface::eDataType type, const std::string &name, const UartMessageInterface::Value &value)
{
    Serial.println("onResponseTemp1");
    Serial.println(type);
    // cout << name << endl;
    // cout << value.type << endl;
    // cout << value.value.val_double << endl;
}

void onResponseTemp2(UartMessageInterface::eDataType type, const std::string &name, const UartMessageInterface::Value &value)
{
    Serial.println("onResponseTemp2");
    Serial.println(type);
    // cout << name << endl;
    // cout << value.type << endl;
    // cout << value.value.val_int << endl;
}


void setup()
{
    Serial.begin(9600);
    Serial.println("Serial OK...");

    setupLED();

    setup3ColorLED();

    Button_D6.setup();
    Button_D7.setup();
    Button_D8.setup();

    Serial.println("Initialize done...");
    
    // Example
    // Callback 등록 (Get Request)
    UartMessageInterface::UartMessageCallbackManagement::registerRequestGetCallBack(UartMessageInterface::SensorTemperature, "ROOM", onRequestTemp1);
    UartMessageInterface::UartMessageCallbackManagement::registerRequestGetCallBack(UartMessageInterface::SensorTemperature, "WATER", onRequestTemp1);

    // Callback 등록 (Get Response)
    UartMessageInterface::UartMessageCallbackManagement::registerResponseGetCallBack(UartMessageInterface::SensorTemperature, "ROOM", onResponseTemp2);
    UartMessageInterface::UartMessageCallbackManagement::registerResponseGetCallBack(UartMessageInterface::SensorTemperature, "WATER", onResponseTemp2);


    // Get Request Message 전달
    UartMessageInterface::UartMessageSender reqTemp(UartMessageInterface::Request, UartMessageInterface::Get);
    reqTemp.appendRequest(UartMessageInterface::SensorTemperature, "ROOM");
    reqTemp.appendRequest(UartMessageInterface::SensorTemperature, "WATER");
    string msgReq = reqTemp.sendMessage();
    Serial.print(String(msgReq.c_str()));

    // Get Request Message 처리
    UartMessageInterface::UartMessageReceiver rcvReq(msgReq);
    rcvReq.processMessage();


    // Get Response Message 전달
    UartMessageInterface::UartMessageSender rspTemp(UartMessageInterface::Response, UartMessageInterface::Get);
    rspTemp.appendResponse(UartMessageInterface::SensorTemperature, "ROOM", 25.5, UartMessageInterface::Double);
    rspTemp.appendResponse(UartMessageInterface::SensorTemperature, "WATER", 19.0, UartMessageInterface::Integer);
    string msgRsp = rspTemp.sendMessage();
    Serial.print(String(msgRsp.c_str()));

    // Get Responses Message 처리
    UartMessageInterface::UartMessageReceiver rcvRsp(msgReq);
    rcvRsp.processMessage();
}

void loop()
{
    // static int color = 0;
    // color += 1;
    // color = color % 8;
    // set3LedColor(color);

    // const int loopDelaySec = 1000;
    // delay(loopDelaySec);
}
