// @Author Honcy Ye
// @Email yeholdon@gmail.com

#ifndef NON_COPYABLE_H
#define NON_COPYABLE_H

class NonCopyable
{
protected:
    NonCopyable() {}
    ~NonCopyable() {}
private:
    // 拷贝构造函数和赋值运算符设为private
    NonCopyable(const NonCopyable&);
    const NonCopyable& operator=(const NonCopyable&);
};
#endif // NON_COPYABLE_H