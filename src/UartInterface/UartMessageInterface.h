#ifndef _MESSAGE_INTERFACE_H_
#define _MESSAGE_INTERFACE_H_

#include <Arduino.h>

namespace UartMessageInterface
{
    const char* DataTypeStr(const unsigned char input);

    String getCurrentTime();

    uint8_t getCheckSum(const String &message);
    char getCheckSum(const char* str, size_t strSize);
    void appendCheckSum(String &message);
    bool verityCheckSum(const String &message);

    typedef struct {
        unsigned char msgId; // 메시지 ID
        unsigned char msgSize; // 헤더를 포함한 메시지 크기
        unsigned char numOfData; // 데이터의 수
        unsigned char sizeOfData; // 각 데이터의 사이즈
        uint32_t seqId;
    } MsgCommonHeader;

    typedef struct {
        unsigned char type;
        char name[11];
    } RequestGetData, SubscribeData, UnsubscribeData;

    typedef struct {
        unsigned char type;
        char name[11];
        uint32_t value;
    } ResponseGetData, RequestSetData, NotificationData;

    class MsgId
    {
    public:
        static const unsigned char RequestGet = 0x10;
        static const unsigned char RequestSet = 0x20;
        static const unsigned char Subscribe = 0x30;
        static const unsigned char Unsubscribe = 0x40;
        static const unsigned char Notification = 0x50;
        static const unsigned char ResponseGet = 0x60;
        static const unsigned char Acknowledge = 0x70;
    };

    class DataType
    {
    public:
        static const unsigned char SensorAll = 0x0F;
        static const unsigned char SensorTemperature = 0x00;
        static const unsigned char SensorCO2 = 0x01;
        static const unsigned char SensorHumidity = 0x02;
        static const unsigned char SensorConductivity = 0x03;
        static const unsigned char ControlAll = 0x1F;
        static const unsigned char Control0 = 0x10;
        static const unsigned char Control1 = 0x11;
        static const unsigned char Control2 = 0x12;
        static const unsigned char Control3 = 0x13;
        static const unsigned char DateTime = 0x20;
    };

}; // namespace UartMessageInterface

#endif
