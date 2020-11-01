#include <FunctionalInterrupt.h>
#include <functional>
#include <string>

using namespace std;

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

const int LED_PIN_R = 0; // D3;
const int LED_PIN_G = 2; // D4;
const int LED_PIN_B = 14; // D5;
void setup3ColorLED()
{
    pinMode(LED_PIN_B, OUTPUT);
    pinMode(LED_PIN_G, OUTPUT);
    pinMode(LED_PIN_R, OUTPUT);
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

const uint32_t DebounceTime = 200; // 0.2s

class Button
{
private:
    const uint32_t _gpioNum;
    const string _name;
    uint32_t _lastCallTime;
    uint32_t _count;
    function<void ()> _isr;

    void isrCommon();

public:
    Button(uint32_t gpioNum, const char* name, function<void()> isr);
    ~Button();

    void initialize();
    uint32_t gpioNum();
    string name();
};



Button::Button(uint32_t gpioNum, const char* name, function<void()> isr)
: _gpioNum(gpioNum), _name(name), _lastCallTime(0), _count(0), _isr(isr)
{
}

Button::~Button()
{}

void Button::initialize()
{
    pinMode(_gpioNum, INPUT_PULLUP);
    // function<void(void)> f = bind(&Button::isrCommon, this);
    attachInterrupt(_gpioNum, bind(&Button::isrCommon, this), FALLING);

    char s[64] = {0, };
    snprintf(s, sizeof(s), "%s Button OK...", _name.c_str());
    Serial.println(s);
}

uint32_t Button::gpioNum()
{
    return _gpioNum;
}

string Button::name()
{
    return _name;
}

void Button::isrCommon()
{
    if(millis() - _lastCallTime < DebounceTime) return;
    _lastCallTime = millis();

    char s[64] = {0, };
    snprintf(s, sizeof(s), "%s Button. Count:%d", _name.c_str(), ++_count);
    Serial.println(s);
    
    if(_isr) _isr();
}

ICACHE_RAM_ATTR void interruptHandler_D6_button();
Button button_D6(12, "[D6/GPIO12]", &interruptHandler_D6_button);
void interruptHandler_D6_button()
{
    set3LedColor(eColor::Red);
}

ICACHE_RAM_ATTR void interruptHandler_D7_button();
Button button_D7(13, "[D7/GPIO13]", &interruptHandler_D7_button);
void interruptHandler_D7_button()
{
    set3LedColor(eColor::Green);
}

ICACHE_RAM_ATTR void interruptHandler_D8_button();
Button button_D8(15, "[D8/GPIO15]", &interruptHandler_D8_button); // PULL DOWN
void interruptHandler_D8_button()
{
    set3LedColor(eColor::Blue);
}

void setup()
{
    setupSerial();

    setupLED();
    setup3ColorLED();

    button_D6.initialize();
    button_D7.initialize();
    button_D8.initialize();

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
