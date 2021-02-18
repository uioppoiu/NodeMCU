#include "MqttModule.h"

PubSubClient MqttModule::pubsubClient;

void MqttModule::init(const char * domain, uint16_t port, WiFiClientSecure &client)
{
    pubsubClient.setServer(domain, port);
    pubsubClient.setClient(client);
}

void MqttModule::setCallback(MQTT_CALLBACK_SIGNATURE)
{
    pubsubClient.setCallback(callback);
}

void MqttModule::reconnect()
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
            WifiModule::wifiClient.getLastSSLError(buf, 256);
            Serial.print("WifiClientSecure SSL error: ");
            Serial.println(buf);

            // Wait 5 seconds before retrying
            delay(3000);
        }
    }
}

void MqttModule::loop()
{
    reconnect();
    pubsubClient.loop();
}