#ifndef _MESSAGE_INTERFACE_H_
#define _MESSAGE_INTERFACE_H_

#include <string>
#include <ctime>
#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;

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
        Double,
    } eValueType;

    struct Value
    {
        eValueType type;
        union {
            uint32_t val_int;
            double val_double;
        } value;
    };

    eDataType str2EnumDataType(const char *input);
    std::string enum2Str(eDataType input);

    std::string getCurrentTime();

    uint8_t getCheckSum(const std::string &message);
    void appendCheckSum(std::string &message);
    bool verityCheckSum(const std::string &message);

}; // namespace UartMessageInterface

#endif
