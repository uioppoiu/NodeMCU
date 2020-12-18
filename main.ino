#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "src/Button/Button.h"
#include "src/Led/Led.h"
#include "src/UartInterface/UartMessageInterface.h"
#include "src/UartInterface/UartMessageCallbackManagement.h"
#include "src/UartInterface/UartMessageSender.h"
#include "src/UartInterface/UartMessageReceiver.h"

const char *WIFI_ID = "ANH_2.4G";
const char *WIFI_PASS = "12345678";
const char *URL_BASE = "http://anhands.synology.me/insert.php";

WiFiClient client;
HTTPClient http;

void onRequestGet(uint32_t seqId, const UartMessageInterface::RequestGetData *dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    for (size_t arrIdx = 0; arrIdx < arrSize; arrIdx++)
    {
        const UartMessageInterface::RequestGetData &data = dataArr[arrIdx];
        Serial.print("SeqId:");
        Serial.print(seqId);
        Serial.print(" Type:");
        Serial.print((uint32_t)data.type);
        Serial.print(" Name:");
        Serial.println(data.name);
    }
}

const char *sensorTypeStr[] = {"humidity", "co2", "temperature", "cond", "ph", "watertemp"};

void onResponseGet(uint32_t seqId, const UartMessageInterface::ResponseGetData *dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);

    Serial.print("SeqId:");
    Serial.print(seqId);

    String dbMsg(URL_BASE);

    for (size_t arrIdx = 0; arrIdx < arrSize; arrIdx++)
    {
        const UartMessageInterface::ResponseGetData &data = dataArr[arrIdx];
        Serial.print(" Type:");
        Serial.print((uint32_t)data.type);
        Serial.print(" Name:");
        Serial.print(data.name);
        Serial.print(" Value:");
        Serial.println(data.value);
    }

    int sensorType[6] = {
        0,
    };
    int sensorValue[6] = {
        0,
    };

    for (size_t arrIdx = 0; arrIdx < arrSize; arrIdx++)
    {
        const UartMessageInterface::ResponseGetData &data = dataArr[arrIdx];

        bool noaction = false;
        for (int i = 0; i < 6; i++)
        {
            if(sensorType[i] == 0)
            {
                sensorType[i] = data.type;
                break;
            }

            if (sensorType[i] == data.type)
            {
                noaction = true;
                break;
            }
        }

        if (noaction)
            continue;

        dbMsg += ((arrIdx == 0) ? "?" : "&");
        // dbMsg += sensorTypeStr[arrIdx];
        // dbMsg += "=";
        // dbMsg += String(seqId + arrIdx);

        switch (data.type)
        {
        case UartMessageInterface::DataType::SensorTemperature:
        {
            if (String(data.name) == String("ROOM"))
            {
                dbMsg += "temperature=";
            }
            else
            {
                dbMsg += "watertemp=";
            }
        }
        break;
        case UartMessageInterface::DataType::SensorCO2:
            dbMsg += "co2=";
            break;
        case UartMessageInterface::DataType::SensorConductivity:
            dbMsg += "cond=";
            break;
        case UartMessageInterface::DataType::SensorHumidity:
            dbMsg += "humidity=";
            break;
        case UartMessageInterface::DataType::SensorPH:
            dbMsg += "ph=";
            break;
        }
        dbMsg += String(data.value);
    }

    Serial.print("URL:");
    Serial.println(dbMsg);

    http.begin(client, dbMsg);
    http.setTimeout(10000);
    int ret = http.GET();
    http.end();

    Serial.printf("Ret:%d. Done\n", ret);
}

void onNotification(uint32_t seqId, const UartMessageInterface::NotificationData *dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    onResponseGet(seqId, dataArr, arrSize);
}

void onSubscribe(uint32_t seqId, const UartMessageInterface::SubscribeData *dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    onRequestGet(seqId, dataArr, arrSize);
}

void onUnsubscribe(uint32_t seqId, const UartMessageInterface::UnsubscribeData *dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    onRequestGet(seqId, dataArr, arrSize);
}

void onRequestSet(uint32_t seqId, const UartMessageInterface::RequestSetData *dataArr, size_t arrSize)
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

uint8_t readBuffer[256] = {
    0,
};
size_t readBufferIdx = 0;

// typedef void(*LoopJob)();
// LoopJob loopJobs[5] = {0,};
// // 1. GetBuffer : listen
// // 2. CheckBuffer : checkBuffer
// // 3. ProcessBuffer : process

// bool isMsgReady = false;

// void listen()
// {
//     while (Serial.available() > 0)
//     {
//         readBuffer[readBufferIdx++] = (uint8_t)Serial.read();
//     }

//     if (readBufferIdx == 128)
//     {
//         memset(readBuffer, 0x00, sizeof(readBuffer));
//         readBufferIdx = 0;
//     }
// }

// void checkBuffer()
// {
//     bool isBegin = false;
//     if (readBufferIdx >= 7)
//     {
//         if (memcmp(readBuffer + readBufferIdx - 7, "<BEGIN>", 7) == 0)
//         {
//             isBegin = true;
//         }
//     }

//     if (isBegin)
//     {
//         Serial.println("BEGIN FOUND!!");

//         memset(readBuffer, 0x00, sizeof(readBuffer));
//         readBufferIdx = 0;
//         return;
//     }

//     bool isEnd = false;
//     if (readBufferIdx >= 5)
//     {
//         if (memcmp(readBuffer + readBufferIdx - 5, "<END>", 5) == 0)
//         {
//             isEnd = true;
//         }
//     }

//     if (isEnd)
//     {
//         Serial.println("END FOUND!!");

//         readBufferIdx = readBufferIdx - 5;
//         isMsgReady=true;
//         return;
//     }
// }

// void process()
// {
//     if(isMsgReady)
//     {
//         readBufferIdx = readBufferIdx - 5;
//         UartMessageInterface::UartMessageReceiver rcv(readBuffer, readBufferIdx);
//         rcv.processMessage();

//         memset(readBuffer, 0x00, sizeof(readBuffer));
//         readBufferIdx = 0;

//         isMsgReady = false;
//     }
// }

void setup()
{
    Serial.begin(115200);
    Serial.flush();
    Serial.println("Serial OK...");

    // setupLED();

    // setup3ColorLED();

    // Button_D6.setup();
    // Button_D7.setup();
    // Button_D8.setup();

    Serial.println("Initialize done...");

    // loopJobs[0] = &listen;
    // loopJobs[1] = &checkBuffer;
    // loopJobs[2] = &process;

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_ID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

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

// void sendTestMessage()
// {
//     static uint32_t cnt = 0;
//     if(cnt % 400 == 0)
//     {
//         request();
//         // Serial.println("Request done...");
//     }
//     else if(cnt % 400 == 200)
//     {
//         response();
//         // Serial.println("Response done...");
//     }

//     cnt++;
//     return;
// }

void loop()
{
    // static size_t fnIdx = 0;
    // delay(10);

    // if (loopJobs[fnIdx] != NULL)
    // {
    //     loopJobs[fnIdx]();
    // }
    // fnIdx++;
    // if(fnIdx == 5) fnIdx = 0;

    // sendTestMessage();

    // digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    // delay(1000);                     // wait for a second
    // digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    // delay(1000);                     // wait for a second

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

        if (readBufferIdx == 256)
        {
            memset(readBuffer, 0x00, sizeof(readBuffer));
            readBufferIdx = 0;
        }
    }
}
