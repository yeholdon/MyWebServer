// @Author Honcy Ye
// @Email yeholdon@gmail.com

#include <assert.h>
#include <cstdio>
#include <string>

#include "base/FileUtil.h"


AppendFile::AppendFile(const std::string &filename)
    : fp_(nullptr)
{
    //printf("%s\n", filename.c_str());
    fp_ = fopen(filename.c_str(), "a+");
    assert(fp_ != nullptr);
    // 把文件流和缓冲区关联
    ::setbuffer(fp_, buffer_, sizeof(buffer_));
}

AppendFile::~AppendFile()
{
    ::fclose(fp_);
}

void AppendFile::Append(const char* log_line, const size_t len)
{
    size_t n = this->Write(log_line, len);
    size_t remain = len - n;
    while (remain > 0)
    {
        size_t x = this->Write(log_line + n, remain);
        if (x == 0)
        {
            int err = ferror(fp_);
            if (err)
                fprintf(stderr, "AppendFile::append() failed !\n");
            break;
        }
        n += x;
        remain = len - n;
    }
}

void AppendFile::Flush()
{
    fflush(fp_);
}

size_t AppendFile::Write(const char* log_line, const size_t len)
{
    return fwrite_unlocked(log_line, 1, len, fp_);
}
