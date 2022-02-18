// @Author Honcy Ye
// @Email yeholdon@gmail.com

#include <assert.h>
#include <functional>
#include <chrono>

#include "base/AsyncLogging.h"
#include "base/LogFile.h"

AsyncLogging::AsyncLogging(const std::string &base_name, int flush_interval)
  : flush_interval_(flush_interval),
    running_(false),
    basename_(base_name),
    up_thread_(),
    mutex_(),
    cond_(),
    current_buffer_(new Buffer),
    next_buffer_(new Buffer),
    buffers_()
{
    assert(basename_.size() > 1);
    current_buffer_->bzero();
    next_buffer_->bzero();
    buffers_.reserve(16);
}

void AsyncLogging::Append(const char* logline, int len)
{
    std::unique_lock<std::mutex> lock(mutex_);
    if (current_buffer_->Avail() > len)
        current_buffer_->Append(logline, len);
    else
    {
        buffers_.push_back(current_buffer_);
        current_buffer_.reset();
        if (next_buffer_)
            current_buffer_ = std::move(next_buffer_);
        else
            current_buffer_.reset(new Buffer);
        current_buffer_->Append(logline, len);
        cond_.notify_one();
    }
}

void AsyncLogging::ThreadFunc()
{
    assert(running_ == true);
    LogFile output(basename_);
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    newBuffer1->bzero();
    newBuffer2->bzero();
    BufferVector buffersToWrite;
    buffersToWrite.reserve(16);
    while (running_)
    {
        assert(newBuffer1 && newBuffer1->length() == 0);
        assert(newBuffer2 && newBuffer2->length() == 0);
        assert(buffersToWrite.empty());

        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (buffers_.empty())  // unusual usage!
            {
                cond_.wait_for(lock, std::chrono::seconds(flush_interval_));
            }
            buffers_.push_back(current_buffer_);
            current_buffer_.reset();

            current_buffer_ = std::move(newBuffer1);
            buffersToWrite.swap(buffers_);
            if (!next_buffer_)
            {
                next_buffer_ = std::move(newBuffer2);
            }
        }

        assert(!buffersToWrite.empty());

        if (buffersToWrite.size() > 25)
        {
            //char buf[256];
            // snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd larger buffers\n",
            //          Timestamp::now().toFormattedString().c_str(),
            //          buffersToWrite.size()-2);
            //fputs(buf, stderr);
            //output.append(buf, static_cast<int>(strlen(buf)));
            buffersToWrite.erase(buffersToWrite.begin()+2, buffersToWrite.end());
        }

        for (size_t i = 0; i < buffersToWrite.size(); ++i)
        {
            // FIXME: use unbuffered stdio FILE ? or use ::writev ?
            output.Append(buffersToWrite[i]->data(), buffersToWrite[i]->length());
        }

        if (buffersToWrite.size() > 2)
        {
            // drop non-bzero-ed buffers, avoid trashing
            buffersToWrite.resize(2);
        }

        if (!newBuffer1)
        {
            assert(!buffersToWrite.empty());
            newBuffer1 = buffersToWrite.back();
            buffersToWrite.pop_back();
            newBuffer1->reset();
        }

        if (!newBuffer2)
        {
            assert(!buffersToWrite.empty());
            newBuffer2 = buffersToWrite.back();
            buffersToWrite.pop_back();
            newBuffer2->reset();
        }

        buffersToWrite.clear();
        output.Flush();
    }
    output.Flush();
}
