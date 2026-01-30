//
// Created by yhkim on 1/30/26.
//

#ifndef MOSAIC_CORE_AUTH_TYPE_H
#define MOSAIC_CORE_AUTH_TYPE_H

#include <string>

namespace mosaic::security {
using AuthType = std::string;

class AuthTypeConst {
  public:
    inline static const AuthType NO_AUTHORIZATION = "no-authorization";
    inline static const AuthType SIMPLE_TOKEN = "simple-token";
};
}  // namespace mosaic::security

#endif  // MOSAIC_CORE_AUTH_TYPE_H
