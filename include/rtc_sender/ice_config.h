//
// Created by yhkim on 25. 7. 2.
//

#ifndef BA_GCS_RTC_SENDER_ICE_CONFIG_H
#define BA_GCS_RTC_SENDER_ICE_CONFIG_H

#include <string>

namespace rtc_sender {
    struct IceConfig {
        std::string turn_url;
        std::string turn_username;
        std::string turn_credential;

        IceConfig(const std::string &url, const std::string &username, const std::string &credential)
            : turn_url(url), turn_username(username), turn_credential(credential) {
        }
    };
} // namespace rtc_sender

#endif  // BA_GCS_RTC_SENDER_ICE_CONFIG_H
