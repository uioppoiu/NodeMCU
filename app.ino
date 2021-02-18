#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include <TZ.h>
#include "src/aws_cert.h"
#include "src/I2CInterface/I2CInterface.h"
#include "src/MessageInterface/MessageInterface.h"

const char *WIFI_ID = "A1504_2.4Ghz";
const char *WIFI_PASS = "hongsik102*";

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

    MessageInterface::MessageSender reqSet(MessageInterface::MsgId::RequestSet);
    static int v = 0;
    reqSet.setSeqId(v++);
    reqSet.appendRequestSetData(MessageInterface::DataType::Control0, doc["control0"]);
    reqSet.appendRequestSetData(MessageInterface::DataType::Control3, doc["control1"]);
    reqSet.appendRequestSetData(MessageInterface::DataType::Control1, doc["control2"]);
    reqSet.appendRequestSetData(MessageInterface::DataType::Control2, doc["control3"]);
    reqSet.sendMessage();

    I2CInterface::I2C_Master::writeSlaveBuffer();
}

X509List ca(ca_str);
X509List cert(cert_str);
PrivateKey key(key_str);
WiFiClientSecure wifiClient;
PubSubClient pubsubClient(AWS_URL, AWS_PORT, subscriptionCallback, wifiClient);

void setup()
{
    // Serial
    Serial.begin(115200);
    Serial.println("\r\nSerial Initialized");

    // LED
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.printf("LED Initialized. Pin:%d\n", LED_BUILTIN);

    // I2C : Master
    I2CInterface::I2C_Master::init();

    // Callback
    MessageInterface::CallBack::registerNotificationCallBack(onNotification);

    // WiFi
    Serial.print("WiFi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_ID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.print("Initialized. IP:");
    Serial.println(WiFi.localIP());

    // Certificate
    wifiClient.setTrustAnchors(&ca);
    wifiClient.setClientRSACert(&cert, &key);

    //timer1 interrupt 1 sec
    timer1_attachInterrupt(global_scheduler);
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
    timer1_write(5000000); //1000ms

    setClock();
}

void setClock()
{
    configTime(TZ_Asia_Seoul, 0, "pool.ntp.org", "time.nist.gov");

    Serial.print("Waiting for NTP time sync: ");
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2)
    {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }
    printTime();
}

int loopCount_T1 = 0;
int loopCount_T2 = 0;
int loopCount_T3 = 0;
bool globalTimeOneSecFlag = 0;
bool globalTimeFiveSecFlag = 0;
bool ledState = 0;
void loop()
{
    defaultLoop();

    if (loopCount_T1 == 0)
    {
        digitalWrite(LED_BUILTIN, ledState);
        ledState = !ledState;
    }

    if (globalTimeOneSecFlag == 1) // get into every 1sec
    {
        Serial.print("[1Sec] ");
        printTime();
        globalTimeOneSecFlag = 0;
    }

    if (globalTimeFiveSecFlag == 1 && globalTimeOneSecFlag == 0) // get into every 5sec
    {
        Serial.print("[5Sec] ");
        printTime();
        sendSensorDataTest();
        globalTimeFiveSecFlag = 0;
    }
}

void defaultLoop()
{
    if (!pubsubClient.connected())
    {
        reconnect();
    }
    pubsubClient.loop();

    loopCount_T1 = (loopCount_T1 + 1) % 50;
    loopCount_T2 = (loopCount_T2 + 1) % 500;
    loopCount_T3 = (loopCount_T3 + 1) % 20;
    delay(10);
}

void printTime()
{
    time_t now = time(nullptr);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    Serial.print("Current time: ");
    Serial.print(asctime(&timeinfo));
}

void sendSensorDataTest()
{
    static int testValue = 0;

    String output;
    StaticJsonDocument<256> doc;

    doc["device_name"] = "NodeMCU_001";
    doc["room_temperature"] = 10000 + testValue;
    doc["room_humidity"] = 20000 + testValue;
    doc["water_temperature"] = 30000 + testValue;
    doc["water_conductivity"] = 40000 + testValue;
    doc["water_ph"] = 50000 + testValue;
    doc["water_co2"] = 60000 + testValue;

    serializeJson(doc, output);

    pubsubClient.publish("update/sensor", output.c_str());

    Serial.printf("Publish sensor data. testValue:%d\n", testValue);
    testValue = (testValue + 1) % 10000;
}

void reconnect()
{
    // Loop until we're reconnected
    while (!pubsubClient.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266";
        // Attempt to connect
        if (pubsubClient.connect(clientId.c_str()))
        {
            Serial.println("connected");
            // Once connected, publish an announcement...
            pubsubClient.publish("outTopic", "Reconnect");
            // ... and resubscribe
            pubsubClient.subscribe("update/actuator");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(pubsubClient.state());
            Serial.println(" try again in 5 seconds");

            char buf[256] = {
                0,
            };
            wifiClient.getLastSSLError(buf, 256);
            Serial.print("WifiClientSecure SSL error: ");
            Serial.println(buf);

            // Wait 5 seconds before retrying
            delay(3000);
        }
    }
}

void onNotification(uint32_t seqId, const MessageInterface::NotificationData *dataArr, size_t arrSize)
{
    Serial.println(__FUNCTION__);
    for (size_t arrIdx = 0; arrIdx < arrSize; arrIdx++)
    {
        const MessageInterface::ResponseGetData &msgData = dataArr[arrIdx];
        Serial.print("SeqId:");
        Serial.print(seqId);
        Serial.print(" Type:");
        Serial.print((uint32_t)msgData.type);
        Serial.print(" Value:");
        Serial.println(msgData.value);
    }

    String output;
    StaticJsonDocument<256> doc;
    doc["device_name"] = "NodeMCU_001";

    for (size_t arrIdx = 0; arrIdx < arrSize; arrIdx++)
    {
        const MessageInterface::ResponseGetData &msgData = dataArr[arrIdx];

        switch (msgData.type)
        {
        case MessageInterface::DataType::SensorWaterTemperature:
            doc["water_temperature"] = msgData.value;
            break;
        case MessageInterface::DataType::SensorRoomTemperature:
            doc["room_temperature"] = msgData.value;
            break;
        case MessageInterface::DataType::SensorCO2:
            doc["water_co2"] = msgData.value;
            break;
        case MessageInterface::DataType::SensorHumidity:
            doc["room_humidity"] = msgData.value;
            break;
        case MessageInterface::DataType::SensorConductivity:
            doc["water_conductivity"] = msgData.value;
            break;
        case MessageInterface::DataType::SensorPH:
            doc["water_ph"] = msgData.value;
            break;
        default:
            break;
        }
    }

    serializeJson(doc, output);
    pubsubClient.publish("update/sensor", output.c_str());

    printTime();
    Serial.printf("Publish sensor data\n %s\n", output.c_str());
}

void setActuator2ArduinoTest()
{
    static int v = 0;
    MessageInterface::MessageSender reqSet(MessageInterface::MsgId::RequestSet);
    reqSet.setSeqId(v++);
    reqSet.appendRequestSetData(MessageInterface::DataType::Control0, v * 10 + 1);
    reqSet.appendRequestSetData(MessageInterface::DataType::Control3, v * 20 + 2);
    reqSet.appendRequestSetData(MessageInterface::DataType::Control1, v * 30 + 3);
    reqSet.appendRequestSetData(MessageInterface::DataType::Control2, v * 40 + 4);
    reqSet.sendMessage();

    I2CInterface::I2C_Master::writeSlaveBuffer();
}
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