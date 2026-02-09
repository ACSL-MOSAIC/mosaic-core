//
// Created by yhkim on 2026. 2. 4.
//

#include <pybind11/pybind11.h>

#include "bind_auto_configurer.h"
#include "bind_configs.h"
#include "bind_core.h"
#include "bind_handlers.h"
#include "bind_logging.h"

PYBIND11_MODULE(_mosaic_core, m) {
    m.doc() = "MOSAIC core Python bindings";

    auto logging_module = m.def_submodule("logging", "Logging functionality");
    auto configs_module = m.def_submodule("configs", "Configuration structs");
    auto handlers_module = m.def_submodule("handlers", "Handlers functionality");
    auto core_module = m.def_submodule("core", "Core functionality");
    auto auto_configurer_module = m.def_submodule("auto_configurer", "Auto configurer functionality");

    bind_logging(logging_module);
    bind_configs(configs_module);
    bind_handlers(handlers_module);
    bind_core(core_module);
    bind_auto_configurer(auto_configurer_module);
}
