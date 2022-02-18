// @Author Honcy Ye
// @Email yeholdon@gmail.com
// @Description 单例的日志管理类，即后端，各线程通过Logger类生成
//              日志消息，再通过该类的output函数传入
#ifndef BASE_ASYNCLOGGING_H_
#define BASE_ASYNCLOGGING_H_

#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include "base/NonCopyable.h"
#include "base/LogStream.h"

class AsyncLogging : NonCopyable
{
public:
    AsyncLogging(const std::string &base_name, int flush_interval = 2);
    ~AsyncLogging()
    {
    }
    void Append(const char* logline, int len);
        void start()
    {
        running_ = true;
        up_thread_.reset(new std::thread(std::bind(&AsyncLogging::ThreadFunc, this)));
    }

    void stop()
    {
        running_ = false;
        // cond_.notify();
        // thread_.join();
    }
private:
    void ThreadFunc();
    typedef FixedBuffer<kLargeBuffer> Buffer;
    typedef std::vector<std::shared_ptr<Buffer>> BufferVector;
    typedef std::shared_ptr<Buffer> BufferPtr;
    const int flush_interval_;
    bool running_;
    std::string basename_;

    std::unique_ptr<std::thread> up_thread_;
    std::mutex mutex_;
    std::condition_variable cond_;

    BufferPtr current_buffer_; // 当前缓冲
    BufferPtr next_buffer_; // 预备缓冲
    BufferVector buffers_; // 待写入文件的已经填满的缓冲
};


#endif // BASE_ASYNCLOGGING_H_