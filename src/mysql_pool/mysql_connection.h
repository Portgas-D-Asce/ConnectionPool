#ifndef CONNECTIONPOOL_MYSQL_CONNECTION_H
#define CONNECTIONPOOL_MYSQL_CONNECTION_H
#include <mysql.h>
#include "mysql_connection_option.h"
#include "connection_pool/connection.h"


class MysqlConnection : public Connection {
public:
    using ConnectionOption = MysqlConnectionOption;
    using ConnectionOptionPtr = std::shared_ptr<ConnectionOption>;
    using ConnectionPtr = std::shared_ptr<MysqlConnection>;
private:
    ConnectionOptionPtr _opt;
    std::string _db;
    MYSQL* _ctx;
public:
    explicit MysqlConnection(ConnectionOptionPtr opt) : _opt(std::move(opt)), _db("mysql"), _ctx(nullptr) {}

    ~MysqlConnection() override { if(_ctx) mysql_close(_ctx); }

    [[nodiscard]] auto ctx() const { return _ctx; }

    bool connect(const struct timeval& timeout) override;
};

#endif //CONNECTIONPOOL_MYSQL_CONNECTION_H
