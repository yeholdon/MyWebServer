// @Author Honcy Ye
// @Email yeholdon@gmail.com
#ifndef BASE_FILE_UTIL_H_
#define BASE_FILE_UTIL_H_

#include <string>

#include "base/NonCopyable.h"

class AppendFile : NonCopyable
{
public:
    // 单参数构造函数，显式声明，避免隐式转换
    explicit AppendFile(const std::string &filename);
    ~AppendFile();
    // 
    void Append(const char* log_line, const size_t len);
    void Flush();
private:
    size_t Write(const char* log_line, const size_t len);
    FILE *fp_;
    char buffer_[64*1024];
};

#endif // BASE_FILE_UTIL_H_