#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "redis_pool/redis_connection.h"
#include "connection_pool/connection_pool.h"
using namespace std;

static void example_argv_command(redisContext *c, size_t n) {
    char **argv, tmp[42];
    size_t *argvlen;
    redisReply *reply;

    /* We're allocating two additional elements for command and key */
    argv = static_cast<char**>(malloc(sizeof(*argv) * (2 + n)));
    argvlen = static_cast<size_t*>(malloc(sizeof(*argvlen) * (2 + n)));

    /* First the command */
    argv[0] = (char*)"RPUSH";
    argvlen[0] = sizeof("RPUSH") - 1;

    /* Now our key */
    argv[1] = (char*)"argvlist";
    argvlen[1] = sizeof("argvlist") - 1;

    /* Now add the entries we wish to add to the list */
    for (size_t i = 2; i < (n + 2); i++) {
        argvlen[i] = snprintf(tmp, sizeof(tmp), "argv-element-%zu", i - 2);
        argv[i] = strdup(tmp);
    }

    /* Execute the command using redisCommandArgv.  We're sending the arguments with
     * two explicit arrays.  One for each argument's string, and the other for its
     * length. */
    reply = static_cast<redisReply *>(redisCommandArgv(c, n + 2, (const char **) argv, (const size_t *) argvlen));

    if (reply == NULL || c->err) {
        fprintf(stderr, "Error:  Couldn't execute redisCommandArgv\n");
        exit(1);
    }

    if (reply->type == REDIS_REPLY_INTEGER) {
        printf("%s reply: %lld\n", argv[0], reply->integer);
    }

    freeReplyObject(reply);

    /* Clean up */
    for (size_t i = 2; i < (n + 2); i++) {
        free(argv[i]);
    }

    free(argv);
    free(argvlen);
}

int main() {
    spdlog::set_default_logger(spdlog::stdout_color_mt("stdout colored"));
    // spdlog::set_level(spdlog::level::critical);
    std::string config_file = "/Users/pk/Project/ConnectionPool/config/redis_pool.toml";
    auto& pool = SingleConnectionPool<RedisConnection>::get_instance(config_file);

    auto routing = [&pool]() {
        auto conn = pool.get();
        if(!conn) {
            printf("get connection failed\n");
            return;
        }
        auto c = conn->ctx();
        redisReply* reply = static_cast<redisReply *>(redisCommand(c, "PING"));
        printf("PING: %s\n", reply->str);
        freeReplyObject(reply);


        /* Set a key */
        reply = static_cast<redisReply *>(redisCommand(c, "SET %s %s", "foo", "hello world"));
        printf("SET: %s\n", reply->str);
        freeReplyObject(reply);

        /* Set a key using binary safe API */
        reply = static_cast<redisReply *>(redisCommand(c, "SET %b %b", "bar", (size_t) 3, "hello", (size_t) 5));
        printf("SET (binary API): %s\n", reply->str);
        freeReplyObject(reply);

        /* Try a GET and two INCR */
        reply = static_cast<redisReply *>(redisCommand(c, "GET foo"));
        printf("GET foo: %s\n", reply->str);
        freeReplyObject(reply);

        reply = static_cast<redisReply *>(redisCommand(c, "INCR counter"));
        printf("INCR counter: %lld\n", reply->integer);
        freeReplyObject(reply);
        /* again ... */
        reply = static_cast<redisReply *>(redisCommand(c, "INCR counter"));
        printf("INCR counter: %lld\n", reply->integer);
        freeReplyObject(reply);

        /* Create a list of numbers, from 0 to 9 */
        reply = static_cast<redisReply *>(redisCommand(c, "DEL mylist"));
        freeReplyObject(reply);
        for (int j = 0; j < 10; j++) {
            char buf[64];

            snprintf(buf,64,"%u",j);
            reply = static_cast<redisReply *>(redisCommand(c, "LPUSH mylist element-%s", buf));
            freeReplyObject(reply);
        }

        /* Let's check what we have inside the list */
        reply = static_cast<redisReply *>(redisCommand(c, "LRANGE mylist 0 -1"));
        if (reply->type == REDIS_REPLY_ARRAY) {
            for (int j = 0; j < reply->elements; j++) {
                printf("%u) %s\n", j, reply->element[j]->str);
            }
        }
        freeReplyObject(reply);

        /* See function for an example of redisCommandArgv */
        example_argv_command(c, 10);
    };

    vector<shared_ptr<thread>> ts(100);
    for(int i = 0; i < 100; ++i) {
        ts[i] = make_shared<thread>(routing);
    }

    for(int i = 0; i < 100; ++i) {
        ts[i]->join();
    }

    // 保证单例先于 spdlog 释放，否则 spdlog 都释放了
    // 单例析构函数还在用 spdlog 打印日志，导致 core dump
    SingleConnectionPool<RedisConnection>::destroy();

    return 0;
}
