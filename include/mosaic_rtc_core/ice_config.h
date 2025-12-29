//
// Created by yhkim on 25. 7. 2.
//

#ifndef MOSAIC_RTC_CORE_ICE_CONFIG_H
#define MOSAIC_RTC_CORE_ICE_CONFIG_H

#include <string>
#include <utility>

namespace mosaic::core {
struct IceConfig {
    std::string turn_url;
    std::string turn_username;
    std::string turn_credential;

    IceConfig(std::string url, std::string username, std::string credential)
        : turn_url(std::move(url)), turn_username(std::move(username)), turn_credential(std::move(credential)) {}
};
}  // namespace mosaic::core

#endif  // MOSAIC_RTC_CORE_ICE_CONFIG_H
