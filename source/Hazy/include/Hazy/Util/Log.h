#pragma once
#include <spdlog/spdlog.h>
#include <hazy_pch.h>

namespace spdlog {
    class logger;
}

namespace Hazy {

    /**
     * @brief 日志等级
     */
    enum class LogLevel {
        trace,
        debug,
        info,
        warn,
        err,
        critical,
        off,
        n_levels
    };

    /**
     * @brief 日志类
     */
    class HAZY_API Logger {
    public:

        /**
         * @brief 设置日志等级
         * @param level     日志等级
         */
        static void setLevel(LogLevel level);

        /**
         * @brief 记录日志
         * @param level     日志等级
         * @param fmt       可变参数模板的格式字符串
         * @param args      可变参数
         * @note            使用std::call_once来确保日志系统只被初始化一次
         */
        template<typename... Args>
        static void Log(LogLevel level, fmt::format_string<Args...> fmt, Args&&... args) {
            init();
            s_logger->log(static_cast<spdlog::level::level_enum>(static_cast<int>(level)), fmt, std::forward<Args>(args)...);
        }

        /**
         * @brief 记录trace日志
         * @param fmt       可变参数模板的格式字符串
         * @param args      可变参数
         * @note            等同于Log(LogLevel::trace, fmt, std::forward<Args>(args)...)
         */
        template<typename... Args>
        static void LogTrace(fmt::format_string<Args...> fmt, Args&&... args) {
            Log(LogLevel::trace, fmt, std::forward<Args>(args)...);
        }

        /**
         * @brief 记录debug日志
         * @param fmt       可变参数模板的格式字符串
         * @param args      可变参数
         * @note            等同于Log(LogLevel::debug, fmt, std::forward<Args>(args)...)
         */
        template<typename... Args>
        static void LogDebug(fmt::format_string<Args...> fmt, Args&&... args) {
            Log(LogLevel::debug, fmt, std::forward<Args>(args)...);
        }

        /**
         * @brief 记录info日志
         * @param fmt       可变参数模板的格式字符串
         * @param args      可变参数
         * @note            等同于Log(LogLevel::info, fmt, std::forward<Args>(args)...)
         */
        template<typename... Args>
        static void LogInfo(fmt::format_string<Args...> fmt, Args&&... args) {
            Log(LogLevel::info, fmt, std::forward<Args>(args)...);
        }

        /**
         * @brief 记录warn日志
         * @param fmt       可变参数模板的格式字符串
         * @param args      可变参数
         * @note            等同于Log(LogLevel::warn, fmt, std::forward<Args>(args)...)
         */
        template<typename... Args>
        static void LogWarn(fmt::format_string<Args...> fmt, Args&&... args) {
            Log(LogLevel::warn, fmt, std::forward<Args>(args)...);
        }

        /**
         * @brief 记录error日志
         * @param fmt       可变参数模板的格式字符串
         * @param args      可变参数
         * @note            等同于Log(LogLevel::err, fmt, std::forward<Args>(args)...)
         */
        template<typename... Args>
        static void LogError(fmt::format_string<Args...> fmt, Args&&... args) {
            Log(LogLevel::err, fmt, std::forward<Args>(args)...);
        }


        /**
         * @brief 记录critical日志
         * @param fmt       可变参数模板的格式字符串
         * @param args      可变参数
         * @note            等同于Log(LogLevel::critical, fmt, std::forward<Args>(args)...)
         */
        template<typename... Args>
        static void LogCritical(fmt::format_string<Args...> fmt, Args&&... args) {
            Log(LogLevel::critical, fmt, std::forward<Args>(args)...);
        }

    private:
        static void init();

        static std::shared_ptr<spdlog::logger> s_logger;
        static LogLevel s_level;
        static std::once_flag s_initializedFlag;
    };

}