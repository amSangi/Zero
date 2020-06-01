#include "core/Logger.hpp"
#include <ctime>
#include <iostream>

namespace zero
{

constexpr const char* LogLevelAsString(Logger::Level level)
{
    switch (level)
    {
        case Logger::Level::LEVEL_FATAL:
            return "FATAL";
        case Logger::Level::LEVEL_ERROR:
            return "ERROR";
        case Logger::Level::LEVEL_WARN:
            return "WARN";
        case Logger::Level::LEVEL_DEBUG:
            return "DEBUG";
        default:
            return "VERBOSE";
    }
}

Logger::Logger()
: severity_filter_(Level::LEVEL_DEBUG)
{
}

void Logger::SetFilter(Level level)
{
    severity_filter_ = level;
}

Logger::Level Logger::GetFilter() const
{
    return severity_filter_;
}

void Logger::Log(Level level, std::string_view title, std::string_view message) const
{
    if (static_cast<int32_t>(level) > static_cast<int32_t>(severity_filter_))
    {
        return;
    }

    time_t raw_time{};
    char buffer[64] = { 0 };
    time(&raw_time);
    tm* time_info = localtime(&raw_time);
    strftime(buffer, sizeof(buffer), "[%Y-%m-%dT%H:%M:%S]", time_info);

    std::ostream* out = &std::cout;
    switch (level)
    {
        case Level::LEVEL_FATAL:
        case Level::LEVEL_ERROR:
            out = &std::cerr;
            break;
        default:
            break;
    }

    (*out) << buffer
           << "[" << LogLevelAsString(level) << "]"
           << "[" << title << "] " << message << std::endl;
}

}

