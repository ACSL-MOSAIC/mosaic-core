//
// Created by yhkim on 12/28/25.
//

#ifndef MOSAIC_AUTO_CONFIGURER_CONFIG_READER_RESOLVER_HPP
#define MOSAIC_AUTO_CONFIGURER_CONFIG_READER_RESOLVER_HPP

#include <functional>
#include <memory>

#include "config_reader_factory.hpp"
#include "i_config_reader.hpp"

namespace mosaic::auto_configurer {
class ConfigReaderResolver {
  public:
    static ConfigReaderResolver& GetInstance();

    bool IsSupportedFile(const std::string& file_path);

    std::shared_ptr<IConfigReader> GetConfigReader(const std::string& file_path);

    template <typename T>
    void RegisterConfigReader() {
        static_assert(std::is_base_of_v<IConfigReader, T>, "T must inherit from IConfigReader");
        auto factory = std::make_shared<ConfigReaderFactory<T>>();
        factories_.push_back(factory);
    }

    void RegisterConfigReader(const std::shared_ptr<IConfigReaderFactory>& factory) {
        factories_.push_back(factory);
    }

    ConfigReaderResolver(const ConfigReaderResolver&) = delete;

    ConfigReaderResolver& operator=(const ConfigReaderResolver&) = delete;

  private:
    std::vector<std::shared_ptr<IConfigReaderFactory>> factories_;

    ConfigReaderResolver();
};
}  // namespace mosaic::auto_configurer

#endif  // MOSAIC_AUTO_CONFIGURER_CONFIG_READER_RESOLVER_HPP
