#include "Button.h"
#include "Led.h"

ICACHE_RAM_ATTR void interruptHandler_D6_button();
Button Button_D6(12, "[D6/GPIO12]", &interruptHandler_D6_button);
void interruptHandler_D6_button()
{
    set3LedColor(eColor::Red);
}

ICACHE_RAM_ATTR void interruptHandler_D7_button();
Button Button_D7(13, "[D7/GPIO13]", &interruptHandler_D7_button);
void interruptHandler_D7_button()
{
    set3LedColor(eColor::Green);
}

ICACHE_RAM_ATTR void interruptHandler_D8_button();
Button Button_D8(15, "[D8/GPIO15]", &interruptHandler_D8_button); // PULL DOWN
void interruptHandler_D8_button()
{
    set3LedColor(eColor::Blue);
}