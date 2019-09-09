#ifndef SINGLETON_H
#define SINGLETON_H

#include <cstdlib>
#include <mutex>
#include <cassert>

template<typename T>
class Singleton
{
public:
    static T& instance()
    {
        static std::once_flag flag;
        std::call_once(flag, [&]{value_ = new T();::atexit(destroy);});
        assert(value_ != NULL);
        return *value_;
    }

private:
    Singleton(){}
    ~Singleton(){}
    Singleton(const Singleton&){}
    const Singleton& operator=(const Singleton&){}

    static void destroy()
    {
        if(value_ != nullptr) {
            delete value_;
            value_ = nullptr;
        }
    }

private:
    static T* value_;
};

template<typename T>
T* Singleton<T>::value_ = nullptr;

#endif // SINGLETON_H
