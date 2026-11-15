#pragma once

#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/spdlog.h>
#include <string>
#include <memory>

class Logger
{
public:
    Logger(const std::string& name, const std::string& level)
        : logger_(name, std::make_shared<spdlog::sinks::ansicolor_stderr_sink_mt>())
    { 
        logger_.set_level(spdlog::level::from_str(level));
    }

    template <typename... Args>
    void trace(fmt::format_string<Args...> words, Args &&...args)
    {
        logger_.trace(words, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void debug(fmt::format_string<Args...> words, Args &&...args)
    {
        logger_.debug(words, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void info(fmt::format_string<Args...> words, Args &&...args)
    {
        logger_.info(words, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void warn(fmt::format_string<Args...> words, Args &&...args)
    {
        logger_.warn(words, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void error(fmt::format_string<Args...> words, Args &&...args)
    {
        logger_.error(words, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void critical(fmt::format_string<Args...> words, Args &&...args)
    {
        logger_.critical(words, std::forward<Args>(args)...);
    }

private:
    spdlog::logger logger_;
};
