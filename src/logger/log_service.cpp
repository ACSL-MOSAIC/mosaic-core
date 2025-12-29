//
// Created by yhkim on 25. 7. 3.
//

#include <mosaic_rtc_core/logger/console_logger.h>
#include <mosaic_rtc_core/logger/log_service.h>

using namespace mosaic::core_log;

std::shared_ptr<ILogger> LogService::DefaultLogger() {
    return std::make_shared<ConsoleLogger>();
}
