#ifndef CONNECTIONPOOL_CONNECTION_OPTION_H
#define CONNECTIONPOOL_CONNECTION_OPTION_H
#include <string>

class ConnectionOption {
private:
    std::string _host;
    size_t _port{};
    std::string _db;
    std::string _user;
    std::string _passwd;
public:
    ConnectionOption() = default;
    ConnectionOption(std::string host, size_t port, std::string db, std::string user, std::string passwd)
        : _host(std::move(host)), _port(port), _db(std::move(db)), _user(std::move(user)), _passwd(std::move(passwd)) {}

    [[nodiscard]] std::string host() const {
        return _host;
    }

    void host(const std::string& host) {
        _host = host;
    }

    [[nodiscard]] size_t port() const {
        return _port;
    }

    void port(size_t port) {
        _port = port;
    }

    std::string db() const {
        return _db;
    }

    void db(std::string db) {
        _db = db;
    }

    [[nodiscard]] std::string user() const {
        return _user;
    }

    void user(const std::string& user) {
        _user = user;
    }

    [[nodiscard]] std::string passwd() const {
        return _passwd;
    }

    void passwd(const std::string& passwd) {
        _passwd = passwd;
    }
};


#endif //CONNECTIONPOOL_CONNECTION_OPTION_H
