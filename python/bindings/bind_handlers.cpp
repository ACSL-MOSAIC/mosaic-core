//
// Created by yhkim on 2026. 2. 4.
//

#include "bind_handlers.hpp"

#include "bind_data_channel.hpp"
#include "bind_media_track.hpp"

void bind_handlers(py::module_& m) {
    auto data_channel_module = m.def_submodule("data_channel", "Data channel handlers");
    auto media_track_module = m.def_submodule("media_track", "Media track handlers");

    bind_data_channel(data_channel_module);
    bind_media_track(media_track_module);
}