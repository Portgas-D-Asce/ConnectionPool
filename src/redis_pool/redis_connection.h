#ifndef CONNECTIONPOOL_REDIS_CONNECTION_H
#define CONNECTIONPOOL_REDIS_CONNECTION_H
#include <hiredis/hiredis.h>

#include "connection_pool/connection.h"
#include "connection_pool/connection_option.h"

class RedisConnection : public Connection {
private:
    std::shared_ptr<ConnectionOption> _opt;
    redisContext* _ctx;
public:
    explicit RedisConnection(std::shared_ptr<ConnectionOption> opt) : _opt(std::move(opt)), _ctx(nullptr) {}

    ~RedisConnection() override { if(_ctx) redisFree(_ctx); }

    [[nodiscard]] auto ctx() const { return _ctx; }

    bool connect(const struct timeval& timeout) override;
};

#endif //CONNECTIONPOOL_REDIS_CONNECTION_H
