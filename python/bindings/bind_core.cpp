//
// Created by yhkim on 2026. 2. 4.
//

#include "bind_core.h"

#include <memory>

#include "mosaic/core/mosaic_connector.h"
#include "mosaic/core/mosaic_connector_factory.h"

void bind_core(py::module_& m) {
    using namespace mosaic::core;

    // shared_ptr holder: MosaicConnectorFactory::Create()에서 shared_ptr<MosaicConnector>로 반환됨
    // 생성자 비노출 — Factory를 통해만 생성 가능
    py::class_<MosaicConnector, std::shared_ptr<MosaicConnector>>(m, "MosaicConnector")
        .def("initialize_webrtc", &MosaicConnector::InitializeWebRTC)
        .def("add_data_channel_handler", &MosaicConnector::AddDataChannelHandler)
        .def("add_media_track_handler", &MosaicConnector::AddMediaTrackHandler)
        .def("shutting_down", &MosaicConnector::ShuttingDown);

    py::class_<MosaicConnectorFactory>(m, "MosaicConnectorFactory")
        .def(py::init<const std::shared_ptr<ServerConfig>&>())
        .def("create", &MosaicConnectorFactory::Create);
}