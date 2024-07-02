#ifndef CONNECTIONPOOL_MYSQL_CONNECTION_H
#define CONNECTIONPOOL_MYSQL_CONNECTION_H
#include <mysql.h>
#include "mysql_connection_option.h"
#include "../connection_pool/connection.h"


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

    auto ctx() const { return _ctx; }

    bool connect(const struct timeval& timeout) override;
};

bool MysqlConnection::connect(const struct timeval& timeout) {
    _ctx = mysql_init(nullptr);

    MYSQL* ptr = mysql_real_connect(_ctx, _opt->host().c_str(), _opt->user().c_str(),
                                    _opt->passwd().c_str(),_db.c_str(), _opt->port(),
                                    nullptr, 0);

    ptr ? printf("connection success!\n") : printf("connection failed!\n");
    return ptr;
}

#endif //CONNECTIONPOOL_MYSQL_CONNECTION_H
