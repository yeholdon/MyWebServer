// @Author Honcy Ye
// @Email yeholdon@gmail.com

#include <time.h>  
#include <sys/time.h> 
#include <string>
#include <iostream>
#include "base/Logging.h"
#include "base/AsyncLogging.h"

// pthread_once 确保AsyncLogging只初始化一次，线程安全的单例
static pthread_once_t once_control_ = PTHREAD_ONCE_INIT;
static AsyncLogging *AsyncLogger_;

std::string Logger::log_file_name_ = "linya_WebServer.log";

void once_init()
{
    AsyncLogger_ = new AsyncLogging(Logger::log_file_name());
    AsyncLogger_->start(); 
}

void output(const char* msg, int len)
{
    pthread_once(&once_control_, once_init);
    //std::cout << std::string(msg) << std::endl;
    AsyncLogger_->Append(msg, len);
}

Logger::Impl::Impl(const std::string &filename, size_t line)
    : stream_(),
    line_(line),
    basename_(filename)
{
    FormatTime();
}

void Logger::Impl::FormatTime()
{
    struct timeval tv;
    time_t time;
    char str_t[26] = {0};
    ::gettimeofday(&tv, NULL);
    time = tv.tv_sec;
    struct tm* p_time = localtime(&time);   
    strftime(str_t, 26, "%Y-%m-%d %H:%M:%S\n", p_time);
    stream_ << str_t;
}

Logger::Logger(const std::string &filename, size_t line)
  : impl_(filename, line)
{ }

Logger::~Logger()
{
    impl_.stream_ << " -- " << impl_.basename_ << ':' << impl_.line_ << '\n';
    const LogStream::Buffer& buf(stream().buffer());
    output(buf.data(), buf.length());
}