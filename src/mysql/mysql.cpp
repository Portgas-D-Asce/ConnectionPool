#include <cassert>
#include "../mysql/mysql_connection.h"
#include "../connection_pool/connection_pool.h"

using namespace std;


int main() {
    ConnectionPool<MysqlConnection>& pool = Singleton<ConnectionPool<MysqlConnection>>::get_instance();

    auto routing = [&pool]() {
        auto conn = pool.get();
        auto c = conn->ctx();
        if(mysql_query(c, "show tables")) {
            printf("%u\n", mysql_errno(c));
            return;
        }

        MYSQL_RES* res = mysql_use_result(c);

        MYSQL_ROW row;
        while((row = mysql_fetch_row(res)) != nullptr) {
            printf("%s\n", row[0]);
        }
        mysql_free_result(res);
    };

    vector<shared_ptr<thread>> ts(100);
    for(int i = 0; i < 100; ++i) {
        ts[i] = make_shared<thread>(routing);
    }

    for(int i = 0; i < 100; ++i) {
        ts[i]->join();
    }

    return 0;
}
