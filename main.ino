#include <cstdio>

void setupSerial()
{
    Serial.begin(115200);
    Serial.println("Serial OK...");
}

void setupLED()
{
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("LED OK...");
}

uint32_t lastFallD3 = 0;
ICACHE_RAM_ATTR void interruptHandler_D3_button()
{
    lastFallD3 = millis();
    // static int count = 0;
    // char s[64] = {0, };
    // snprintf(s, sizeof(s), "D3 Button. Count:%d", ++count);
    // Serial.println(s);
}

void setupButtonD3()
{
    const int interrupt_pin = 0; // D3
    pinMode(interrupt_pin, INPUT_PULLUP);
    attachInterrupt(interrupt_pin, interruptHandler_D3_button, FALLING);
    Serial.println("D3 Button OK...");
}

uint32_t lastFallD6 = 0;
ICACHE_RAM_ATTR void interruptHandler_D6_button()
{
    lastFallD6 = millis();
    // static int count = 0;
    // char s[64] = {0, };
    // snprintf(s, sizeof(s), "D6 Button. Count:%d", ++count);
    // Serial.println(s);
}

void setupButtonD6()
{
    const int interrupt_pin = 12; // D6
    pinMode(interrupt_pin, INPUT_PULLUP);
    attachInterrupt(interrupt_pin, interruptHandler_D6_button, FALLING);
    Serial.println("D6 Button OK...");
}

uint32_t lastFallD7 = 0;
ICACHE_RAM_ATTR void interruptHandler_D7_button()
{
    lastFallD7 = millis();
    // static int count = 0;
    // char s[64] = {0, };
    // snprintf(s, sizeof(s), "D7 Button. Count:%d", ++count);
    // Serial.println(s);
}

void setupButtonD7()
{
    const int interrupt_pin = 13; // D7
    pinMode(interrupt_pin, INPUT_PULLUP);
    attachInterrupt(interrupt_pin, interruptHandler_D7_button, FALLING);
    Serial.println("D7 Button OK...");
}

void setup()
{
    setupSerial();
    setupLED();
    setupButtonD3();
    setupButtonD6();
    setupButtonD7();

    Serial.println("Initialize done...");
    Serial.println("Initialize done...");
    Serial.println("Initialize done...");
}

int countD3 = 0;
int countD6 = 0;
int countD7 = 0;
void loop()
{
    const int loopDelaySec = 500;
    delay(loopDelaySec);

    uint32_t currentTime = millis();

    if (currentTime - lastFallD3 < loopDelaySec)
    {
        char s[64] = {0, };
        snprintf(s, sizeof(s), "D3 Button. Count:%d", ++countD3);
        Serial.println(s);
    }

    if (currentTime - lastFallD6 < loopDelaySec)
    {
        char s[64] = {0, };
        snprintf(s, sizeof(s), "D6 Button. Count:%d", ++countD6);
        Serial.println(s);
    }

    if (currentTime - lastFallD7 < loopDelaySec)
    {
        char s[64] = {0, };
        snprintf(s, sizeof(s), "D7 Button. Count:%d", ++countD7);
        Serial.println(s);
    }
}
