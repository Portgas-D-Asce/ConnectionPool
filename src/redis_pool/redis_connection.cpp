#include "redis_connection.h"

bool RedisConnection::connect(const struct timeval& timeout) {
    _ctx = redisConnectWithTimeout(_opt->host().c_str(), _opt->port(), timeout);

    if(_ctx && !_ctx->err) {
        spdlog::debug("connect succeeded.");
        return true;
    }

    if(_ctx) {
        spdlog::warn("connect failed: {}!", _ctx->errstr);
        redisFree(_ctx);
        // 释放之后，设置为空，防止在析构时二次释放
        _ctx = nullptr;
    } else {
        spdlog::warn("connect failed, ctx is nullptr!");
    }

    return false;
}