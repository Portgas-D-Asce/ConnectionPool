#ifndef CONNECTIONPOOL_CONNECTION_H
#define CONNECTIONPOOL_CONNECTION_H
#include <memory>
#include <chrono>
// 为什么这块可以找到 spdlog ？？？？？
#include <spdlog/spdlog.h>

class Connection {
private:
    // 最后一次被使用的时间点
    std::chrono::system_clock::time_point _last_active;
    // 控制智能指针是否释放资源
    bool _flag;
public:
    explicit Connection() : _last_active(std::chrono::system_clock::now()), _flag(true) {}

    virtual ~Connection() = default;

    // 禁止 connection 拷贝，否则会导致 ctx 指针二次释放
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    virtual bool connect(const struct timeval& timeout) = 0;

    [[nodiscard]] bool flag() const {
        return _flag;
    }

    void flag(bool flg) {
        _flag = flg;
    }

    void last_active(std::chrono::system_clock::time_point tp) {
        _last_active = tp;
    }

    long long idle() {
        std::chrono::nanoseconds idle = std::chrono::system_clock::now() - _last_active;
        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(idle);
        return ms.count();
    }

    // 自定义智能指针释放函数
    static void deleter(Connection* p) {
        assert(p != nullptr && "delete a nullptr connection");
        if(p->_flag) {
            spdlog::debug("release connection(not ctx)");
            delete p;
        } else {
            spdlog::debug("recycle connection {}", static_cast<void*>(p));
        }
    }
};

#endif //CONNECTIONPOOL_CONNECTION_H
