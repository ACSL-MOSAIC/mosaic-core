//
// Created by yhkim on 12/30/25.
//

#ifndef MOSAIC_AUTO_CONFIGURER_I_CONFIG_READER_FACTORY_H
#define MOSAIC_AUTO_CONFIGURER_I_CONFIG_READER_FACTORY_H

#include <memory>
#include <string>

#include "i_config_reader.h"

namespace mosaic::auto_configurer {
class IConfigReaderFactory {
  public:
    virtual ~IConfigReaderFactory() = default;
    virtual bool IsSupportedExtension(const std::string& extension) = 0;
    virtual std::shared_ptr<IConfigReader> CreateConfigReader() = 0;
};
}  // namespace mosaic::auto_configurer

#endif  // MOSAIC_AUTO_CONFIGURER_I_CONFIG_READER_FACTORY_H
