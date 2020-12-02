#include <Arduino.h>
#include "src/Button/Button.h"
#include "src/Led/Led.h"
#include "src/UartInterface/UartMessageInterface.h"
#include "src/UartInterface/UartMessageCallbackManagement.h"
#include "src/UartInterface/UartMessageSender.h"
#include "src/UartInterface/UartMessageReceiver.h"

void onRequestGet(uint32_t seqId, const UartMessageInterface::RequestGetData* dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    for(size_t arrIdx = 0 ; arrIdx < arrSize ; arrIdx++)
    {
        const UartMessageInterface::RequestGetData& data = dataArr[arrIdx];
        Serial.print("SeqId:");
        Serial.print(seqId);
        Serial.print(" Type:");
        Serial.print((uint32_t)data.type);
        Serial.print(" Name:");
        Serial.println(data.name);
    }
}

void onResponseGet(uint32_t seqId, const UartMessageInterface::ResponseGetData* dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    for(size_t arrIdx = 0 ; arrIdx < arrSize ; arrIdx++)
    {
        const UartMessageInterface::ResponseGetData& data = dataArr[arrIdx];
        Serial.print("SeqId:");
        Serial.print(seqId);
        Serial.print(" Type:");
        Serial.print((uint32_t)data.type);
        Serial.print(" Name:");
        Serial.print(data.name);
        Serial.print(" Value:");
        Serial.println(data.value);
    }
}

void onNotification(uint32_t seqId, const UartMessageInterface::NotificationData* dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    onNotification(seqId, dataArr, arrSize);
}

void onSubscribe(uint32_t seqId, const UartMessageInterface::SubscribeData* dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    onRequestGet(seqId, dataArr, arrSize);
}

void onUnsubscribe(uint32_t seqId, const UartMessageInterface::UnsubscribeData* dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    onRequestGet(seqId, dataArr, arrSize);
}

void onRequestSet(uint32_t seqId, const UartMessageInterface::RequestSetData* dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    onResponseGet(seqId, dataArr, arrSize);
}

void onAcknowledge(uint32_t seqId, unsigned char msgId)
{
    Serial.println(__FUNCTION__);
    Serial.print("SeqId:");
    Serial.print(seqId);
    Serial.print("MsgId:");
    Serial.println((uint32_t)msgId);
}


void setup()
{
    Serial.begin(9600);
    Serial.flush();
    Serial.println("Serial OK...");

    // setupLED();

    // setup3ColorLED();

    // Button_D6.setup();
    // Button_D7.setup();
    // Button_D8.setup();

    Serial.println("Initialize done...");

    // Example
    // Callback 등록
    UartMessageInterface::UartMessageCallbackManagement::registerRequestGetCallBack(onRequestGet);
    UartMessageInterface::UartMessageCallbackManagement::registerResponseGetCallBack(onResponseGet);
    UartMessageInterface::UartMessageCallbackManagement::registerRequestSetCallBack(onRequestSet);
    UartMessageInterface::UartMessageCallbackManagement::registerNotificationCallBack(onNotification);
    UartMessageInterface::UartMessageCallbackManagement::registerSubscribeCallBack(onSubscribe);
    UartMessageInterface::UartMessageCallbackManagement::registerUnsubscribeCallBack(onUnsubscribe);
    UartMessageInterface::UartMessageCallbackManagement::registerAcknowledgeCallBack(onAcknowledge);
}

char readBuffer[1024] = {0,};
size_t readBufferIdx = 0;

uint32_t seqId = 0;

void request()
{
    // Get Request Message 전달
    UartMessageInterface::UartMessageSender reqGet(UartMessageInterface::MsgId::RequestGet);
    reqGet.setSeqId(seqId++);
    reqGet.appendRequestGetData(UartMessageInterface::DataType::SensorTemperature, "ROOM", sizeof("ROOM"));
    reqGet.appendRequestGetData(UartMessageInterface::DataType::SensorCO2, "ROOM", sizeof("ROOM"));
    reqGet.appendRequestGetData(UartMessageInterface::DataType::SensorTemperature, "WATER", sizeof("WATER"));
    reqGet.sendMessage();
}

void response()
{
    // Get Response Message 전달
    UartMessageInterface::UartMessageSender rspGet(UartMessageInterface::MsgId::ResponseGet);
    rspGet.setSeqId(seqId++);
    rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorTemperature, "ROOM", sizeof("ROOM"), 111);
    rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorCO2, "ROOM", sizeof("ROOM"), 2222);
    rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorTemperature, "WATER", sizeof("WATER"), 33333);
    rspGet.sendMessage();
}

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(1000);                     // wait for a second
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(1000);                     // wait for a second

    request();
    delay(2000);
    response();
}

uint8_t readBuffer[128] = {0,};
size_t readBufferIdx = 0;
void serialEvent()
{
    while (Serial.available() > 0)
    {
        readBuffer[readBufferIdx++] = (uint8_t)Serial.read();

        bool isBegin = false;
        if (readBufferIdx >= 7)
        {
            if (memcmp(readBuffer + readBufferIdx - 7, "<BEGIN>", 7) == 0)
            {
                isBegin = true;
            }
        }

        if (isBegin)
        {
            Serial.println("BEGIN FOUND!!");

            memset(readBuffer, 0x00, sizeof(readBuffer));
            readBufferIdx = 0;
            continue;
        }

        bool isEnd = false;
        if (readBufferIdx >= 5)
        {
            if (memcmp(readBuffer + readBufferIdx - 5, "<END>", 5) == 0)
            {
                isEnd = true;
            }
        }

        if (isEnd)
        {
            Serial.println("END FOUND!!");

            readBufferIdx = readBufferIdx - 5;
            UartMessageInterface::UartMessageReceiver rcv(readBuffer, readBufferIdx);
            rcv.processMessage();

            memset(readBuffer, 0x00, sizeof(readBuffer));
            readBufferIdx = 0;

            continue;
        }

        if(readBufferIdx == 256)
        {
            memset(readBuffer, 0x00, sizeof(readBuffer));
            readBufferIdx = 0;
        }
    }
}