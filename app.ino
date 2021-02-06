#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include <TZ.h>
#include "src/aws_cert.h"

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
    Serial.printf("LED Initialized. Pin:%d", LED_BUILTIN);

    // I2C : Master
    // I2CInterface::I2C_Master::init();

    // Callback
    // I2CInterface::CallBack::registerNotificationCallBack(onNotification);

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
bool ledState = 0;
void loop()
{
    defaultLoop();

    if (loopCount_T1 == 0)
    {
        digitalWrite(LED_BUILTIN, ledState);
        ledState = !ledState;
    }

    if (loopCount_T2 == 0)
    {
        printTime();
        sendSensorData();
    }
}

void defaultLoop()
{
    if (!pubsubClient.connected())
    {
        reconnect();
    }
    pubsubClient.loop();

    loopCount_T1 = (loopCount_T1 + 1) % 500;
    loopCount_T2 = (loopCount_T2 + 1) % 5000;
    delay(1);
}

void printTime()
{
    time_t now = time(nullptr);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    Serial.print("Current time: ");
    Serial.print(asctime(&timeinfo));
}

void sendSensorData()
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
            pubsubClient.subscribe("inTopic");
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
