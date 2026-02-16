//
// Created by yhkim on 25. 7. 2.
//

#ifndef MOSAIC_CORE_LOGGER_MACROS_HPP
#define MOSAIC_CORE_LOGGER_MACROS_HPP

#include "i_logger.hpp"
#include "log_service.hpp"

namespace mosaic::core_log {
#define MOSAIC_LOG_DEBUG(...) mosaic::core_log::GetLogger()->LOG_DEBUG(__VA_ARGS__)

#define MOSAIC_LOG_WARNING(...) mosaic::core_log::GetLogger()->LOG_WARNING(__VA_ARGS__)

#define MOSAIC_LOG_INFO(...) mosaic::core_log::GetLogger()->LOG_INFO(__VA_ARGS__)

#define MOSAIC_LOG_ERROR(...) mosaic::core_log::GetLogger()->LOG_ERROR(__VA_ARGS__)
}  // namespace mosaic::core_log

#endif  // MOSAIC_CORE_LOGGER_MACROS_HPP
