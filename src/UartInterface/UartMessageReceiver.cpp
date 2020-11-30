#include "UartMessageReceiver.h"

namespace UartMessageInterface
{
    UartMessageReceiver::UartMessageReceiver(const String &message)
        : _messageBuffer(message)
    {
        // if (verityCheckSum(message) == false) throw;
    }

    UartMessageReceiver::~UartMessageReceiver()
    {
    }

    void UartMessageReceiver::processMessage()
    {
        const MsgCommonHeader *msgHeader = (const MsgCommonHeader *)_messageBuffer.begin();
        const unsigned char msgId = msgHeader->msgId; // 메시지 ID
        const uint32_t seqId = msgHeader->seqId;
        const unsigned char numOfData = msgHeader->numOfData; // 데이터의 수

        switch (msgId)
        {
        case MsgId::RequestGet:
        {
            const RequestGetData *dataArr = (const RequestGetData *)(_messageBuffer.begin() + sizeof(MsgCommonHeader));
            UartMessageCallbackManagement::invokeRequestGetCallBack(seqId, dataArr, numOfData);
        }
        break;
        case MsgId::RequestSet:
        {
            const RequestSetData *dataArr = (const RequestSetData *)(_messageBuffer.begin() + sizeof(MsgCommonHeader));
            UartMessageCallbackManagement::invokeRequestSetCallBack(seqId, dataArr, numOfData);
        }
        break;
        case MsgId::Subscribe:
        {
            const SubscribeData *dataArr = (const SubscribeData *)(_messageBuffer.begin() + sizeof(MsgCommonHeader));
            UartMessageCallbackManagement::invokeSubscribeCallBack(seqId, dataArr, numOfData);
        }
        break;
        case MsgId::Unsubscribe:
        {
            const UnsubscribeData *dataArr = (const UnsubscribeData *)(_messageBuffer.begin() + sizeof(MsgCommonHeader));
            UartMessageCallbackManagement::invokeUnsubscribeCallBack(seqId, dataArr, numOfData);
        }
        break;
        case MsgId::Notification:
        {
            const NotificationData *dataArr = (const NotificationData *)(_messageBuffer.begin() + sizeof(MsgCommonHeader));
            UartMessageCallbackManagement::invokeNotificationCallBack(seqId, dataArr, numOfData);
        }
        break;
        case MsgId::ResponseGet:
        {
            const ResponseGetData *dataArr = (const ResponseGetData *)(_messageBuffer.begin() + sizeof(MsgCommonHeader));
            UartMessageCallbackManagement::invokeResponseGetCallBack(seqId, dataArr, numOfData);
        }
        break;
        case MsgId::Acknowledge:
        {
            UartMessageCallbackManagement::invokeAcknowledgeCallBack(seqId, msgId);
        }
        break;
        default:
            return;
        }
    }
}; // namespace UartMessageInterface