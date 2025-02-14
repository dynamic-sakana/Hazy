#include "Hazy/Util/Log.h"
#include <hazy_pch.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Hazy {
    std::shared_ptr<spdlog::logger> Logger::s_logger;    // 声明变量
    LogLevel Logger::s_level = LogLevel::trace;          // 默认打印等级是trace
    std::once_flag Logger::s_initializedFlag;            // 声明初始化标志

    void Logger::init() {
        std::call_once(s_initializedFlag, (
            []() {
                spdlog::set_pattern("%^[%T] %n: %v%$");
                s_logger = spdlog::stdout_color_mt("Hazy");
                s_logger->set_level(spdlog::level::trace);
            }));
    }

    void Logger::setLevel(LogLevel level) {
        init();
        s_level = level;
        s_logger->set_level(static_cast<spdlog::level::level_enum>(static_cast<int>(level)));
    }
}

