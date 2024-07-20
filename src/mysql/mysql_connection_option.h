#ifndef CONNECTIONPOOL_MYSQL_CONNECTION_OPTION_H
#define CONNECTIONPOOL_MYSQL_CONNECTION_OPTION_H
#include <string>
#include "connection_pool/connection_option.h"

class MysqlConnectionOption : public ConnectionOption {
public:
    explicit MysqlConnectionOption(std::string host = "127.0.0.1", size_t port = 3306,
        std::string user = "root", std::string passwd = "pk2147483647");
};


#endif //CONNECTIONPOOL_MYSQL_CONNECTION_OPTION_H
