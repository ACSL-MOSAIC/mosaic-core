//
// Created by yhkim on 12/31/25.
//

#ifndef MOSAIC_AUTO_CONFIGURER_A_MT_HANDLER_CONFIGURER_H
#define MOSAIC_AUTO_CONFIGURER_A_MT_HANDLER_CONFIGURER_H

#include "i_configurable_connector.h"

namespace mosaic::auto_configurer {
class AMTHandlerConfigurer : public IConfigurableConnector {
  public:
    AMTHandlerConfigurer() = default;

    virtual std::string GetConnectorType() const override = 0;

    virtual void Configure(std::shared_ptr<core::MosaicConnector> mosaic_container) override = 0;

    std::shared_ptr<handlers::IMediaTrackHandler> GetHandler() {
        return handler_;
    }

  protected:
    std::shared_ptr<handlers::IMediaTrackHandler> handler_;
};
}  // namespace mosaic::auto_configurer

#endif  // MOSAIC_AUTO_CONFIGURER_A_MT_HANDLER_CONFIGURER_H
