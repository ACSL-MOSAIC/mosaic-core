//
// Created by yhkim on 25. 7. 1.
//

#include "rtc_sender/handlers/data_channel/data_channel_sendable.h"

#include <json/json.h>

#include "api/data_channel_interface.h"
#include "rtc_sender/logger/log.h"

using namespace rtc_sender::handlers;

bool DataChannelSendable::Sendable() const {
    return GetState() == kOpen;
}

void DataChannelSendable::Send(const webrtc::DataBuffer &buffer) const {
    if (!Sendable()) {
        RTC_SENDER_LOG_ERROR("DataChannel is not open, cannot send data... Ignoring send request.");
        return;
    }
    ADataChannelHandler::Send(buffer);
}

void DataChannelSendable::SendString(const std::string &string) const {
    if (!Sendable()) {
        RTC_SENDER_LOG_ERROR("DataChannel is not open, cannot send data... Ignoring send request.");
        return;
    }
    webrtc::DataBuffer buffer(rtc::CopyOnWriteBuffer(string), false);
    Send(buffer);
}

void DataChannelSendable::SendStringAsByte(const std::string &string) const {
    if (!Sendable()) {
        RTC_SENDER_LOG_ERROR("DataChannel is not open, cannot send data... Ignoring send request.");
        return;
    }
    webrtc::DataBuffer buffer(rtc::CopyOnWriteBuffer(string), true);
    Send(buffer);
}

void DataChannelSendable::SendJson(const Json::Value &json_data) const {
    if (!Sendable()) {
        RTC_SENDER_LOG_ERROR("DataChannel is not open, cannot send data... Ignoring send request.");
        return;
    }
    const Json::StreamWriterBuilder writer;
    const std::string json_string = Json::writeString(writer, json_data);
    const webrtc::DataBuffer buffer(rtc::CopyOnWriteBuffer(json_string), false);
    Send(buffer);
}
