#include "UartMessageCallbackManagement.h"

namespace UartMessageInterface
{
    UartMessageCallbackManagement::CallBackRequestGet UartMessageCallbackManagement::_callBackRequestGet = NULL;
    UartMessageCallbackManagement::CallBackResponseGet UartMessageCallbackManagement::_callBackResponseGet = NULL;
    UartMessageCallbackManagement::CallBackRequestSet UartMessageCallbackManagement::_callBackRequestSet = NULL;
    UartMessageCallbackManagement::CallBackSubscribe UartMessageCallbackManagement::_callBackSubscribe = NULL;
    UartMessageCallbackManagement::CallBackUnsubscribe UartMessageCallbackManagement::_callBackUnsubscribe = NULL;
    UartMessageCallbackManagement::CallBackNotification UartMessageCallbackManagement::_callBackNotification = NULL;
    UartMessageCallbackManagement::CallBackAcknowledge UartMessageCallbackManagement::_callBackAcknowledge = NULL;

    void UartMessageCallbackManagement::registerRequestGetCallBack(const CallBackRequestGet func)
    {
        _callBackRequestGet = func;
    }

    void UartMessageCallbackManagement::invokeRequestGetCallBack(uint32_t seqId, const RequestGetData *dataArr, size_t arrSize)
    {
        if(_callBackRequestGet == NULL) return;
        _callBackRequestGet(seqId, dataArr, arrSize);
    }

    void UartMessageCallbackManagement::registerResponseGetCallBack(const CallBackResponseGet func)
    {
        _callBackResponseGet = func;
    }

    void UartMessageCallbackManagement::invokeResponseGetCallBack(uint32_t seqId, const ResponseGetData *dataArr, size_t arrSize)
    {
        if(_callBackResponseGet == NULL) return;

        for(size_t arrIdx = 0 ; arrIdx < arrSize ; arrIdx++)
        {
            UartMessageInterface::readEndian((ResponseGetData *)(dataArr + arrIdx));
        }

        _callBackResponseGet(seqId, dataArr, arrSize);
    }

    void UartMessageCallbackManagement::registerNotificationCallBack(const CallBackNotification func)
    {
        _callBackNotification = func;
    }

    void UartMessageCallbackManagement::invokeNotificationCallBack(uint32_t seqId, const NotificationData *dataArr, size_t arrSize)
    {
        if(_callBackNotification == NULL) return;
        
        for(size_t arrIdx = 0 ; arrIdx < arrSize ; arrIdx++)
        {
            UartMessageInterface::readEndian((ResponseGetData *)(dataArr + arrIdx));
        }

        _callBackNotification(seqId, dataArr, arrSize);
    }

    void UartMessageCallbackManagement::registerSubscribeCallBack(const CallBackSubscribe func)
    {
        _callBackSubscribe = func;
    }

    void UartMessageCallbackManagement::invokeSubscribeCallBack(uint32_t seqId, const SubscribeData *dataArr, size_t arrSize)
    {
        if(_callBackSubscribe == NULL) return;
        _callBackSubscribe(seqId, dataArr, arrSize);
    }

    void UartMessageCallbackManagement::registerUnsubscribeCallBack(const CallBackUnsubscribe func)
    {
        _callBackUnsubscribe = func;
    }

    void UartMessageCallbackManagement::invokeUnsubscribeCallBack(uint32_t seqId, const UnsubscribeData *dataArr, size_t arrSize)
    {
        if(_callBackUnsubscribe == NULL) return;
        _callBackUnsubscribe(seqId, dataArr, arrSize);
    }

    void UartMessageCallbackManagement::registerRequestSetCallBack(const CallBackRequestSet func)
    {
        _callBackRequestSet = func;
    }

    void UartMessageCallbackManagement::invokeRequestSetCallBack(uint32_t seqId, const RequestSetData *dataArr, size_t arrSize)
    {
        if(_callBackRequestSet == NULL) return;
        
        for(size_t arrIdx = 0 ; arrIdx < arrSize ; arrIdx++)
        {
            UartMessageInterface::readEndian((ResponseGetData *)(dataArr + arrIdx));
        }

        _callBackRequestSet(seqId, dataArr, arrSize);
    }

    void UartMessageCallbackManagement::registerAcknowledgeCallBack(const CallBackAcknowledge func)
    {
        _callBackAcknowledge = func;
    }

    void UartMessageCallbackManagement::invokeAcknowledgeCallBack(uint32_t seqId, unsigned char msgId)
    {
        if(_callBackAcknowledge == NULL) return;
        _callBackAcknowledge(seqId, msgId);
    }

}; // namespace UartMessageInterface