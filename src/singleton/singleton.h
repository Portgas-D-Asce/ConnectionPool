#ifndef CONNECTIONPOOL_SINGLETON_H
#define CONNECTIONPOOL_SINGLETON_H
#include <memory>
#include <thread>

template<typename T>
class Singleton {
public:
    // 如何兼容零个参数
    template<typename... Args>
    static T& get_instance(Args&&... args) {
        static std::once_flag flag;
        // 引用了之后会影响完美转发吗？？？？？？
        std::call_once(flag, [&]() {
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
};

template<typename T>
std::unique_ptr<T> Singleton<T>::_instance = nullptr;

#endif //CONNECTIONPOOL_SINGLETON_H
