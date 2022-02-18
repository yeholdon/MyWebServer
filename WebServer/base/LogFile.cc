// @Author Honcy Ye
// @Email yeholdon@gmail.com
#include <string>
#include "base/LogFile.h"
#include "base/FileUtil.h"

LogFile::LogFile(const std::string& basename, int flush_everyN)
    : basename_(basename),
    flush_everyN_(flush_everyN),
    file_(new AppendFile(basename)),
    count_(0)
{
}

LogFile::~LogFile()
{ }

void LogFile::Append(const char* logline, int len)
{
    std::unique_lock<std::mutex> lock(mutex_);
    append_unlocked(logline, len);
}

void LogFile::Flush()
{
    std::unique_lock<std::mutex> lock(mutex_);
    file_->Flush();
}

void LogFile::append_unlocked(const char* logline, int len)
{
    file_->Append(logline, len);
    ++count_;
    if (count_ >= flush_everyN_)
    {
        count_ = 0;
        file_->Flush();
    }
}