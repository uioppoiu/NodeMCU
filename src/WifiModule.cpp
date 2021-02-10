#include "WifiModule.h"

X509List WifiModule::ca;
X509List WifiModule::cert;
PrivateKey WifiModule::key;
WiFiClientSecure WifiModule::wifiClient;

void WifiModule::init(const char* cert_str, const char* key_str, const char* ca_str)
{
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
    ca.append(ca_str);
    cert.append(cert_str);
    key.parse(key_str);
    wifiClient.setTrustAnchors(&ca);
    wifiClient.setClientRSACert(&cert, &key);
}