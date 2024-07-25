#ifndef CONNECTION_POOL_CONNECTION_POOL_OPTION_H
#define CONNECTION_POOL_CONNECTION_POOL_OPTION_H
#include <utility>

class ConnectionPoolOption {
private:
    // 连接池中连接的最小数量和最大数量
    size_t _mn, _mx, _limit;
    // 连接距离上次使用空闲时间阈值
    size_t _idle;
public:
    // load from config file generally
    explicit ConnectionPoolOption(size_t mn, size_t mx, size_t limit, size_t idle)
        : _mn(mn), _mx(mx), _limit(limit), _idle(idle) {}

    [[nodiscard]] size_t mn() const {
        return _mn;
    }

    [[nodiscard]] size_t mx() const {
        return _mx;
    }

    [[nodiscard]] size_t idle() const {
        return _idle;
    }

    [[nodiscard]] size_t limit() const {
        return _limit;
    }
};


#endif //CONNECTION_POOL_CONNECTION_POOL_OPTION_H
