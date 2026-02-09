//
// Created by yhkim on 2026. 2. 4.
//

#include "bind_data_channel.h"

#include <memory>

#include <json/json.h>
#include <pybind11/stl.h>

#include "mosaic/handlers/data_channel/data_channel_receivable.h"
#include "mosaic/handlers/data_channel/data_channel_sendable.h"

// Json::Value ↔ Python object type caster
// pybind11::cast is shadowed by the class member cast inside type_caster,
// so conversion logic is implemented as helper functions outside the class
namespace pybind11 {
namespace detail {

static object json_to_pyobj(const Json::Value& json) {
    switch (json.type()) {
        case Json::nullValue:
            return none();
        case Json::booleanValue:
            return cast(json.asBool());
        case Json::intValue:
            return cast(json.asInt64());
        case Json::uintValue:
            return cast(json.asUInt64());
        case Json::realValue:
            return cast(json.asDouble());
        case Json::stringValue:
            return cast(json.asString());
        case Json::arrayValue: {
            list arr;
            for (const auto& item : json)
                arr.append(json_to_pyobj(item));
            return arr;
        }
        case Json::objectValue: {
            dict obj;
            for (auto it = json.begin(); it != json.end(); ++it)
                obj[cast(it.name())] = json_to_pyobj(*it);
            return obj;
        }
        default:
            return none();
    }
}

static Json::Value pyobj_to_json(handle src) {
    if (src.is_none())
        return {};
    if (isinstance<bool_>(src))
        return {src.cast<bool>()};
    if (isinstance<int_>(src))
        return {src.cast<Json::Int64>()};
    if (isinstance<float_>(src))
        return {src.cast<double>()};
    if (isinstance<str>(src))
        return {src.cast<std::string>()};
    if (isinstance<list>(src)) {
        Json::Value arr(Json::arrayValue);
        for (auto item : src.cast<list>())
            arr.append(pyobj_to_json(item));
        return arr;
    }
    if (isinstance<dict>(src)) {
        Json::Value obj(Json::objectValue);
        for (auto& [key, val] : src.cast<dict>())
            obj[key.cast<std::string>()] = pyobj_to_json(val);
        return obj;
    }
    return {};
}

template <>
struct type_caster<Json::Value> {
  public:
    PYBIND11_TYPE_CASTER(Json::Value, const_name("dict"));

    // Python dict → Json::Value
    bool load(handle src, bool) {
        if (!isinstance<dict>(src))
            return false;
        value = Json::Value(Json::objectValue);
        for (auto& [key, val] : src.cast<dict>())
            value[key.cast<std::string>()] = pyobj_to_json(val);
        return true;
    }

    // Json::Value → Python dict
    static handle cast(const Json::Value& src, return_value_policy, handle) {
        dict result;
        for (auto it = src.begin(); it != src.end(); ++it)
            result[str(it.name())] = json_to_pyobj(*it);
        return result.release();
    }
};

}  // namespace detail
}  // namespace pybind11

namespace {

using namespace mosaic::handlers;

// Trampolines: Dispatch C++ HandleData calls to Python's handle_data
// No PYBIND11_OVERRIDE fallback needed since C++ default implementation is empty
class PyDataChannelStringReceivable : public DataChannelStringReceivable {
  public:
    using DataChannelStringReceivable::DataChannelStringReceivable;

    void HandleData(const std::string& data) override {
        py::gil_scoped_acquire gil;
        py::cast(this).attr("handle_data")(data);
    }
};

class PyDataChannelJsonReceivable : public DataChannelJsonReceivable {
  public:
    using DataChannelJsonReceivable::DataChannelJsonReceivable;

    void HandleData(const Json::Value& data) override {
        py::gil_scoped_acquire gil;
        py::cast(this).attr("handle_data")(data);
    }
};

class PyDataChannelByteReceivable : public DataChannelByteReceivable {
  public:
    using DataChannelByteReceivable::DataChannelByteReceivable;

    void HandleData(const std::vector<uint8_t>& data) override {
        py::gil_scoped_acquire gil;
        py::cast(this).attr("handle_data")(data);
    }
};

}  // namespace

void bind_data_channel(py::module_& m) {
    using namespace mosaic::handlers;

    // ChannelState enum
    py::enum_<IDataChannelHandler::ChannelState>(m, "ChannelState")
        .value("CONNECTING", IDataChannelHandler::kConnecting)
        .value("OPEN", IDataChannelHandler::kOpen)
        .value("CLOSING", IDataChannelHandler::kClosing)
        .value("CLOSED", IDataChannelHandler::kClosed)
        .value("UNKNOWN", IDataChannelHandler::kUnknown);

    // Base classes (no constructor - for inheritance chain registration)
    // shared_ptr holder: passed as shared_ptr<IDataChannelHandler> in MosaicConnector::AddDataChannelHandler
    py::class_<IDataChannelHandler, std::shared_ptr<IDataChannelHandler>>(m, "IDataChannelHandler");

    py::class_<ADataChannelHandler, std::shared_ptr<ADataChannelHandler>, IDataChannelHandler>(m, "ADataChannelHandler")
        .def("get_label", &ADataChannelHandler::GetLabel)
        .def("get_state", &ADataChannelHandler::GetState)
        .def("close", &ADataChannelHandler::CloseDataChannel);

    // Receivable classes (Python subclasses can override handle_data)
    py::class_<DataChannelStringReceivable, std::shared_ptr<DataChannelStringReceivable>, ADataChannelHandler, PyDataChannelStringReceivable>(
        m, "DataChannelStringReceivable")
        .def(py::init<const std::string&>())
        .def("handle_data", [](DataChannelStringReceivable&, py::object) {});

    py::class_<DataChannelJsonReceivable, std::shared_ptr<DataChannelJsonReceivable>, ADataChannelHandler, PyDataChannelJsonReceivable>(m,
                                                                                            "DataChannelJsonReceivable")
        .def(py::init<const std::string&>())
        .def("handle_data", [](DataChannelJsonReceivable&, py::object) {});

    py::class_<DataChannelByteReceivable, std::shared_ptr<DataChannelByteReceivable>, ADataChannelHandler, PyDataChannelByteReceivable>(m,
                                                                                            "DataChannelByteReceivable")
        .def(py::init<const std::string&>())
        .def("handle_data", [](DataChannelByteReceivable&, py::object) {});

    // DataChannelSendable (Send(DataBuffer) is excluded as it uses WebRTC internal types)
    py::class_<DataChannelSendable, std::shared_ptr<DataChannelSendable>, ADataChannelHandler>(m, "DataChannelSendable")
        .def(py::init<const std::string&>())
        .def("sendable", &DataChannelSendable::Sendable)
        .def("send_string", &DataChannelSendable::SendString, py::arg("message"), py::arg("is_async") = false)
        .def("send_string_as_byte",
             &DataChannelSendable::SendStringAsByte,
             py::arg("message"),
             py::arg("is_async") = false)
        .def("send_json", &DataChannelSendable::SendJson, py::arg("data"), py::arg("is_async") = false);
}