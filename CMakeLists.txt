cmake_minimum_required(VERSION 3.26)
project(ConnectionPool)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-O2)

add_subdirectory(src/redis)

add_subdirectory(src/mysql)
