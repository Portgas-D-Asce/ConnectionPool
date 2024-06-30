cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-O2)

include_directories(/usr/local/mysql-8.0.30-macos12-arm64/include)

link_directories(/usr/local/mysql-8.0.30-macos12-arm64/lib)

add_executable(mysql mysql.cpp)

target_link_libraries(mysql mysqlclient)
