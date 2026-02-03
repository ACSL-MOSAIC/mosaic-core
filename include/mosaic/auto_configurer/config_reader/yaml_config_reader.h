//
// Created by yhkim on 12/28/25.
//

#ifndef MOSAIC_AUTO_CONFIGURER_YAML_CONFIG_READER_H
#define MOSAIC_AUTO_CONFIGURER_YAML_CONFIG_READER_H

#include <memory>

#include "i_config_reader.h"

namespace mosaic::auto_configurer {
class YamlConfigReader : public IConfigReader {
  public:
    YamlConfigReader();

    ~YamlConfigReader() override;

    bool IsSupportedExtension(const std::string& extension) override;

    core::MosaicConfig LoadConfigs(const std::string& file_path) override;

  private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};
}  // namespace mosaic::auto_configurer

#endif  // MOSAIC_AUTO_CONFIGURER_YAML_CONFIG_READER_H
