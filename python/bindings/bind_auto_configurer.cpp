//
// Created by yhkim on 2026. 2. 4.
//

#include "bind_auto_configurer.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <pybind11/stl.h>

#include "mosaic/auto_configurer/auto_configurer.h"
#include "mosaic/auto_configurer/connector/a_dc_handler_configurer.h"
#include "mosaic/auto_configurer/connector/a_mt_handler_configurer.h"
#include "mosaic/auto_configurer/connector/a_parallel_dc_handler_configurer.h"
#include "mosaic/auto_configurer/connector/configurable_connector_factory.h"
#include "mosaic/auto_configurer/connector/connector_resolver.h"

namespace {

using namespace mosaic::auto_configurer;
using namespace mosaic::handlers;
using namespace mosaic::core;

// --- Trampoline: IConfigurableConnector ---
// GetConnectorType (pure virtual), ValidateConfig (default empty), Configure (pure virtual)
// All three methods dispatch to snake_case; methods with empty C++ default fallback to no-op lambda
class PyIConfigurableConnector : public IConfigurableConnector {
  public:
    using IConfigurableConnector::IConfigurableConnector;

    [[nodiscard]] std::string GetConnectorType() const override {
        py::gil_scoped_acquire gil;
        return py::cast(this).attr("get_connector_type")().cast<std::string>();
    }

    void ValidateConfig() override {
        py::gil_scoped_acquire gil;
        py::cast(this).attr("validate_config")();
    }

    void Configure() override {
        py::gil_scoped_acquire gil;
        py::cast(this).attr("configure")();
    }
};

// --- Trampoline: IConfigurableConnectorFactory ---
// GetConnectorType, CreateConnector are both pure virtual → dispatch to snake_case
class PyIConfigurableConnectorFactory : public IConfigurableConnectorFactory {
  public:
    using IConfigurableConnectorFactory::IConfigurableConnectorFactory;

    std::string GetConnectorType() override {
        PYBIND11_OVERRIDE_PURE_NAME(std::string,                    // Return type
                                    IConfigurableConnectorFactory,  // Parent class
                                    "get_connector_type",           // Python method name
                                    GetConnectorType                // C++ function name
        );
    }

    std::shared_ptr<IConfigurableConnector> CreateConnector() override {
        PYBIND11_OVERRIDE_PURE_NAME(std::shared_ptr<IConfigurableConnector>,  // Return type
                                    IConfigurableConnectorFactory,            // Parent class
                                    "create_connector",                       // Python method name
                                    CreateConnector                           // C++ function name
        );
    }
};

// --- Trampoline: ADCHandlerConfigurer ---
class PyADCHandlerConfigurer : public ADCHandlerConfigurer {
  public:
    using ADCHandlerConfigurer::ADCHandlerConfigurer;

    [[nodiscard]] std::string GetConnectorType() const override {
        py::gil_scoped_acquire gil;
        return py::cast(this).attr("get_connector_type")().cast<std::string>();
    }

    void ValidateConfig() override {
        py::gil_scoped_acquire gil;
        py::cast(this).attr("validate_config")();
    }

    void Configure() override {
        py::gil_scoped_acquire gil;
        py::cast(this).attr("configure")();
    }

    void SetHandler(const std::shared_ptr<IDataChannelHandler>& h) {
        handler_ = h;
    }
};

// --- Trampoline: AMTHandlerConfigurer ---
class PyAMTHandlerConfigurer : public AMTHandlerConfigurer {
  public:
    using AMTHandlerConfigurer::AMTHandlerConfigurer;

    [[nodiscard]] std::string GetConnectorType() const override {
        py::gil_scoped_acquire gil;
        return py::cast(this).attr("get_connector_type")().cast<std::string>();
    }

    void ValidateConfig() override {
        py::gil_scoped_acquire gil;
        py::cast(this).attr("validate_config")();
    }

    void Configure() override {
        py::gil_scoped_acquire gil;
        py::cast(this).attr("configure")();
    }

    void SetHandler(const std::shared_ptr<IMediaTrackHandler>& h) {
        handler_ = h;
    }
};

// --- Trampoline: AParallelDCHandlerConfigurer ---
class PyAParallelDCHandlerConfigurer : public AParallelDCHandlerConfigurer {
  public:
    using AParallelDCHandlerConfigurer::AParallelDCHandlerConfigurer;

    [[nodiscard]] std::string GetConnectorType() const override {
        py::gil_scoped_acquire gil;
        return py::cast(this).attr("get_connector_type")().cast<std::string>();
    }

    void ValidateConfig() override {
        py::gil_scoped_acquire gil;
        py::cast(this).attr("validate_config")();
    }

    void Configure() override {
        py::gil_scoped_acquire gil;
        py::cast(this).attr("configure")();
    }

    void SetHandlers(const std::vector<std::shared_ptr<IDataChannelHandler>>& hs) {
        handlers_ = hs;
    }
};

// --- Trampoline: AutoConfigurer ---
// BeforeConfigure, AfterConfigure는 기본 빈 본문 → no-op lambda로 폴백
class PyAutoConfigurer : public AutoConfigurer {
  public:
    using AutoConfigurer::AutoConfigurer;

    void BeforeConfigure() override {
        py::gil_scoped_acquire gil;
        py::cast(this).attr("before_configure")();
    }

    void AfterConfigure() override {
        py::gil_scoped_acquire gil;
        py::cast(this).attr("after_configure")();
    }

    // Protected member accessors — called only from pybind11 property/method lambdas
    std::shared_ptr<MosaicConfig> GetMosaicConfig() const {
        return mosaic_config_;
    }

    void AddConnector(const std::shared_ptr<IConfigurableConnector>& c) {
        configurable_connectors_.push_back(c);
    }

    std::vector<std::shared_ptr<IConfigurableConnector>> GetConnectors() const {
        return configurable_connectors_;
    }

    std::unordered_map<std::string, std::shared_ptr<IDataChannelHandler>> GetDcHandlerMap() const {
        return dc_handler_map_;
    }

    std::unordered_map<std::string, std::shared_ptr<IMediaTrackHandler>> GetMtHandlerMap() const {
        return mt_handler_map_;
    }
};

}  // namespace

void bind_auto_configurer(py::module_& m) {
    using namespace mosaic::auto_configurer;

    // IConfigurableConnector (abstract — pure virtual get_connector_type, configure)
    py::class_<IConfigurableConnector, std::shared_ptr<IConfigurableConnector>, PyIConfigurableConnector>(
        m, "IConfigurableConnector")
        .def(py::init<>())
        .def("get_connector_type", [](IConfigurableConnector&) -> std::string { return {}; })
        .def("set_config",
             [](IConfigurableConnector& self, const ConnectorConfig& config) {
                 auto config_ptr = std::make_shared<ConnectorConfig>(config);
                 self.SetConfig(config_ptr);
             })
        .def("get_config",
             [](const IConfigurableConnector& self) -> ConnectorConfig {
                 auto config_ptr = self.GetConfig();
                 return config_ptr ? *config_ptr : ConnectorConfig{};
             })
        .def("validate_config", [](IConfigurableConnector&) {})
        .def("configure", [](IConfigurableConnector&) {});

    // IConfigurableConnectorFactory (abstract — pure virtual get_connector_type, create_connector)
    // Register with ConnectorResolver to automatically create connectors during auto_configure
    py::class_<IConfigurableConnectorFactory,
               std::shared_ptr<IConfigurableConnectorFactory>,
               PyIConfigurableConnectorFactory>(m, "IConfigurableConnectorFactory")
        .def(py::init<>())
        .def("get_connector_type", [](IConfigurableConnectorFactory&) -> std::string { return {}; })
        .def("create_connector",
             [](IConfigurableConnectorFactory&) -> std::shared_ptr<IConfigurableConnector> { return nullptr; });

    // ADCHandlerConfigurer (abstract, inherits IConfigurableConnector)
    // handler property: set via self.handler = <handler> in Python subclass's configure()
    py::class_<ADCHandlerConfigurer,
               std::shared_ptr<ADCHandlerConfigurer>,
               IConfigurableConnector,
               PyADCHandlerConfigurer>(m, "ADCHandlerConfigurer")
        .def(py::init<>())
        .def("get_connector_type", [](ADCHandlerConfigurer&) -> std::string { return {}; })
        .def("validate_config", [](ADCHandlerConfigurer&) {})
        .def("configure", [](ADCHandlerConfigurer&) {})
        .def("get_handler", &ADCHandlerConfigurer::GetHandler)
        .def(
            "set_handler",
            [](ADCHandlerConfigurer& self, const std::shared_ptr<IDataChannelHandler>& h) {
                dynamic_cast<PyADCHandlerConfigurer&>(self).SetHandler(h);
            },
            "Set the data channel handler (use this in Python configure() method)");

    // AMTHandlerConfigurer (abstract, inherits IConfigurableConnector)
    py::class_<AMTHandlerConfigurer,
               std::shared_ptr<AMTHandlerConfigurer>,
               IConfigurableConnector,
               PyAMTHandlerConfigurer>(m, "AMTHandlerConfigurer")
        .def(py::init<>())
        .def("get_connector_type", [](AMTHandlerConfigurer&) -> std::string { return {}; })
        .def("validate_config", [](AMTHandlerConfigurer&) {})
        .def("configure", [](AMTHandlerConfigurer&) {})
        .def("get_handler", &AMTHandlerConfigurer::GetHandler)
        .def(
            "set_handler",
            [](AMTHandlerConfigurer& self, const std::shared_ptr<IMediaTrackHandler>& h) {
                dynamic_cast<PyAMTHandlerConfigurer&>(self).SetHandler(h);
            },
            "Set the media track handler (use this in Python configure() method)");

    // AParallelDCHandlerConfigurer (abstract, inherits IConfigurableConnector)
    py::class_<AParallelDCHandlerConfigurer,
               std::shared_ptr<AParallelDCHandlerConfigurer>,
               IConfigurableConnector,
               PyAParallelDCHandlerConfigurer>(m, "AParallelDCHandlerConfigurer")
        .def(py::init<>())
        .def("get_connector_type", [](AParallelDCHandlerConfigurer&) -> std::string { return {}; })
        .def("validate_config", [](AParallelDCHandlerConfigurer&) {})
        .def("configure", [](AParallelDCHandlerConfigurer&) {})
        .def("get_handlers", &AParallelDCHandlerConfigurer::GetHandlers)
        .def(
            "set_handlers",
            [](AParallelDCHandlerConfigurer& self, const std::vector<std::shared_ptr<IDataChannelHandler>>& hs) {
                dynamic_cast<PyAParallelDCHandlerConfigurer&>(self).SetHandlers(hs);
            },
            "Set the data channel handlers (use this in Python configure() method)");

    // AutoConfigurer (Python subclasses can override before_configure / after_configure)
    py::class_<AutoConfigurer, PyAutoConfigurer>(m, "AutoConfigurer")
        .def(py::init<>())
        .def("auto_configure",
             [](AutoConfigurer& self, const std::string& config_file_path) {
                 // Check if Python subclass

                 if (auto* py_self = dynamic_cast<PyAutoConfigurer*>(&self)) {
                     py_self->ReadConfigs(config_file_path);
                     {
                         py::gil_scoped_release release;
                         py_self->CreateMosaicConnector();
                     }
                     py_self->ResolveConnectors();
                     py_self->BeforeConfigure();
                     py_self->ConfigureConnectors();
                     py_self->AfterConfigure();
                 } else {
                     // C++ object: just call directly
                     self.AutoConfigure(config_file_path);
                 }
             })
        .def("before_configure", [](AutoConfigurer&) {})
        .def("after_configure", [](AutoConfigurer&) {})
        .def("get_mosaic_connector", &AutoConfigurer::GetMosaicConnector)
        .def_property_readonly(
            "mosaic_config",
            [](AutoConfigurer& self) { return dynamic_cast<PyAutoConfigurer&>(self).GetMosaicConfig(); })
        .def_property_readonly(
            "configurable_connectors",
            [](AutoConfigurer& self) { return dynamic_cast<PyAutoConfigurer&>(self).GetConnectors(); })
        .def("add_configurable_connector",
             [](AutoConfigurer& self, const std::shared_ptr<IConfigurableConnector>& c) {
                 dynamic_cast<PyAutoConfigurer&>(self).AddConnector(c);
             })
        .def_property_readonly(
            "dc_handler_map",
            [](AutoConfigurer& self) { return dynamic_cast<PyAutoConfigurer&>(self).GetDcHandlerMap(); })
        .def_property_readonly("mt_handler_map", [](AutoConfigurer& self) {
            return dynamic_cast<PyAutoConfigurer&>(self).GetMtHandlerMap();
        });

    // ConnectorResolver singleton — free functions
    m.def(
        "register_configurable_connector",
        [](const std::shared_ptr<IConfigurableConnectorFactory>& factory) {
            ConnectorResolver::GetInstance().RegisterConfigurableConnector(factory);
        },
        "Register connector factory with ConnectorResolver to auto-create connectors during auto_configure");

    m.def(
        "is_supported_connector_type",
        [](const std::string& connector_type) -> bool {
            return ConnectorResolver::GetInstance().IsSupportedType(connector_type);
        },
        "Check if connector type is registered");

    m.def(
        "get_configurable_connector",
        [](const std::string& connector_type) -> std::shared_ptr<IConfigurableConnector> {
            return ConnectorResolver::GetInstance().GetConfigurableConnector(connector_type);
        },
        "Create IConfigurableConnector instance for the given registered connector type");
}