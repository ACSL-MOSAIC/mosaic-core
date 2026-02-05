//
// Created by yhkim on 2026. 2. 4.
//

#ifndef MOSAIC_CORE_BIND_LOGGING_H
#define MOSAIC_CORE_BIND_LOGGING_H

#include <pybind11/pybind11.h>

namespace py = pybind11;

void bind_logging(py::module_& m);

#endif  // MOSAIC_CORE_BIND_LOGGING_H
