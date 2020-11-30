#include <time.h>
#include "UartMessageInterface.h"

namespace UartMessageInterface
{
    const char* DataTypeStr(const unsigned char input)
    {
        switch (input)
        {
        case DataType::SensorAll:
            return "SensorAll";
        case DataType::SensorTemperature:
            return "Temp";
        case DataType::SensorCO2:
            return "CO2";
        case DataType::SensorHumidity:
            return "Humid";
        case DataType::SensorConductivity:
            return "Conduct";
        case DataType::ControlAll:
            return "CtrlAll";
        case DataType::Control0:
            return "Control0";
        case DataType::Control1:
            return "Control1";
        case DataType::Control2:
            return "Control2";
        case DataType::DateTime:
            return "DateTime";
        default:
            return "-";
        }
    }

    String getCurrentTime()
    {
        char sResult[32] = {
            0,
        };
        time_t result = time(nullptr);
        strftime(sResult, sizeof(sResult), "%FT%T", localtime(&result));

        // cout << sResult << endl;
        return sResult;
    }

    uint8_t getCheckSum(const String &message)
    {
        uint8_t sum = 0;
        for (const char &c : message)
        {
            sum = sum + (uint8_t)c;
        }

        uint8_t checkSum = (sum ^ 0xFF) + 1;

        // cout << "CheckSum : " << (int)checkSum << endl;
        return checkSum;
    }

    char getCheckSum(const char* str, size_t strSize)
    {
        uint8_t sum = 0;
        for(size_t idx = 0 ; idx < strSize ; idx++)
        {
            sum = sum + (uint8_t)str[idx];
        }

        uint8_t checkSum = (sum ^ 0xFF) + 1;

        // cout << "CheckSum : " << (int)checkSum << endl;
        return (char)checkSum;
    }

    void appendCheckSum(String &message)
    {
        message += ((char)getCheckSum(message));
    }

    bool verityCheckSum(const String &message)
    {
        uint8_t sum = 0;
        for (const char &c : message)
        {
            sum = sum + (uint8_t)c;
        }

        // cout << "Verity : " << (sum == 0) << endl;
        return (sum == 0);
    }

}; // namespace UartMessageInterface