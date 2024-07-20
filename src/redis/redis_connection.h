#ifndef CONNECTIONPOOL_REDIS_CONNECTION_H
#define CONNECTIONPOOL_REDIS_CONNECTION_H
#include <hiredis/hiredis.h>
#include "redis_connection_option.h"
#include "connection_pool/connection.h"


class RedisConnection : public Connection {
public:
    using ConnectionOption = RedisConnectionOption;
    using ConnectionOptionPtr = std::shared_ptr<ConnectionOption>;
    using ConnectionPtr = std::shared_ptr<RedisConnection>;
private:
    ConnectionOptionPtr _opt;
    redisContext* _ctx;
public:
    explicit RedisConnection(ConnectionOptionPtr opt) : _opt(std::move(opt)), _ctx(nullptr) {}

    ~RedisConnection() override { if(_ctx) redisFree(_ctx); }

    [[nodiscard]] auto ctx() const { return _ctx; }

    bool connect(const struct timeval& timeout) override;
};

#endif //CONNECTIONPOOL_REDIS_CONNECTION_H
