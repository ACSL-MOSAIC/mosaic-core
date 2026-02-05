//
// Created by yhkim on 2026. 2. 4.
//

#include "bind_logging.h"

#include <pybind11/stl.h>

#include "mosaic/logger/i_logger.h"
#include "mosaic/logger/webrtc_log.h"

namespace {

// Trampoline: Python 서브클래스에서 LOG()를 오버라이드할 수 있도록 필요
class PyILogger : public mosaic::core_log::ILogger {
  public:
    using ILogger::ILogger;

    void LOG(const std::string& message, mosaic::core_log::LogLevel log_level) override {
        PYBIND11_OVERRIDE_PURE(void, mosaic::core_log::ILogger, log, message, log_level);
    }
};

}  // namespace

void bind_logging(py::module_& m) {
    // LogLevel enum
    py::enum_<mosaic::core_log::LogLevel>(m, "LogLevel")
        .value("DEBUG", mosaic::core_log::DEBUG)
        .value("WARNING", mosaic::core_log::WARNING)
        .value("INFO", mosaic::core_log::INFO)
        .value("ERROR", mosaic::core_log::ERROR);

    // WebRTCLogLevel enum
    py::enum_<mosaic::core_log::WebRTCLogLevel>(m, "WebRTCLogLevel")
        .value("LS_VERBOSE", mosaic::core_log::LS_VERBOSE)
        .value("LS_INFO", mosaic::core_log::LS_INFO)
        .value("LS_WARNING", mosaic::core_log::LS_WARNING)
        .value("LS_ERROR", mosaic::core_log::LS_ERROR)
        .value("LS_NONE", mosaic::core_log::LS_NONE);

    // ILogger 추상 클래스 (PyILogger 트램폴린으로 Python 서브클래스 가능)
    py::class_<mosaic::core_log::ILogger, std::shared_ptr<mosaic::core_log::ILogger>, PyILogger>(m, "ILogger")
        .def("log", &mosaic::core_log::ILogger::LOG)
        .def("log_debug",
             static_cast<void (mosaic::core_log::ILogger::*)(const std::string&)>(&mosaic::core_log::ILogger::LOG_DEBUG))
        .def("log_warning",
             static_cast<void (mosaic::core_log::ILogger::*)(const std::string&)>(
                 &mosaic::core_log::ILogger::LOG_WARNING))
        .def("log_info",
             static_cast<void (mosaic::core_log::ILogger::*)(const std::string&)>(&mosaic::core_log::ILogger::LOG_INFO))
        .def("log_error",
             static_cast<void (mosaic::core_log::ILogger::*)(const std::string&)>(&mosaic::core_log::ILogger::LOG_ERROR))
        .def_static("get_log_level", &mosaic::core_log::ILogger::GetLogLevel)
        .def_static("is_debug_enabled", &mosaic::core_log::ILogger::IsDebugEnabled)
        .def_static("is_info_enabled", &mosaic::core_log::ILogger::IsInfoEnabled)
        .def_static("is_warning_enabled", &mosaic::core_log::ILogger::IsWarningEnabled)
        .def_static("is_error_enabled", &mosaic::core_log::ILogger::IsErrorEnabled);

    // LogService 자유함수 (LogService 자체는 생성자·소멸자가 private인 singleton이므로 py::class_ 바인딩 불가)
    m.def("get_logger", &mosaic::core_log::GetLogger);
    m.def("set_log_level", &mosaic::core_log::SetLogLevel);
    m.def("get_log_level", &mosaic::core_log::GetLogLevel);
    // RegisterLogger<Impl>은 템플릿이므로 shared_ptr 기반 set_logger로 대체
    m.def("set_logger", [](std::shared_ptr<mosaic::core_log::ILogger> logger) {
        mosaic::core_log::LogService::GetInstance().logger_ = std::move(logger);
    });

    // WebRTC 로깅 설정 함수
    m.def("set_webrtc_log_level", &mosaic::core_log::SetWebRTCLogLevel);
    m.def("set_webrtc_log_timestamps", &mosaic::core_log::SetWebRTCLogTimestamps);
    m.def("set_webrtc_log_threads", &mosaic::core_log::SetWebRTCLogThreads);
}