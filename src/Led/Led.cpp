#include "Led.h"

std::string color2String(int color)
{
    if(color == eColor::Black) return "Black";
    if(color == eColor::Red) return "Red";
    if(color == eColor::Green) return "Green";
    if(color == eColor::Yellow) return "Yellow";
    if(color == eColor::Blue) return "Blue";
    if(color == eColor::Magenta) return "Magenta";
    if(color == eColor::Cyan) return "Cyan";
    if(color == eColor::White) return "White";
    return "Invalid";
}

void set3LedColor(int color)
{
    bool R = color & 0x01;
    bool G = color & 0x02;
    bool B = color & 0x04;
    digitalWrite(LED_PIN_R, R);
    digitalWrite(LED_PIN_G, G);
    digitalWrite(LED_PIN_B, B);

    char s[64] = {0, };
    snprintf(s, sizeof(s), "[LED] Color:%s - B:%d G:%d R:%d", color2String(color).c_str(), B, G, R);
    Serial.println(s);
}