#include "mysql_connection_option.h"

MysqlConnectionOption::MysqlConnectionOption(std::string host, size_t port,
                                             std::string user, std::string passwd)
    : ConnectionOption(std::move(host), port, std::move(user), std::move(passwd)) {}
