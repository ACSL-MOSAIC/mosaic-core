//
// Created by yhkim on 25. 7. 3.
//

#include "mosaic/logger/log_service.hpp"

#include "mosaic/logger/console_logger.hpp"

using namespace mosaic::core_log;

std::shared_ptr<ILogger> LogService::DefaultLogger() {
    return std::make_shared<ConsoleLogger>();
}
