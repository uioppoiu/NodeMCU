#include <cstdio>

const uint32_t debounceTime = 100; // 0.1s

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

uint32_t lastCallTimeD3 = 0;
ICACHE_RAM_ATTR void interruptHandler_D3_button()
{
    if(millis() - lastCallTimeD3 < debounceTime) return;
    lastCallTimeD3 = millis();

    static int count = 0;
    char s[64] = {0, };
    snprintf(s, sizeof(s), "D3 Button. Count:%d", ++count);
    Serial.println(s);
}

void setupButtonD3()
{
    const int interrupt_pin = 0; // D3
    pinMode(interrupt_pin, INPUT_PULLUP);
    attachInterrupt(interrupt_pin, interruptHandler_D3_button, FALLING);
    Serial.println("D3 Button OK...");
}

uint32_t lastCallTimeD6 = 0;
ICACHE_RAM_ATTR void interruptHandler_D6_button()
{
    if(millis() - lastCallTimeD6 < debounceTime) return;
    lastCallTimeD6 = millis();

    static int count = 0;
    char s[64] = {0, };
    snprintf(s, sizeof(s), "D6 Button. Count:%d", ++count);
    Serial.println(s);
}

void setupButtonD6()
{
    const int interrupt_pin = 12; // D6
    pinMode(interrupt_pin, INPUT_PULLUP);
    attachInterrupt(interrupt_pin, interruptHandler_D6_button, FALLING);
    Serial.println("D6 Button OK...");
}

uint32_t lastCallTimeD7 = 0;
ICACHE_RAM_ATTR void interruptHandler_D7_button()
{
    if(millis() - lastCallTimeD7 < debounceTime) return;
    lastCallTimeD7 = millis();

    static int count = 0;
    char s[64] = {0, };
    snprintf(s, sizeof(s), "D7 Button. Count:%d", ++count);
    Serial.println(s);
}

void setupButtonD7()
{
    const int interrupt_pin = 13; // D7
    pinMode(interrupt_pin, INPUT_PULLUP);
    attachInterrupt(interrupt_pin, interruptHandler_D7_button, FALLING);
    Serial.println("D7 Button OK...");
}

uint32_t lastCallTimeD8 = 0;
ICACHE_RAM_ATTR void interruptHandler_D8_button()
{
    if(millis() - lastCallTimeD8 < debounceTime) return;
    lastCallTimeD8 = millis();

    static int count = 0;
    char s[64] = {0, };
    snprintf(s, sizeof(s), "D8 Button. Count:%d", ++count);
    Serial.println(s);
}

void setupButtonD8()
{
    const int interrupt_pin = 15; // D8
    pinMode(interrupt_pin, INPUT_PULLUP);
    attachInterrupt(interrupt_pin, interruptHandler_D8_button, FALLING);
    Serial.println("D8 Button OK...");
}

void setup()
{
    setupSerial();

    setupLED();
    

    // setupButtonD3();
    setupButtonD6();
    setupButtonD7();
    setupButtonD8();

    Serial.println("Initialize done...");
    Serial.println("Initialize done...");
    Serial.println("Initialize done...");
}

void loop()
{
    const int loopDelaySec = 500;
    delay(loopDelaySec);
}
