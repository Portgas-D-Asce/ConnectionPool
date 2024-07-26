#ifndef CONNECTIONPOOL_CONNECTION_POOL_H
#define CONNECTIONPOOL_CONNECTION_POOL_H
#include <queue>
#include <string>
#include <cassert>
#include <mutex>
#include <condition_variable>
#include <thread>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <toml++/toml.h>

#include "connection_option.h"
#include "connection_pool_option.h"
#include "singleton/singleton.h"


template<typename Connection>
class ConnectionPool {
public:
    using MilliSecond = std::chrono::duration<size_t, std::milli>;
private:
    std::string _config_file;
    // 连接配置
    std::shared_ptr<ConnectionOption> _conn_opt;
    // 线程池配置
    std::shared_ptr<ConnectionPoolOption> _pool_opt;
    // 空闲连接队列
    std::queue<std::shared_ptr<Connection>> _connections;
    // 创建的全部连接数
    size_t _total;
    bool _stop;

    // 同步相关
    mutable std::mutex _mtx;
    // 连接创建/销毁线程条件变量
    mutable std::condition_variable _cv_create;
    mutable std::condition_variable _cv_destroy;
    // 获取连接线程条件变量
    mutable std::condition_variable _cv_get;
    // 创建/销毁连接线程
    std::shared_ptr<std::thread> _create, _destroy;

public:
    std::shared_ptr<Connection> get(MilliSecond timeout = static_cast<MilliSecond>(1000));

    // single pattern need this
    friend class std::default_delete<ConnectionPool>;
    friend class Singleton<ConnectionPool>;
    ConnectionPool(const ConnectionPool&) = delete;
    ConnectionPool(const ConnectionPool&&) = delete;
    ConnectionPool& operator=(const ConnectionPool&) = delete;
    ConnectionPool& operator=(const ConnectionPool&&) = delete;

private:
    explicit ConnectionPool(std::string config_file);
    ~ConnectionPool();

private:
    void create_connection();

    void destroy_connection();

    void recycle_connection(Connection* p);

    void create_routing();

    void destroy_routing();
};

template<typename Connection>
using SingleConnectionPool = Singleton<ConnectionPool<Connection>>;

template<typename Connection>
ConnectionPool<Connection>::ConnectionPool(std::string config_file)
    : _config_file(std::move(config_file)), _total(0), _stop(false) {

    // singleton class's constructor params loaded from config file generally
    auto config = toml::parse_file(_config_file);

    // 构建连接选项
    std::string host = config["connection"]["host"].value_or("127.0.0.1");
    size_t port = config["connection"]["port"].value_or(0);
    std::string db = config["connection"]["db"].value_or("");
    std::string user = config["connection"]["user"].value_or("");
    std::string passwd = config["connection"]["passwd"].value_or("");
    _conn_opt = std::make_shared<ConnectionOption>(host, port, db, user, passwd);

    // 构建连接池选项
    size_t mn = config["connection_pool"]["mn"].value_or(4);
    size_t mx = config["connection_pool"]["mx"].value_or(16);
    size_t limit = config["connection_pool"]["limit"].value_or(64);
    size_t idle = config["connection_pool"]["idle"].value_or(10000);
    _pool_opt = std::make_shared<ConnectionPoolOption>(mn, mx, limit, idle);

    spdlog::info("connection pool loaded from config file: {}", _config_file);
    spdlog::info("loaded host: {}", _conn_opt->host());
    spdlog::info("loaded port: {}", _conn_opt->port());
    spdlog::info("loaded user: {}", _conn_opt->user());
    spdlog::info("mn: {}", _pool_opt->mn());
    spdlog::info("mx: {}", _pool_opt->mx());
    spdlog::info("limit: {}", _pool_opt->limit());
    spdlog::info("idle: {}", _pool_opt->idle());

    // 创建最小连接个数个连接
    for(size_t i = 0; i < _pool_opt->mn(); ++i) create_connection();

    // 补充连接线程
    _create = std::make_shared<std::thread>(&ConnectionPool::create_routing, this);
    // 销毁连接线程
    _destroy = std::make_shared<std::thread>(&ConnectionPool::destroy_routing, this);

    // 设置多线程日志
    spdlog::set_default_logger(spdlog::stdout_color_mt("stdout colored"));
}

template<typename Connection>
ConnectionPool<Connection>::~ConnectionPool() {
    // 停止所有连接池
    {
        std::lock_guard<std::mutex> lg(_mtx);
        _stop = true;
    }
    // 通知创建/释放连接线程，并等待结束
    _cv_create.notify_one();
    _cv_destroy.notify_one();
    _create->join();
    _destroy->join();

    // 回收所有连接，大括号是必要的
    {
        std::unique_lock<std::mutex> uq(_mtx);
        while(_total) {
            _cv_destroy.wait(uq, [this](){ return !_connections.empty(); });
            while(!_connections.empty()) destroy_connection();
        }
    }
}

template<typename Connection>
std::shared_ptr<Connection> ConnectionPool<Connection>::get(ConnectionPool<Connection>::MilliSecond timeout) {
    std::shared_ptr<Connection> conn = nullptr;

    {
        std::unique_lock<std::mutex> uq(_mtx);
        // 连接池停止时，不应再提供连接
        if(_stop) return nullptr;

        // 等待连接队列不为空，等待失败了则表示获取失败
        if(!_cv_get.wait_for(uq, timeout, [this]() { return !_connections.empty(); })) {
            _cv_create.notify_one();
            return nullptr;
        }
        assert(!_connections.empty() && "get connection from empty queue");

        // 正常来说，是不应这样的，但特殊场景
        conn = std::shared_ptr<Connection>(_connections.front().get(), [this](auto&& ptr) {
            recycle_connection(std::forward<decltype(ptr)>(ptr));
        });

        // 暂时设置 “引用计数归零时，不释放资源”
        _connections.front()->flag(false);
        _connections.pop();

        // 恢复 “引用计数归零时，释放资源”，连接用完是会回到池子中的
        conn->flag(true);
    }

    spdlog::debug("a connection was used");

    // 通知创建/销毁连接线程
    _cv_create.notify_one();
    return conn;
}

template<typename Connection>
void ConnectionPool<Connection>::create_connection() {
    auto conn = std::shared_ptr<Connection>(new Connection(_conn_opt), Connection::deleter);
    if(conn->connect({1, 5000})) {
        _connections.push(conn);
        _total++;
        assert(_total <= _pool_opt->limit() && "total connections > limit");
        spdlog::debug("create connection succeeded.");
    } else {
        spdlog::warn("create connection failed!");
    }
}

template<typename Connection>
void ConnectionPool<Connection>::destroy_connection() {
    assert(_connections.front()->flag() && "memory leak when release connection");
    assert(!_connections.empty() && "memory leak when release connection");
    _connections.pop();
    _total--;
}

template<typename Connection>
void ConnectionPool<Connection>::recycle_connection(Connection* p) {
    assert(p != nullptr && p->flag() && "recycle a nullptr/wrong_flag connection");
    // 刷新连接最近一次使用时间
    p->last_active(std::chrono::system_clock::now());
    {
        // 回收连接
        std::lock_guard<std::mutex> lg(_mtx);
        _connections.emplace(p, Connection::deleter);
    }

    // 虽然写的是通知一个等待获取连接的线程，但可能存在虚假唤醒
    _cv_get.notify_one();

    // 通知创建/销毁线程
    _cv_destroy.notify_one();
    spdlog::debug("recycle a connection");
}

template<typename Connection>
void ConnectionPool<Connection>::create_routing() {
    while(true) {
        std::unique_lock<std::mutex> uq(_mtx);
        // 总连接数（正在使用的连接 + 空闲连接）超过上限时，不再创建新的连接
        // 当线程池中连接数小于最小值时，需要向池中添加新连接
        // stop 也算唤醒成功，否则可能会再次陷入睡眠，导致线程无法结束
        _cv_create.wait(uq, [this]() {
            return (_total < _pool_opt->limit() && _connections.size() < _pool_opt->mn()) || _stop;
        });

        // stop 唤醒，直接退出循环，结束线程
        if(_stop) break;

        // 只创建这么多次，成不成功看天命，总比死循环合适
        size_t cnt = _connections.size();
        while(cnt++ < _pool_opt->mn() && _total < _pool_opt->limit()) create_connection();
    }
    spdlog::info("create thread ended");
}

template<typename Connection>
void ConnectionPool<Connection>::destroy_routing() {
    while(true) {
        // 连接池中连接超过最大连接数，无理由释放多余连接
        // 连接池中连接数超过最小连接数，且没超过最大连接数，但最早的连接已经很久没有使用了
        auto check = [this]() {
            return _connections.size() > _pool_opt->mx() ||
                   (_connections.size() > _pool_opt->mn() && _connections.front()->idle() > _pool_opt->idle());
        };

        std::unique_lock<std::mutex> uq(_mtx);
        // stop 也算唤醒成功，否则可能会再次陷入睡眠，导致线程无法结束
        _cv_destroy.wait(uq, [this, check]() { return check() || _stop; });

        // stop 唤醒，直接退出循环，结束线程
        if(_stop) break;

        while(check()) destroy_connection();
    }
    spdlog::info("destroy thread ended");
}

#endif //CONNECTIONPOOL_CONNECTION_POOL_H
