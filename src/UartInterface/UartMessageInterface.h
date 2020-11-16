#ifndef _MESSAGE_INTERFACE_H_
#define _MESSAGE_INTERFACE_H_

#include <Arduino.h>

namespace UartMessageInterface
{
    typedef enum
    {
        Request = 0,
        Response,
        Notification,
        Acknowledge,
    } eMessageType;

    typedef enum
    {    
        Get = 0,
        Set,
        Subscribe,
        Unsubscribe
    } eCommandType;

    typedef enum
    {
        Invalid = -1,
        SensorAll = 10,
        SensorTemperature,
        SensorCO2,
        SensorHumidity,
        SensorConductivity,
        ControlAll = 20,
        Control0,
        Control1,
        Control2,
        DateTime = 30,
    } eDataType;

    typedef enum
    {
        Integer = 0,
        Float,
    } eValueType;

    struct Value
    {
        eValueType type;
        union {
            int val_int;
            float val_float;
        } value;
    };

    eDataType str2EnumDataType(const char *input);
    const char* enum2Str(eDataType input);

    String getCurrentTime();

    uint8_t getCheckSum(const String &message);
    void appendCheckSum(String &message);
    bool verityCheckSum(const String &message);

}; // namespace UartMessageInterface

#endif
