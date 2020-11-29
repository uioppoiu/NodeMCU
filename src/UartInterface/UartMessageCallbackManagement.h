#ifndef _UART_MESSAGE_CALL_BACK_MANAGEMENT_H_
#define _UART_MESSAGE_CALL_BACK_MANAGEMENT_H_

#include <Arduino.h>
#include "UartMessageInterface.h"

namespace UartMessageInterface
{
    class UartMessageCallbackManagement
    {
    public:
        typedef void(*CallBackRequestGet)(eDataType, const String &);
        typedef void(*CallBackResponseGet)(eDataType, const String &, const Value &);
        typedef void(*CallBackSubscribe)(eDataType, const String &, unsigned int);
        typedef void(*CallBackUnsubscribe)(eDataType, const String &);
        typedef void(*CallBackRequestSet)(eDataType, const String &, const Value &);
        typedef void(*CallBackAcknowledge)(eDataType, const String &);
        
        static void registerRequestGetCallBack(const CallBackRequestGet func);
        static void invokeRequestGetCallBack(eDataType type, const String &name);
        static void registerResponseGetCallBack(const CallBackResponseGet func);
        static void invokeResponseGetCallBack(eDataType type, const String &name, const Value &value);
        static void registerSubscribeCallBack(const CallBackSubscribe func);
        static void invokeSubscribeCallBack(eDataType type, const String &name, unsigned int period);
        static void registerUnsubscribeCallBack(const CallBackUnsubscribe func);
        static void invokeUnsubscribeCallBack(eDataType type, const String &name);
        static void registerAcknowledgeCallBack(const CallBackAcknowledge func);
        static void invokeAcknowledgeCallBack(eDataType type, const String &name);

    private:
        static UartMessageCallbackManagement *_Instance;
        static UartMessageCallbackManagement &getInstance();

        CallBackRequestGet _callBackRequestGet;
        CallBackResponseGet _callBackResponseGet;
        CallBackSubscribe _callBackSubscribe;
        CallBackUnsubscribe _callBackUnsubscribe;
        CallBackAcknowledge _callBackAcknowledge;
    };


}; // namespace UartMessageInterface

#endif
