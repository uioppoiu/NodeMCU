#include <Arduino.h>
#include "Button.h"
#include "Led.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("Serial OK...");

    setupLED();

    setup3ColorLED();

    Button_D6.setup();
    Button_D7.setup();
    Button_D8.setup();

    Serial.println("Initialize done...");
}

void loop()
{
    static int color = 0;
    color += 1;
    color = color % 8;
    set3LedColor(color);

    const int loopDelaySec = 1000;
    delay(loopDelaySec);
}
