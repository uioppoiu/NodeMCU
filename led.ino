#include <Arduino.h>
#include <string>

using namespace std;

const int LED_PIN_R = 0; // D3;
const int LED_PIN_G = 2; // D4;
const int LED_PIN_B = 14; // D5;

struct eColor
{
    typedef enum
    {
        Black = 0x00,
        Red = 0x01,
        Green = 0x02,
        Yellow = 0x03,
        Blue = 0x04,
        Magenta = 0x05,
        Cyan = 0x06,
        White = 0x07
    } Enum;
};

string color2String(int color)
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

void setupLED()
{
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("LED OK...");
}

void setup3ColorLED()
{
    pinMode(LED_PIN_B, OUTPUT);
    pinMode(LED_PIN_G, OUTPUT);
    pinMode(LED_PIN_R, OUTPUT);
    Serial.println("3Color LED OK...");
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