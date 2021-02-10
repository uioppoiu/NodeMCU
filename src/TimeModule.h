#ifndef __TIME_MODULE_H__
#define __TIME_MODULE_H__

#include <time.h>
#include <TZ.h>

class TimeModule
{
public:
    static void setClock();
    static void printTime();
};

#endif