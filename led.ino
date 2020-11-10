#include "Led.h"

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