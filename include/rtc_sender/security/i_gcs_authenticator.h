//
// Created by yhkim on 12/25/25.
//

#ifndef BA_GCS_RTC_SENDER_I_GCS_AUTHENTICATOR_H
#define BA_GCS_RTC_SENDER_I_GCS_AUTHENTICATOR_H

namespace rtc_sender::security {
    class IGCSAuthenticator {
    public:
        virtual ~IGCSAuthenticator() = default;

        virtual bool IsAuthenticated() = 0;
    };
} // namespace rtc_sender::security

#endif //BA_GCS_RTC_SENDER_I_GCS_AUTHENTICATOR_H
