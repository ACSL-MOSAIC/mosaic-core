//
// Created by yhkim on 12/28/25.
//

#include "mosaic/auto_configurer/config_reader/config_reader_resolver.h"

#include <filesystem>
#include <stdexcept>

#include "mosaic/auto_configurer/config_reader/yaml_config_reader.h"

using namespace mosaic::auto_configurer;

ConfigReaderResolver& ConfigReaderResolver::GetInstance() {
    static ConfigReaderResolver instance;
    return instance;
}

std::string ExtractFileExtension(const std::string& file_path) {
    const std::filesystem::path path(file_path);
    std::string extension = path.extension().string();

    if (extension.empty()) {
        return "";
    }

    // Remove the leading dot from extension (e.g., ".yaml" -> "yaml")
    if (extension[0] == '.') {
        extension = extension.substr(1);
    }

    return extension;
}

bool ConfigReaderResolver::IsSupportedFile(const std::string& file_path) {
    const std::string extension = ExtractFileExtension(file_path);

    if (extension.empty()) {
        return false;
    }

    for (const auto& factory : factories_) {
        if (factory && factory->IsSupportedExtension(extension)) {
            return true;
        }
    }

    return false;
}

std::shared_ptr<IConfigReader> ConfigReaderResolver::GetConfigReader(const std::string& file_path) {
    const std::string extension = ExtractFileExtension(file_path);

    if (extension.empty()) {
        throw std::invalid_argument("File has no extension: " + file_path);
    }

    for (const auto& factory : factories_) {
        if (factory && factory->IsSupportedExtension(extension)) {
            return factory->CreateConfigReader();
        }
    }

    throw std::invalid_argument("Unsupported file extension: " + extension);
}

ConfigReaderResolver::ConfigReaderResolver() {
    factories_.push_back(std::make_shared<ConfigReaderFactory<YamlConfigReader>>());
}