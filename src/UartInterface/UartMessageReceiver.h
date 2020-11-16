#ifndef _UART_MESSAGE_RECEIVER_H_
#define _UART_MESSAGE_RECEIVER_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include "UartMessageInterface.h"
#include "UartMessageCallbackManagement.h"

namespace UartMessageInterface
{
    class UartMessageReceiver
    {
    public:
        UartMessageReceiver(const String &message);
        ~UartMessageReceiver();
        
        bool isMessageValid();
        void processMessage();

    private:
        DynamicJsonDocument _jsonDoc;
        UartMessageReceiver();

        bool isUartMessage(const String &message);

        void handleRequestMessage();
        void invokeRequestGet(JsonObjectConst data);
        void invokeSubscribe(JsonObjectConst data);
        void invokeUnsubscribe(JsonObjectConst data);

        void handleResponseMessage();
        void invokeResponseGet(JsonObjectConst data);

        // TODO Set
    };

}; // namespace UartMessageInterface

#endif
