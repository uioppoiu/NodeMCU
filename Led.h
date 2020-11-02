#ifndef _LED_H_
#define _LED_H_

#include <Arduino.h>

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

std::string color2String(int color);
void set3LedColor(int color);

#endif