//
// Created by yhkim on 12/30/25.
//

#ifndef MOSAIC_AUTO_CONFIGURER_CONFIG_READER_FACTORY_H
#define MOSAIC_AUTO_CONFIGURER_CONFIG_READER_FACTORY_H

#include <memory>
#include <type_traits>

#include "i_config_reader.h"
#include "i_config_reader_factory.h"

namespace mosaic::auto_configurer {
template <typename T>
class ConfigReaderFactory : public IConfigReaderFactory {
    static_assert(std::is_base_of_v<IConfigReader, T>, "T must inherit from IConfigReader");

  public:
    bool IsSupportedExtension(const std::string& extension) override {
        const auto reader = CreateConfigReader();
        return reader->IsSupportedExtension(extension);
    }

    std::shared_ptr<IConfigReader> CreateConfigReader() override {
        return std::make_shared<T>();
    }
};
}  // namespace mosaic::auto_configurer

#endif  // MOSAIC_AUTO_CONFIGURER_CONFIG_READER_FACTORY_H
