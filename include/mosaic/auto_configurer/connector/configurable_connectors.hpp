//
// Created by yhkim on 12/28/25.
//

#ifndef MOSAIC_AUTO_CONFIGURER_CONFIGURABLE_CONNECTOR_HPP
#define MOSAIC_AUTO_CONFIGURER_CONFIGURABLE_CONNECTOR_HPP

#include "../../configs_decl.hpp"
#include "../../core/mosaic_connector.hpp"

namespace mosaic::auto_configurer {
class IConfigurableConnector {
  public:
    virtual ~IConfigurableConnector() = default;

    virtual std::string GetConnectorType() const = 0;

    void SetConfig(const std::shared_ptr<core::ConnectorConfig>& connector_config) {
        connector_config_ = connector_config;
    }

    virtual void ValidateConfig() {}

    std::shared_ptr<core::ConnectorConfig> GetConfig() const {
        return connector_config_;
    }

    virtual void Configure() = 0;

  protected:
    std::shared_ptr<core::ConnectorConfig> connector_config_;
};

class ADCHandlerConfigurer : public IConfigurableConnector {
  public:
    ADCHandlerConfigurer() : IConfigurableConnector() {}

    virtual std::string GetConnectorType() const override = 0;

    virtual void Configure() override = 0;

    std::shared_ptr<handlers::IDataChannelHandler> GetHandler() {
        return handler_;
    }

  protected:
    std::shared_ptr<handlers::IDataChannelHandler> handler_;
};

class AMultipleDCHandlerConfigurer : public IConfigurableConnector {
  public:
    AMultipleDCHandlerConfigurer() : IConfigurableConnector() {}

    virtual std::string GetConnectorType() const override = 0;

    virtual void Configure() override = 0;

    std::vector<std::shared_ptr<handlers::IDataChannelHandler>> GetHandlers() {
        return handlers_;
    }

  protected:
    std::vector<std::shared_ptr<handlers::IDataChannelHandler>> handlers_;
};

class AMTHandlerConfigurer : public IConfigurableConnector {
  public:
    AMTHandlerConfigurer() = default;

    virtual std::string GetConnectorType() const override = 0;

    virtual void Configure() override = 0;

    std::shared_ptr<handlers::IMediaTrackHandler> GetHandler() {
        return handler_;
    }

  protected:
    std::shared_ptr<handlers::IMediaTrackHandler> handler_;
};
}  // namespace mosaic::auto_configurer

#endif  // MOSAIC_AUTO_CONFIGURER_CONFIGURABLE_CONNECTOR_HPP
