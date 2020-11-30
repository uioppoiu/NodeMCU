#ifndef _UART_MESSAGE_RECEIVER_H_
#define _UART_MESSAGE_RECEIVER_H_

#include <Arduino.h>
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
        const String _messageBuffer;
        UartMessageReceiver();
    };

}; // namespace UartMessageInterface

#endif
