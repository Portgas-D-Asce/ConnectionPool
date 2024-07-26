#ifndef CONNECTIONPOOL_MYSQL_CONNECTION_H
#define CONNECTIONPOOL_MYSQL_CONNECTION_H
#include <mysql.h>

#include "connection_pool/connection.h"
#include "connection_pool/connection_option.h"


class MysqlConnection : public Connection {
private:
    std::shared_ptr<ConnectionOption> _opt;
    MYSQL* _ctx;
public:
    explicit MysqlConnection(std::shared_ptr<ConnectionOption> opt) : _opt(std::move(opt)), _ctx(nullptr) {}

    ~MysqlConnection() override { if(_ctx) mysql_close(_ctx); }

    [[nodiscard]] auto ctx() const { return _ctx; }

    bool connect(const struct timeval& timeout) override;
};

#endif //CONNECTIONPOOL_MYSQL_CONNECTION_H
