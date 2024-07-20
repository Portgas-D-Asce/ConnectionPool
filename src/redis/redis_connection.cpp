#include "redis_connection.h"

bool RedisConnection::connect(const struct timeval& timeout) {
    _ctx = redisConnectWithTimeout(_opt->host().c_str(), _opt->port(), timeout);

    if(_ctx && !_ctx->err) {
        printf("connection success!\n");
        return true;
    }
    printf("connection failed!\n");
    if(_ctx) {
        printf("connect error: %s\n", _ctx->errstr);
        redisFree(_ctx);
    } else {
        printf("connect error: connect failed\n");
    }

    return false;
}