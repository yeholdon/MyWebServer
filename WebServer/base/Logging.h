// @Author Honcy Ye
// @Email yeholdon@gmail.com
// @Description To generate log messages, including a LogStream

#ifndef BASE_LOGGING_H_
#define BASE_LOGGING_H_

#include "base/LogStream.h"

class Logger
{
public:
    enum LogLevel
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    };
    Logger(const std::string &filename, size_t line);
    // Logger(const std::string &filename, int line, LogLevel level);
    // Logger(const std::string &filename, int line, LogLevel level, const char* func);
    // Logger(const std::string &filename, int line, bool toAbort);
    ~Logger();
    LogStream& stream() { return impl_.stream_; }

    static void set_log_file_name(const std::string &fileName)
    {
        log_file_name_ = fileName;
    }
    // 所有日志
    static std::string log_file_name()
    {
        return log_file_name_;
    }

private:
    class Impl
    {
    public:
        Impl(const std::string &filename, size_t line);
        void FormatTime();

        LogStream stream_;
        size_t line_;
        std::string basename_;
    };
    Impl impl_;
    static std::string log_file_name_;
};

#define LOG Logger(__FILE__, __LINE__).stream()

#endif // BASE_LOGGING_H_