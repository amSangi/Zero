#pragma once

#include "core/ZeroBase.hpp"
#include <string_view>
#include <mutex>

#if LOGGING_ENABLED
#define LOG_FATAL(logger, title, message) logger.Log(Logger::Level::LEVEL_FATAL, title, message)
#define LOG_ERROR(logger, title, message) logger.Log(Logger::Level::LEVEL_ERROR, title, message)
#define LOG_WARN(logger, title, message) logger.Log(Logger::Level::LEVEL_WARN, title, message)
#define LOG_DEBUG(logger, title, message) logger.Log(Logger::Level::LEVEL_DEBUG, title, message)
#define LOG_VERBOSE(logger, title, message) logger.Log(Logger::Level::LEVEL_VERBOSE, title, message)
#else
#define LOG_FATAL(logger, title, message)
#define LOG_ERROR(logger, title, message)
#define LOG_WARN(logger, title, message)
#define LOG_DEBUG(logger, title, message)
#define LOG_VERBOSE(logger, title, message)
#endif

namespace zero
{

    /**
     * @brief Log different types of messages with different levels
     * The logs are outputted to the console
     */
    class Logger
    {
    public:

        /**
         * @brief The severity of a log message.
         *
         * The levels move from the most severe, `LEVEL_FATAL`, to the least severe, `LEVEL_VERBOSE`.
         */
        enum class Level
        {
            LEVEL_FATAL = 0,      ///< Very severe error events that may cause the application to terminate.
            LEVEL_ERROR = 1,      ///< Error events that may prevent normal program execution.
            LEVEL_WARN = 2,       ///< Harmful situations that may indicate potential problems with the application.
            LEVEL_DEBUG = 3,      ///< Broad information about a subsystem. E.g. Number of entities that will be rendered.
            LEVEL_VERBOSE = 4,    ///< Highly detailed tracing of program execution. E.g. Calling an API.
        }; // enum class Level

        Logger();
        ~Logger() = default;

        /**
         * @brief Set the log filter
         *
         * This will filter out all logs that are not as severe as the given level.
         * For example, if the filter is `LEVEL_DEBUG`, all `LEVEL_VERBOSE` logs will be filtered out.
         *
         * @param level the minimum severity level to log
         */
        void SetFilter(Level level);

        /**
         * @brief Get the severity level
         * @return the severity level
         */
        [[nodiscard]] Level GetFilter();

        /**
         * @brief Log a message
         * @param level the log severity
         * @param title the title of the log (e.g. 'RenderSystem')
         * @param message the message to log
         */
        void Log(Level level, std::string_view title, std::string_view message);

    private:
        Level severity_filter_;
        std::mutex mutex_;

    }; // class Logger

} // namespace zero