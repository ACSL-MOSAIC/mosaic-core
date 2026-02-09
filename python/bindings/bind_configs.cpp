//
// Created by yhkim on 2026. 2. 4.
//

#include "bind_configs.h"

#include <memory>

#include <pybind11/stl.h>

#include "mosaic/configs_decl.h"

void bind_configs(py::module_& m) {
    using namespace mosaic::core;

    py::class_<IceServerConfig>(m, "IceServerConfig")
        .def(py::init<>())
        .def_readwrite("urls", &IceServerConfig::urls)
        .def_readwrite("username", &IceServerConfig::username)
        .def_readwrite("credential", &IceServerConfig::credential);

    py::class_<WebRtcConfig>(m, "WebRtcConfig")
        .def(py::init<>())
        .def_readwrite("ice_servers", &WebRtcConfig::ice_servers);

    py::class_<AuthConfig>(m, "AuthConfig")
        .def(py::init<>())
        .def_readwrite("type", &AuthConfig::type)
        .def_readwrite("robot_id", &AuthConfig::robot_id)
        .def_readwrite("params", &AuthConfig::params);

    py::class_<ConnectorConfig>(m, "ConnectorConfig")
        .def(py::init<>())
        .def_readwrite("type", &ConnectorConfig::type)
        .def_readwrite("label", &ConnectorConfig::label)
        .def_readwrite("params", &ConnectorConfig::params);

    // shared_ptr holder: passed as shared_ptr<ServerConfig> in MosaicConnectorFactory constructor
    py::class_<ServerConfig, std::shared_ptr<ServerConfig>>(m, "ServerConfig")
        .def(py::init<>())
        .def_readwrite("ws_url", &ServerConfig::ws_url)
        .def_readwrite("auth_config", &ServerConfig::auth_config)
        .def_readwrite("webrtc_config", &ServerConfig::webrtc_config)
        .def("get_signaling_url", &ServerConfig::GetSignalingUrl);

    // shared_ptr holder: returned as shared_ptr<MosaicConfig> from AutoConfigurer::mosaic_config_
    py::class_<MosaicConfig, std::shared_ptr<MosaicConfig>>(m, "MosaicConfig")
        .def(py::init<>())
        .def_readwrite("server", &MosaicConfig::server)
        .def_readwrite("connectors", &MosaicConfig::connectors);
}