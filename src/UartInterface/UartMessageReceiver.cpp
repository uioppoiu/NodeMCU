#include "UartMessageReceiver.h"

namespace UartMessageInterface
{
    UartMessageReceiver::UartMessageReceiver(const String &message)
        : _jsonDoc(256)
    {
        if (isUartMessage(message) == false)
        {
            // throw invalid_argument("This message is not Uart message");
            return;
        }

        if (verityCheckSum(message) == false)
        {
            // throw invalid_argument("CheckSum error");
            return;
        }

        deserializeJson(_jsonDoc, message.c_str(), message.length() - 1);
    }

    UartMessageReceiver::~UartMessageReceiver()
    {
        _jsonDoc.garbageCollect();
    }

    bool UartMessageReceiver::isMessageValid()
    {
        return true; // TODO 내용 검사
    }

    bool UartMessageReceiver::isUartMessage(const String &message)
    {
        return true; // TODO
    }

    void UartMessageReceiver::processMessage()
    {
        if (!_jsonDoc.containsKey("MsgType"))
        {
            return;
        }

        const char *sMessageType = _jsonDoc["MsgType"];

        if (String(sMessageType).equals("Req"))
        {
            handleRequestMessage();
        }
        else if ((String(sMessageType).equals("Rsp")) ||
                 (String(sMessageType).equals("Noti")))
        {
            handleResponseMessage();
        }
    }

    void UartMessageReceiver::handleRequestMessage()
    {
        if (!_jsonDoc.containsKey("Cmd"))
        {
            return;
        }
        const char *sCommandType = _jsonDoc["Cmd"];

        if (!_jsonDoc.containsKey("Data"))
        {
            return;
        }
        JsonArrayConst datas = _jsonDoc["Data"];

        if (String(sCommandType).equals("Get"))
        {
            for (JsonArrayConst::iterator iter = datas.begin(); iter != datas.end(); ++iter)
            {
                invokeRequestGet(iter->as<JsonObjectConst>());
            }
        }
        else if (String(sCommandType).equals("Subs"))
        {
            for (JsonArrayConst::iterator iter = datas.begin(); iter != datas.end(); ++iter)
            {
                invokeSubscribe(*iter);
            }
        }
        else if (String(sCommandType).equals("Unsubs"))
        {
            for (JsonArrayConst::iterator iter = datas.begin(); iter != datas.end(); ++iter)
            {
                invokeUnsubscribe(*iter);
            }
        }
    }

    void UartMessageReceiver::invokeRequestGet(JsonObjectConst data)
    {
        eDataType dataType = str2EnumDataType(data["Type"]);
        const char *name = data["Name"];
        UartMessageCallbackManagement::invokeRequestGetCallBack(dataType, name);
    }

    void UartMessageReceiver::invokeSubscribe(JsonObjectConst data)
    {
        eDataType dataType = str2EnumDataType(data["Type"]);
        const char *name = data["Name"];
        unsigned int period = data["Period"];
        UartMessageCallbackManagement::invokeSubscribeCallBack(dataType, name, period);
    }

    void UartMessageReceiver::invokeUnsubscribe(JsonObjectConst data)
    {
        eDataType dataType = str2EnumDataType(data["Type"]);
        const char *name = data["Name"];
        UartMessageCallbackManagement::invokeUnsubscribeCallBack(dataType, name);
    }

    void UartMessageReceiver::handleResponseMessage()
    {
        if (!_jsonDoc.containsKey("Cmd"))
            return;
        const char *sCommandType = _jsonDoc["Cmd"];

        if (!_jsonDoc.containsKey("Data"))
            return;
        JsonArrayConst datas = _jsonDoc["Data"];

        if (String(sCommandType).equals("Get"))
        {
            for (JsonArrayConst::iterator iter = datas.begin(); iter != datas.end(); ++iter)
            {
                invokeResponseGet(*iter);
            }
        }
    }

    void UartMessageReceiver::invokeResponseGet(JsonObjectConst data)
    {
        eDataType dataType = str2EnumDataType(data["Type"]);
        const char *name = data["Name"];
        const char *sValueType = data["ValType"];
        eValueType valueType = (String(sValueType).equals("Float")) ? Float : Integer;

        Value v;
        if (valueType == Float)
        {
            v.type = Float;
            v.value.val_float = data["Val"];
        }
        else
        {
            v.type = Integer;
            v.value.val_int = data["Val"];
        }

        UartMessageCallbackManagement::invokeResponseGetCallBack(dataType, name, v);
    }
}; // namespace UartMessageInterface