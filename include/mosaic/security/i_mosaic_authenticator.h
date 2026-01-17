//
// Created by yhkim on 12/25/25.
//

#ifndef MOSAIC_SECURITY_I_MOSAIC_AUTHENTICATOR_H
#define MOSAIC_SECURITY_I_MOSAIC_AUTHENTICATOR_H

namespace mosaic::security {
class IMosaicAuthenticator {
  public:
    virtual ~IMosaicAuthenticator() = default;

    virtual bool IsAuthenticated() = 0;
};
}  // namespace mosaic::security

#endif  // MOSAIC_SECURITY_I_MOSAIC_AUTHENTICATOR_H
