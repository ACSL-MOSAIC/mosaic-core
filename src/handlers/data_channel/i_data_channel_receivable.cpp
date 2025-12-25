//
// Created by yhkim on 7/17/25.
//

#include <rtc_sender/handlers/data_channel/i_data_channel_receivable.h>

#include <api/data_channel_interface.h>

#include <json/reader.h>
#include <json/value.h>

using namespace rtc_sender::handlers;

Json::Value IDataChannelReceivable::ConvertDataBufferToJson(const webrtc::DataBuffer &buffer) {
    Json::Value json_data;
    const Json::CharReaderBuilder reader_builder;
    std::string errors;
    if (std::istringstream message_stream(std::string(buffer.data.data<char>(), buffer.data.size())); !
        Json::parseFromStream(reader_builder, message_stream, &json_data, &errors)) {
        RTC_SENDER_LOG_ERROR("Failed to parse DataBuffer to JSON: {}", errors);
        throw std::runtime_error("JSON parsing failed: " + errors);
    }
    return json_data;
}
