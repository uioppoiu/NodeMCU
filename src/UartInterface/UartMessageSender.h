#ifndef _UART_MESSAGE_SENDER_H_
#define _UART_MESSAGE_SENDER_H_

#include <Arduino.h>
#include "UartMessageInterface.h"
#include "UartMessageCallbackManagement.h"

namespace UartMessageInterface
{
    class UartMessageSender
    {
    public:
        UartMessageSender(const unsigned char msgId);
        ~UartMessageSender();

        void setSeqId(uint32_t seqId);
        
        template<typename T>
        void appendData(const T& data)
        {
            if (_header->sizeOfData != 0)
            {
                if (_header->sizeOfData != sizeof(T))
                {
                    // 호환이 안 되는 Data type을 섞어서 append할 수 없다.
                    return;
                }
            }

            memcpy(_messageBuffer + sizeof(MsgCommonHeader) + (_header->numOfData * _header->sizeOfData), &data, sizeof(T));

            _header->msgSize += sizeof(T);
            _header->numOfData += 1;
            _header->sizeOfData = sizeof(T);
        }

        void appendRequestGetData(unsigned char dataType, const char* name, size_t sizeOfName);
        void appendResponseGetData(unsigned char dataType, const char* name, size_t sizeOfName, uint32_t value);
        void appendNotificationData(unsigned char dataType, const char* name, size_t sizeOfName, uint32_t value);
        void appendRequestSetData(unsigned char dataType, const char* name, size_t sizeOfName, uint32_t value);
        void appendSubscribeData(unsigned char dataType, const char* name, size_t sizeOfName);
        void appendUnsubscribeData(unsigned char dataType, const char* name, size_t sizeOfName);

        void sendMessage();

    private:
        char _messageBuffer[128];
        MsgCommonHeader* _header;
        UartMessageSender();
    };

}; // namespace UartMessageInterface

#endif
