//
// Created by yhkim on 7/17/25.
//

#include <api/data_channel_interface.h>
#include <json/reader.h>
#include <json/value.h>
#include <mosaic/handlers/data_channel/data_channel_receivable.h>

using namespace mosaic::handlers;

Json::Value IDataChannelReceivable::ConvertDataBufferToJson(const webrtc::DataBuffer& buffer) {
    Json::Value json_data;
    const Json::CharReaderBuilder reader_builder;
    std::string errors;
    if (std::istringstream message_stream(std::string(buffer.data.data<char>(), buffer.data.size()));
        !Json::parseFromStream(reader_builder, message_stream, &json_data, &errors)) {
        MOSAIC_LOG_ERROR("Failed to parse DataBuffer to JSON: {}", errors);
        throw std::runtime_error("JSON parsing failed: " + errors);
    }
    return json_data;
}
