//
// Created by yhkim on 1/19/26.
//

#ifndef MOSAIC_AUTO_CONFIGURER_A_PARALLEL_DC_HANDLER_CONFIGURER_H
#define MOSAIC_AUTO_CONFIGURER_A_PARALLEL_DC_HANDLER_CONFIGURER_H

#include "i_configurable_connector.h"

namespace mosaic::auto_configurer {
class AParallelDCHandlerConfigurer : public IConfigurableConnector {
  public:
    AParallelDCHandlerConfigurer() : IConfigurableConnector() {}

    virtual std::string GetConnectorType() const override = 0;

    virtual void Configure() override = 0;

    std::vector<std::shared_ptr<handlers::IDataChannelHandler>> GetHandlers() {
        return handlers_;
    }

  protected:
    std::vector<std::shared_ptr<handlers::IDataChannelHandler>> handlers_;
};
}  // namespace mosaic::auto_configurer

#endif  // MOSAIC_AUTO_CONFIGURER_A_PARALLEL_DC_HANDLER_CONFIGURER_H
