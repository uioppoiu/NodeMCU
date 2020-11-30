#include "Arduino.h"
#include "UartMessageSender.h"

namespace UartMessageInterface
{

    UartMessageSender::UartMessageSender(const unsigned char msgId)
    : _header((MsgCommonHeader*)_messageBuffer)
    {
        memset(_messageBuffer, 0x00, sizeof(_messageBuffer));
        _header->msgId = msgId;
        _header->msgSize = sizeof(MsgCommonHeader);
    }
    
    UartMessageSender::~UartMessageSender()
    {}

    void UartMessageSender::setSeqId(uint32_t seqId)
    {
        _header->seqId = seqId;
    }

    void UartMessageSender::sendMessage()
    {
        Serial.write("<BEGIN>", sizeof("<BEGIN>"));
        delay(10);

        char checkSum = getCheckSum(_messageBuffer, _header->msgSize);
        _messageBuffer[_header->msgSize] = checkSum;
        _header->msgSize += 1;

        // Serial.printf("CheckSum:%u\n", checkSum);
        // Serial.printf("MsgId:%u MsgSize:%u\n", _header->msgId, _header->msgSize);
        // const RequestGetData* data = (const RequestGetData*)(_messageBuffer + sizeof(MsgCommonHeader));
        // Serial.printf("Type:%u Name:%s\n", data[0].type, data[0].name);
        // Serial.printf("Type:%u Name:%s\n", data[1].type, data[1].name);
        // Serial.printf("Type:%u Name:%s\n", data[2].type, data[2].name);
        
        // Serial.write(_messageBuffer, _header->msgSize);

        for(size_t offset = 0 ; offset < _header->msgSize ; )
        {
            size_t size = ((_header->msgSize - offset) > 64) ? 64 : (_header->msgSize - offset);
            Serial.write(_messageBuffer + offset, size);
            delay(10);
            offset += size;
        }

        Serial.write("<END>", sizeof("<END>"));
        delay(10);
    }

    void UartMessageSender::appendRequestGetData(unsigned char dataType, const char *name, size_t sizeOfName)
    {
        RequestGetData data;

        data.type = dataType;

        memset(data.name, 0x00, sizeof(data.name));

        size_t strSize = (sizeOfName > sizeof(data.name) - 1) ? (sizeof(data.name) - 1) : sizeOfName;
        memcpy(data.name, name, strSize);

        // Serial.printf("DataType:%u, Name:%s\n", dataType, name);

        appendData(data);
    }

    void UartMessageSender::appendResponseGetData(unsigned char dataType, const char *name, size_t sizeOfName, uint32_t value)
    {
        ResponseGetData data;

        data.type = dataType;

        memset(data.name, 0x00, sizeof(data.name));

        size_t strSize = (sizeOfName > sizeof(data.name) - 1) ? (sizeof(data.name) - 1) : sizeOfName;
        memcpy(data.name, name, strSize);

        data.value = value;

        appendData(data);
    }

    void UartMessageSender::appendNotificationData(unsigned char dataType, const char *name, size_t sizeOfName, uint32_t value)
    {
        appendResponseGetData(dataType, name, sizeOfName, value);
    }

    void UartMessageSender::appendRequestSetData(unsigned char dataType, const char *name, size_t sizeOfName, uint32_t value)
    {
        appendResponseGetData(dataType, name, sizeOfName, value);
    }

    void UartMessageSender::appendSubscribeData(unsigned char dataType, const char *name, size_t sizeOfName)
    {
        appendRequestGetData(dataType, name, sizeOfName);
    }

    void UartMessageSender::appendUnsubscribeData(unsigned char dataType, const char *name, size_t sizeOfName)
    {
        appendRequestGetData(dataType, name, sizeOfName);
    }

}; // namespace UartMessageInterface