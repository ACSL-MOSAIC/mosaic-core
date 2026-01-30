//
// Created by yhkim on 12/28/25.
//

#ifndef MOSAIC_AUTO_CONFIGURER_AUTO_CONFIGURER_H
#define MOSAIC_AUTO_CONFIGURER_AUTO_CONFIGURER_H

#include <memory>

#include <mosaic/auto_configurer/config_reader/i_config_reader.h>
#include <mosaic/auto_configurer/connector/i_configurable_connector.h>
#include <mosaic/handlers/data_channel/i_data_channel_handler.h>
#include <mosaic/handlers/media_track/i_media_track_handler.h>

namespace mosaic::auto_configurer {
class AutoConfigurer {
  public:
    AutoConfigurer() = default;

    virtual ~AutoConfigurer() = default;

    virtual void BeforeConfigure() {}

    void AutoConfigure(const std::string& config_file_path);

    virtual void AfterConfigure() {}

    std::shared_ptr<core::MosaicConnector> GetMosaicConnector() const {
        return mosaic_connector_;
    }

  protected:
    std::shared_ptr<core::MosaicConfig> mosaic_config_;
    std::shared_ptr<core::MosaicConnector> mosaic_connector_;
    std::vector<std::shared_ptr<IConfigurableConnector>> configurable_connectors_;
    std::unordered_map<std::string, std::shared_ptr<handlers::IDataChannelHandler>> dc_handler_map_;
    std::unordered_map<std::string, std::shared_ptr<handlers::IMediaTrackHandler>> mt_handler_map_;

  private:
    std::shared_ptr<IConfigReader> config_reader_;

    void ReadConfigs(const std::string& config_file_path);

    void CreateMosaicConnector();

    void ResolveConnectors();

    void ConfigureConnectors();
};
}  // namespace mosaic::auto_configurer

#endif  // MOSAIC_AUTO_CONFIGURER_AUTO_CONFIGURER_H
