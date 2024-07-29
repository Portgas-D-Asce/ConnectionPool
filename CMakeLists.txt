# 指定 cmake 版本
cmake_minimum_required(VERSION 3.20)

# 指定项目名称
project(connection_pool VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

include(cmake/base.cmake)

if (NOT CONNECTION_POOL_REDIS)
    option(CONNECTION_POOL_REDIS "Use redis connection pool" ON)
    set(CONNECTION_POOL_REDIS ON)
endif ()

if (NOT CONNECTION_POOL_MYSQL)
    option(CONNECTION_POOL_MYSQL "Use mysql connection pool" OFF)
    set(CONNECTION_POOL_MYSQL OFF)
endif ()

if (NOT CONNECTION_POOL_UNIT_TEST)
    option(CONNECTION_POOL_UNIT_TEST "Enable unit test" OFF)
    set(CONNECTION_POOL_UNIT_TEST OFF)
endif ()

# 源码模块
add_subdirectory(src)

add_subdirectory(example)

# 测试模块
# 顶层不开启 ctest 可以正常使用，make test 无法使用
# make: *** No rule to make target `test'.  Stop.
# 下层不开启 ctest 和 make test 均无法使用，提示无测试用例
if(CONNECTION_POOL_UNIT_TEST)
    enable_testing()
    add_subdirectory(test)
endif ()


include(cmake/cmake.cmake)

include(cmake/cpack.cmake)
