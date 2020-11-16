#include "UartMessageCallbackManagement.h"

namespace UartMessageInterface
{
    UartMessageCallbackManagement *UartMessageCallbackManagement::_Instance = NULL;

    UartMessageCallbackManagement &UartMessageCallbackManagement::getInstance()
    {
        if (_Instance == NULL)
        {
            _Instance = new UartMessageCallbackManagement;
        }
        return *_Instance;
    }

    void UartMessageCallbackManagement::registerRequestGetCallBack(const CallBackRequestGet func)
    {
        getInstance()._callBackRequestGet = func;
    }

    void UartMessageCallbackManagement::invokeRequestGetCallBack(eDataType dataType, const String &name)
    {
        getInstance()._callBackRequestGet(dataType, name);
    }

    void UartMessageCallbackManagement::registerResponseGetCallBack(const CallBackResponseGet func)
    {
        getInstance()._callBackResponseGet = func;
    }

    void UartMessageCallbackManagement::invokeResponseGetCallBack(eDataType dataType, const String &name, const Value &value)
    {
        getInstance()._callBackResponseGet(dataType, name, value);
    }

    void UartMessageCallbackManagement::registerSubscribeCallBack(const CallBackSubscribe func)
    {
        getInstance()._callBackSubscribe = func;
    }

    void UartMessageCallbackManagement::invokeSubscribeCallBack(eDataType dataType, const String &name, unsigned int period)
    {
        getInstance()._callBackSubscribe(dataType, name, period);
    }

    void UartMessageCallbackManagement::registerUnsubscribeCallBack(const CallBackUnsubscribe func)
    {
        getInstance()._callBackUnsubscribe = func;
    }

    void UartMessageCallbackManagement::invokeUnsubscribeCallBack(eDataType dataType, const String &name)
    {
        getInstance()._callBackUnsubscribe(dataType, name);
    }

}; // namespace UartMessageInterface