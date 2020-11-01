#include <cstdio>

const uint32_t debounceTime = 200; // 0.2s

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

const int LED_R = 0; // D3;
const int LED_G = 2; // D4;
const int LED_B = 14; // D5;
void setup3ColorLED()
{
    pinMode(LED_B, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_R, OUTPUT);
    Serial.println("3Color LED OK...");
}

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

char* color2String(int color)
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
    digitalWrite(LED_R, R);
    digitalWrite(LED_G, G);
    digitalWrite(LED_B, B);

    char s[64] = {0, };
    snprintf(s, sizeof(s), "[LED] Color:%s - B:%d G:%d R:%d", color2String(color), B, G, R);
    Serial.println(s);
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

    set3LedColor(eColor::Red);
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

    set3LedColor(eColor::Green);
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
    
    set3LedColor(eColor::Blue);
}

void setupButtonD8()
{
    const int interrupt_pin = 15; // D8 (PULL DOWN)
    pinMode(interrupt_pin, INPUT_PULLUP);
    attachInterrupt(interrupt_pin, interruptHandler_D8_button, FALLING);
    Serial.println("D8 Button OK...");
}

void setup()
{
    setupSerial();

    setupLED();
    setup3ColorLED();

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
    static int color = 0;
    color += 1;
    color = color % 8;

    set3LedColor(color);

    const int loopDelaySec = 1000;
    delay(loopDelaySec);
}
