#include "redis_connection_option.h"


RedisConnectionOption::RedisConnectionOption(std::string host, size_t port,
                                             std::string user, std::string passwd)
    : ConnectionOption(std::move(host), port, std::move(user), std::move(passwd)) {}