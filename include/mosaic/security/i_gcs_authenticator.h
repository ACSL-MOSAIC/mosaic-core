//
// Created by yhkim on 12/25/25.
//

#ifndef MOSAIC_RTC_CORE_I_GCS_AUTHENTICATOR_H
#define MOSAIC_RTC_CORE_I_GCS_AUTHENTICATOR_H

namespace mosaic::security {
class IGCSAuthenticator {
  public:
    virtual ~IGCSAuthenticator() = default;

    virtual bool IsAuthenticated() = 0;
};
}  // namespace mosaic::security

#endif  // MOSAIC_RTC_CORE_I_GCS_AUTHENTICATOR_H
