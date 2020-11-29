#include "Arduino.h"
#include "UartMessageSender.h"

namespace UartMessageInterface
{
    UartMessageSender::UartMessageSender(eMessageType messageType, eCommandType commandType)
        : _seqId(0), _jsonDoc(256)
    {
        switch (messageType)
        {
        case Request:
            _jsonDoc["MsgType"] = "Req";
            break;
        case Response:
            _jsonDoc["MsgType"] = "Rsp";
            break;
        case Notification:
            _jsonDoc["MsgType"] = "Noti";
            break;
        case Acknowledge:
            _jsonDoc["MsgType"] = "Ack";
            break;
        default:
            // throw invalid_argument("Invalid MessageType");
            return;
        }

        switch (commandType)
        {
        case Get:
            _jsonDoc["Cmd"] = "Get";
            break;
        case Set:
            _jsonDoc["Cmd"] = "Set";
            break;
        case Subscribe:
            _jsonDoc["Cmd"] = "Subs";
            break;
        case Unsubscribe:
            _jsonDoc["Cmd"] = "Unsubs";
            break;
        default:
            // throw invalid_argument("Invalid CommandType");
            return;
        }
    }

    UartMessageSender::~UartMessageSender()
    {
        _jsonDoc.garbageCollect();
    }

    void UartMessageSender::appendRequestAll(eDataType type)
    {
        if (type != SensorAll || type != ControlAll)
            return;

        _jsonDoc.remove("Data");
        JsonArray dataArr = _jsonDoc.createNestedArray("Data");
        JsonObject data = dataArr.createNestedObject();

        if (type == SensorAll)
            data["Type"] = "SensorAll";
        else if (type == ControlAll)
            data["Type"] = "CtrlAll";
    }

    void UartMessageSender::appendRequest(eDataType dataType, const String &name)
    {
        if (!_jsonDoc.containsKey("Data"))
        {
            _jsonDoc.createNestedArray("Data");
        }

        JsonArray dataArr = _jsonDoc.getMember("Data");
        JsonObject data = dataArr.createNestedObject();
        switch (dataType)
        {
        case SensorTemperature:
            data["Type"] = "Temp";
            break;
        case SensorCO2:
            data["Type"] = "CO2";
            break;
        case SensorHumidity:
            data["Type"] = "Humid";
            break;
        case SensorConductivity:
            data["Type"] = "Conduct";
            break;
        case Control1:
            data["Type"] = "Control1";
            break;
        case Control2:
            data["Type"] = "Control2";
            break;
        case DateTime:
            data["Type"] = "DateTime";
            break;
        case SensorAll:
        case ControlAll:
            appendRequestAll(dataType);
            return;
        default:
            return;
        }
        data["Name"] = name;
    }

    void UartMessageSender::appendSubscribe(eDataType dataType, const String &name, unsigned int period)
    {
        if (!_jsonDoc.containsKey("Data"))
        {
            _jsonDoc.createNestedArray("Data");
        }

        JsonArray dataArr = _jsonDoc.getMember("Data");
        JsonObject data = dataArr.createNestedObject();
        switch (dataType)
        {
        case SensorTemperature:
            data["Type"] = "Temp";
            break;
        case SensorCO2:
            data["Type"] = "CO2";
            break;
        case SensorHumidity:
            data["Type"] = "Humid";
            break;
        case SensorConductivity:
            data["Type"] = "Conduct";
            break;
        case Control1:
            data["Type"] = "Control1";
            break;
        case Control2:
            data["Type"] = "Control2";
            break;
        case DateTime:
            data["Type"] = "DateTime";
            break;
        case SensorAll:
        case ControlAll:
            appendSubscribeAll(dataType, period);
            return;
        default:
            return;
        }
        data["Name"] = name;
        data["Period"] = period;
    }

    void UartMessageSender::appendSubscribeAll(eDataType dataType, unsigned int period)
    {
        if (dataType != SensorAll || dataType != ControlAll)
            return;

        _jsonDoc.remove("Data");
        JsonArray dataArr = _jsonDoc.createNestedArray("Data");
        JsonObject data = dataArr.createNestedObject();

        if (dataType == SensorAll)
            data["Type"] = "SensorAll";
        else if (dataType == ControlAll)
            data["Type"] = "CtrlAll";

        data["Period"] = period;        
    }

    void UartMessageSender::appendUnsubscribe(eDataType dataType, const String &name)
    {
        if (!_jsonDoc.containsKey("Data"))
        {
            _jsonDoc.createNestedArray("Data");
        }

        JsonArray dataArr = _jsonDoc.getMember("Data");
        JsonObject data = dataArr.createNestedObject();
        switch (dataType)
        {
        case SensorTemperature:
            data["Type"] = "Temp";
            break;
        case SensorCO2:
            data["Type"] = "CO2";
            break;
        case SensorHumidity:
            data["Type"] = "Humid";
            break;
        case SensorConductivity:
            data["Type"] = "Conduct";
            break;
        case Control1:
            data["Type"] = "Control1";
            break;
        case Control2:
            data["Type"] = "Control2";
            break;
        case DateTime:
            data["Type"] = "DateTime";
            break;
        case SensorAll:
        case ControlAll:
            appendUnsubscribeAll(dataType);
            return;
        default:
            return;
        }
        data["Name"] = name;
    }

    void UartMessageSender::appendUnsubscribeAll(eDataType dataType)
    {
        if (dataType != SensorAll || dataType != ControlAll)
            return;

        _jsonDoc.remove("Data");
        JsonArray dataArr = _jsonDoc.createNestedArray("Data");
        JsonObject data = dataArr.createNestedObject();

        if (dataType == SensorAll)
            data["Type"] = "SensorAll";
        else if (dataType == ControlAll)
            data["Type"] = "CtrlAll";
    }

        void UartMessageSender::appendAcknowledge(eDataType dataType, const String &name)
    {
        if (!_jsonDoc.containsKey("Data"))
        {
            _jsonDoc.createNestedArray("Data");
        }

        JsonArray dataArr = _jsonDoc.getMember("Data");
        JsonObject data = dataArr.createNestedObject();
        switch (dataType)
        {
        case SensorTemperature:
            data["Type"] = "Temp";
            break;
        case SensorCO2:
            data["Type"] = "CO2";
            break;
        case SensorHumidity:
            data["Type"] = "Humid";
            break;
        case SensorConductivity:
            data["Type"] = "Conduct";
            break;
        case Control1:
            data["Type"] = "Control1";
            break;
        case Control2:
            data["Type"] = "Control2";
            break;
        case DateTime:
            data["Type"] = "DateTime";
            break;
        default:
            return;
        }
        data["Name"] = name;
    }

    String UartMessageSender::sendMessageStr()
    {
        String buf;
        serializeJson(_jsonDoc, buf);
        appendCheckSum(buf);

        return buf;
        // Serial.println(buf.c_str());

        _jsonDoc.garbageCollect();
    }
    
    void UartMessageSender::sendMessage()
    {
        Serial.print("<BEGIN>");

        String buf;
        serializeJson(_jsonDoc, buf);
        appendCheckSum(buf);

        for(size_t offset = 0 ; offset < buf.length() ; )
        {
            size_t end = (offset + 64 <= buf.length()) ? offset + 64 : buf.length();
            Serial.print(buf.substring(offset, end));
            offset = end;
        }

        Serial.print("<END>\n");

        _jsonDoc.garbageCollect();
    }

}; // namespace UartMessageInterface