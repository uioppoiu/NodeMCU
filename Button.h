#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <Arduino.h>
#include <functional>
#include <string>

using namespace std;

const uint32_t DebounceTime = 200; // 0.2s

class Button
{
private:
    const uint32_t _gpioNum;
    const string _name;
    const uint32_t _debounceTime;
    uint32_t _lastCallTime;
    uint32_t _count;
    function<void ()> _isr;

    void isrCommon();

public:
    Button(uint32_t gpioNum, const char* name, function<void()> isr, uint32_t debounceTime = DebounceTime);
    ~Button();

    void setup();
    uint32_t gpioNum();
    string name();
};

#endif