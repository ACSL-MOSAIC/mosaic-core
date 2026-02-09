//
// Created by yhkim on 2026. 2. 4.
//

#ifndef MOSAIC_CORE_BIND_CONFIGS_H
#define MOSAIC_CORE_BIND_CONFIGS_H

#include <pybind11/pybind11.h>

namespace py = pybind11;

void bind_configs(py::module_& m);

#endif  // MOSAIC_CORE_BIND_CONFIGS_H