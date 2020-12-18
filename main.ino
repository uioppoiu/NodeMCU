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
    static int v = 0;
    v++;
    UartMessageInterface::UartMessageSender rspGet(UartMessageInterface::MsgId::ResponseGet);
    rspGet.setSeqId(seqId++);
    for (size_t arrIdx = 0; arrIdx < arrSize; arrIdx++)
    {
        const UartMessageInterface::RequestGetData &msg = dataArr[arrIdx];
        switch (msg.type)
        {
        case UartMessageInterface::DataType::SensorTemperature:
        {
            if (memcmp(msg.name, "WATER", sizeof("WATER")) == 0)
            {
                rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorTemperature, "WATER", sizeof("WATER"), 1000 + v);
            }
            else
            {
                rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorTemperature, "ROOM", sizeof("ROOM"), 2000 + v);
            }
        }
        break;
        case UartMessageInterface::DataType::SensorCO2:
            rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorCO2, "CO2", sizeof("CO2"), 3000 + v);
            break;
        case UartMessageInterface::DataType::SensorHumidity:
            rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorHumidity, "HUMID", sizeof("HUMID"), 4000 + v);
            break;
        case UartMessageInterface::DataType::SensorConductivity:
            rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorConductivity, "CONDUC", sizeof("CONDUC"), 5000 + v);
            break;
        case UartMessageInterface::DataType::SensorPH:
            rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorPH, "PH", sizeof("PH"), 6000 + v);
            break;
        default:
            break;
        }
    }

    rspGet.sendMessage();
}

void onResponseGet(uint32_t seqId, const UartMessageInterface::ResponseGetData *dataArr, size_t arrSize)
{
    enum
    {
        IDX_HUMIDITY = 0,
        IDX_CO2 = 1,
        IDX_TEMP_ROOM = 2,
        IDX_CONDUCTIVITY = 3,
        IDX_PH = 4,
        IDX_TEMP_WATER = 5,
    };
    const size_t NumOfSensor = 6;
    const char *sensorTypeStr[NumOfSensor] = {"humidity", "co2", "temperature", "cond", "ph", "watertemp"};
    uint32_t sensorVal[NumOfSensor] = {
        0,
    };

    for (size_t arrIdx = 0; arrIdx < arrSize; arrIdx++)
    {
        const UartMessageInterface::ResponseGetData &msg = dataArr[arrIdx];

        switch (msg.type)
        {
        case UartMessageInterface::DataType::SensorTemperature:
        {
            if (memcmp(msg.name, "WATER", sizeof("WATER")) == 0)
            {
                sensorVal[IDX_TEMP_WATER] = msg.value;
            }
            else
            {
                sensorVal[IDX_TEMP_ROOM] = msg.value;
            }
        }
        break;
        case UartMessageInterface::DataType::SensorCO2:
            sensorVal[IDX_CO2] = msg.value;
            break;
        case UartMessageInterface::DataType::SensorHumidity:
            sensorVal[IDX_HUMIDITY] = msg.value;
            break;
        case UartMessageInterface::DataType::SensorConductivity:
            sensorVal[IDX_CONDUCTIVITY] = msg.value;
            break;
        case UartMessageInterface::DataType::SensorPH:
            sensorVal[IDX_PH] = msg.value;
            break;
        default:
            break;
        }
    }

    String dbMsg(URL_BASE);
    for (size_t dbIdx = 0; dbIdx < NumOfSensor; dbIdx++)
    {
        dbMsg += ((dbIdx == 0) ? "?" : "&");
        dbMsg += sensorTypeStr[dbIdx];
        dbMsg += "=";
        dbMsg += String(sensorVal[dbIdx]);
    }

    Serial.print("URL:");
    Serial.println(dbMsg);

    http.begin(client, dbMsg);
    http.setTimeout(10000);
    int ret = http.GET();
    http.end();

    // Serial.printf("Ret:%d. Done\n", ret);
}

void onNotification(uint32_t seqId, const UartMessageInterface::NotificationData *dataArr, size_t arrSize)
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

uint8_t readBuffer[256] = {0,};
size_t readBufferIdx = 0;

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

    // WiFi.mode(WIFI_STA);
    // WiFi.begin(WIFI_ID, WIFI_PASS);
    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     delay(500);
    //     Serial.print(".");
    // }
    // Serial.print("IP address: ");
    // Serial.println(WiFi.localIP());

    // Example
    // Callback 등록
    UartMessageInterface::UartMessageCallbackManagement::registerRequestGetCallBack(onRequestGet);
    UartMessageInterface::UartMessageCallbackManagement::registerResponseGetCallBack(onResponseGet);
    UartMessageInterface::UartMessageCallbackManagement::registerNotificationCallBack(onNotification);
}

void sendTestMessage()
{
    static int v = 0;
    UartMessageInterface::UartMessageSender rspGet(UartMessageInterface::MsgId::ResponseGet);
    rspGet.setSeqId(v++);
    rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorTemperature, "WATER", sizeof("WATER"), 1000 + v);
    rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorTemperature, "ROOM", sizeof("ROOM"), 2000 + v);
    rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorCO2, "CO2", sizeof("CO2"), 3000 + v);
    rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorHumidity, "HUMID", sizeof("HUMID"), 4000 + v);
    rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorConductivity, "CONDUC", sizeof("CONDUC"), 5000 + v);
    rspGet.appendResponseGetData(UartMessageInterface::DataType::SensorPH, "PH", sizeof("PH"), 6000 + v);
    rspGet.sendMessage();
}

void loop()
{
    // static bool LED_State = false;
    // digitalWrite(LED_BUILTIN, LED_State);
    // LED_State = !LED_State;
    // delay(5000);
    // sendTestMessage();

    if(Serial.available() > 0) serialEvent();
}


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
            Serial.println("Begin FOUND!!");

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
            Serial.println("End FOUND!!");

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