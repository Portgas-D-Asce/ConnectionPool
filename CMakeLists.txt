# 指定 cmake 版本
cmake_minimum_required(VERSION 3.20)

# 指定项目名称
project(connection_pool VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/base.cmake)

# 源码模块
add_subdirectory(src)


# 测试模块
# 顶层不开启 ctest 可以正常使用，make test 无法使用
# make: *** No rule to make target `test'.  Stop.
# 下层不开启 ctest 和 make test 均无法使用，提示无测试用例
option(ENABLE_UNIT_TESTS "Enable unit tests" ON)
set(ENABLE_UNIT_TESTS OFF)
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")

# add_subdirectory(example)

if(ENABLE_UNIT_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()

include(cmake/cmake.cmake)

# cpack 打包
include(cmake/cpack.cmake)
