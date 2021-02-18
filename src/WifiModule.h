#ifndef __WIFI_MODULE_H__
#define __WIFI_MODULE_H__

#include <ESP8266WiFi.h>

#define WIFI_ID "A1504_2.4Ghz"
#define WIFI_PASS "hongsik102*"

class WifiModule
{
public:
    static X509List ca;
    static X509List cert;
    static PrivateKey key;
    static WiFiClientSecure wifiClient;

    static void init(const char* cert_str, const char* key_str, const char* ca_str);
};

#endif