#include <time.h>
#include "UartMessageInterface.h"

namespace UartMessageInterface
{
    eDataType str2EnumDataType(const char *input)
    {
        String str(input);
        if (str.equals("SensorAll"))
            return SensorAll;
        if (str.equals("Temp"))
            return SensorTemperature;
        if (str.equals("CO2"))
            return SensorCO2;
        if (str.equals("Humid"))
            return SensorHumidity;
        if (str.equals("Conduct"))
            return SensorConductivity;
        if (str.equals("CtrlAll"))
            return ControlAll;
        if (str.equals("Control0"))
            return Control0;
        if (str.equals("Control1"))
            return Control1;
        if (str.equals("Control2"))
            return Control2;
        if (str.equals("DateTime"))
            return DateTime;
        return Invalid;
    }

    const char* enum2Str(eDataType input)
    {
        switch (input)
        {
        case SensorAll:
            return "SensorAll";
        case SensorTemperature:
            return "Temp";
        case SensorCO2:
            return "CO2";
        case SensorHumidity:
            return "Humid";
        case SensorConductivity:
            return "Conduct";
        case ControlAll:
            return "CtrlAll";
        case Control0:
            return "Control0";
        case Control1:
            return "Control1";
        case Control2:
            return "Control2";
        case DateTime:
            return "DateTime";
        case Invalid:
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