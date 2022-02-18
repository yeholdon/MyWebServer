// @Author Honcy Ye
// @Email yeholdon@gmail.com
#include <algorithm>
#include "base/LogStream.h"


const char zero[] = "0123456789";


template<typename T> 
size_t Convert(char buf[], T value)
{
    T i = value;
    char *p = buf;

    do
    {
        int lsd = static_cast<int>(i % 10);
        i /= 10;
        *p++ = zero[lsd];
    } while (i != 0);

    if (value < 0)
    {
        *p++ = '-';
    }
    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

template<typename T>
void LogStream::FormatInteger(T v)
{
    // buffer容不下kMaxNumericSize个字符的话会被直接丢弃
    if (buffer_.Avail() >= kMaxNumericSize)
    {
        size_t len = Convert(buffer_.current(), v);
        buffer_.Add(len);
    }
}

LogStream& LogStream::operator<<(short v)
{
    *this << static_cast<int>(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned short v)
{
    *this << static_cast<unsigned int>(v);
    return *this;
}

LogStream& LogStream::operator<<(int v)
{
    FormatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned int v)
{
    FormatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long v)
{
    FormatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long v)
{
    FormatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long long v)
{
    FormatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long v)
{
    FormatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(double v)
{
    if (buffer_.Avail() >= kMaxNumericSize)
    {
        int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12g", v);
        buffer_.Add(len);
    }
    return *this;
}

LogStream& LogStream::operator<<(long double v)
{
    if (buffer_.Avail() >= kMaxNumericSize)
    {
        int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12Lg", v);
        buffer_.Add(len);
    }
    return *this;
}