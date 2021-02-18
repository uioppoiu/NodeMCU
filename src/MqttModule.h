#ifndef __MQTT_MODULE_H__
#define __MQTT_MODULE_H__

#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "WifiModule.h"

class MqttModule
{
public:
    static PubSubClient pubsubClient;

    static void init(const char * domain, uint16_t port, WiFiClientSecure &client);
    static void setCallback(MQTT_CALLBACK_SIGNATURE);
    static void reconnect();
    static void loop();
};

#endif