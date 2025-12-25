//
// Created by yhkim on 25. 7. 3.
//

#include <rtc_sender/logger/log_service.h>
#include <rtc_sender/logger/console_logger.h>

using namespace rtc_sender::log;

std::shared_ptr<ILogger> LogService::DefaultLogger() {
    return std::make_shared<ConsoleLogger>();
}
