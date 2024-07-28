# Connection Pool

A simple general concurrency singleton connection pool.

support redis and mysql

## redis
based on [hiredis](https://github.com/redis/hiredis) 

## mysql

# dependence
necessary
- [hiredis](https://github.com/redis/hiredis)
- [tomlplusplus](https://github.com/marzer/tomlplusplus)
- [spdlog](https://github.com/gabime/spdlog)
optional
- [googletest](https://github.com/google/googletest)

# usage
```cmake
find_package(connection_pool 1.0.0 QUIET)
if (NOT connection_pool_FOUND)
    include(FetchContent)
    fetchcontent_declare(connection_pool
        GIT_REPOSITORY https://github.com/Portgas-D-Asce/ConnectionPool.git
        GIT_TAG v1.0.0-alpha
    )
    fetchcontent_makeavailable(connection_pool)
    # 拉取失败
    if(NOT connection_pool_POPULATED)
        message(FATAL_ERROR "fetch connection_pool failed!")
    endif ()
endif ()

target_link_libraries(exec
    PUBLIC
    $<$<BOOL:${connection_pool_FOUND}>:connection_pool::>redis_pool
    # $<$<BOOL:${connection_pool_FOUND}>:connection_pool::>mysql_pool
)
```
