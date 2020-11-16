#ifndef _UART_MESSAGE_SENDER_H_
#define _UART_MESSAGE_SENDER_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include "UartMessageInterface.h"
#include "UartMessageCallbackManagement.h"

namespace UartMessageInterface
{
    class UartMessageSender
    {
    public:
        UartMessageSender(eMessageType messageType, eCommandType commandType);
        ~UartMessageSender();

        // Request
        void appendRequest(eDataType type, const String &name); // All 이면 return;
        void appendRequestAll(eDataType type);                  // 나머지 비우고 All로

        // Response
        template<typename VALUE_TYPE>
        void appendResponse(eDataType dataType, const String &name, eValueType valueType, const VALUE_TYPE &value)
        {
            if (!_jsonDoc.containsKey("Data"))
            {
                _jsonDoc.createNestedArray("Data");
            }

            JsonArray dataArr = _jsonDoc.getMember("Data");
            JsonObject data = dataArr.createNestedObject();
            switch (dataType)
            {
            case SensorTemperature:
                data["Type"] = "Temp";
                break;
            case SensorCO2:
                data["Type"] = "CO2";
                break;
            case SensorHumidity:
                data["Type"] = "Humid";
                break;
            case SensorConductivity:
                data["Type"] = "Conduct";
                break;
            case Control1:
                data["Type"] = "Control1";
                break;
            case Control2:
                data["Type"] = "Control2";
                break;
            case DateTime:
                data["Type"] = "DateTime";
                break;
            default:
                return;
            }

            data["Name"] = name;

            if (valueType == Float)
            {
                data["ValType"] = "Float";
                data["Val"] = (float)value;

            }
            else // Integer
            {
                data["ValType"] = "Integer";
                data["Val"] = (int)value;
            }
        }

        // Subscribe
        void appendSubscribe(eDataType type, const String &name, unsigned int period); // All 이면 return;
        void appendSubscribeAll(eDataType type, unsigned int period);                  // 나머지 비우고 All로

        // Unsubscribe
        void appendUnsubscribe(eDataType type, const String &name); // All 이면 return;
        void appendUnsubscribeAll(eDataType type);                  // 나머지 비우고 All로

        String sendMessage();

    private:
        unsigned int _seqId;
        DynamicJsonDocument _jsonDoc;
        UartMessageSender();
    };

}; // namespace UartMessageInterface

#endif
