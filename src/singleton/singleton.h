#ifndef CONNECTIONPOOL_SINGLETON_H
#define CONNECTIONPOOL_SINGLETON_H
#include <memory>
#include <thread>

template<typename T>
class Singleton {
public:
    // 兼容 0 个，多个任意参数，同种类型仅有一个实例，即使传递参数不同
    template<typename... Args>
    static T& get_instance(Args&&... args) {
        std::call_once(_init_flag, [&]() {
            // 这块还有完美转发的必要么，lambda 表达式已经引用了？？？？？？？
            _instance = std::unique_ptr<T>(new T(std::forward<Args>(args)...));
        });
        return *_instance;
    }

    static void destroy() {
        static std::once_flag flag;
        std::call_once(flag, [&]() {
            _instance.reset(nullptr);
        });
    }
private:
    static std::unique_ptr<T> _instance;
    // 使用不同的参数构造单例，会导致多个局部静态变量，进而导致多例
    // 故取消局部静态变量
    static std::once_flag _init_flag;
};

template<typename T>
std::unique_ptr<T> Singleton<T>::_instance = nullptr;

template<typename T>
std::once_flag Singleton<T>::_init_flag = {};

#endif //CONNECTIONPOOL_SINGLETON_H
