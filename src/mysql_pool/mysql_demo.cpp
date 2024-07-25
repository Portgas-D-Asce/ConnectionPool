#include <cassert>
#include <mysql.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "singleton/singleton.h"
#include "connection_pool/connection_pool.h"
#include "mysql_pool/mysql_connection.h"

using namespace std;


int main() {
    spdlog::set_default_logger(spdlog::stdout_color_mt("stdout colored"));
    std::string config_file = "/Users/pk/Project/ConnectionPool/config/mysql_pool.toml";
    auto& pool = SingleConnectionPool<MysqlConnection>::get_instance(config_file);

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

    SingleConnectionPool<MysqlConnection>::destroy();

    return 0;
}
