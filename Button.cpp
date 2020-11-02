#include "Button.h"
#include <FunctionalInterrupt.h>

using namespace std;

Button::Button(uint32_t gpioNum, const char* name, function<void()> isr, uint32_t debounceTime)
: _gpioNum(gpioNum), _name(name), _debounceTime(debounceTime), _lastCallTime(0), _count(0), _isr(isr)
{
}

Button::~Button()
{}

void Button::setup()
{
    pinMode(_gpioNum, INPUT_PULLUP);
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