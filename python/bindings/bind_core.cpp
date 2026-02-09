//
// Created by yhkim on 2026. 2. 4.
//

#include "bind_core.h"

#include <memory>

#include "mosaic/core/mosaic_connector.h"
#include "mosaic/core/mosaic_connector_factory.h"

void bind_core(py::module_& m) {
    using namespace mosaic::core;

    // shared_ptr holder: returned as shared_ptr<MosaicConnector> from MosaicConnectorFactory::Create()
    // Constructor not exposed — can only be created via Factory
    py::class_<MosaicConnector, std::shared_ptr<MosaicConnector>>(m, "MosaicConnector")
        .def("initialize_webrtc", &MosaicConnector::InitializeWebRTC)
        .def("add_data_channel_handler", &MosaicConnector::AddDataChannelHandler)
        .def("add_media_track_handler", &MosaicConnector::AddMediaTrackHandler)
        .def("shutting_down", &MosaicConnector::ShuttingDown);

    py::class_<MosaicConnectorFactory>(m, "MosaicConnectorFactory")
        .def(py::init<const std::shared_ptr<ServerConfig>&>())
        .def("create", &MosaicConnectorFactory::Create);
}