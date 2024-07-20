#include "mysql_connection.h"

bool MysqlConnection::connect(const struct timeval& timeout) {
    _ctx = mysql_init(nullptr);

    MYSQL* ptr = mysql_real_connect(_ctx, _opt->host().c_str(), _opt->user().c_str(),
                                    _opt->passwd().c_str(),_db.c_str(), _opt->port(),
                                    nullptr, 0);

    ptr ? printf("connection success!\n") : printf("connection failed!\n");
    return ptr;
}