// @Author Honcy Ye
// @Email yeholdon@gmail.com
// @Description 进一步封装FileAppend，实现定次数和定时写文件

#include <memory>
#include <string>
#include <mutex>

#include "base/FileUtil.h"
#include "base/NonCopyable.h"


// TODO 提供自动归档功能
class LogFile : NonCopyable
{
public:
    // 每被append flushEveryN次，flush一下，会往文件写，只不过，文件也是带缓冲区的
    LogFile(const std::string& basename, int flush_everyN = 1024);
    ~LogFile();

    void Append(const char* logline, int len);
    void Flush();
    bool RollFile();

private:
    void append_unlocked(const char* logline, int len);

    const std::string basename_;
    const int flush_everyN_;

    int count_;
    std::mutex mutex_;
    std::unique_ptr<AppendFile> file_;
};