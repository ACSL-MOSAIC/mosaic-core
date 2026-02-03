//
// Created by yhkim on 12/28/25.
//

#ifndef MOSAIC_AUTO_CONFIGURER_I_CONFIG_READER_H
#define MOSAIC_AUTO_CONFIGURER_I_CONFIG_READER_H

#include <filesystem>

#include <mosaic/configs_decl.h>

namespace mosaic::auto_configurer {
class IConfigReader {
  public:
    virtual ~IConfigReader() = default;

    virtual bool IsSupportedExtension(const std::string& extension) = 0;

    virtual core::MosaicConfig LoadConfigs(const std::string& file_path) = 0;
};
}  // namespace mosaic::auto_configurer

#endif  // MOSAIC_AUTO_CONFIGURER_I_CONFIG_READER_H
