cmake_minimum_required(VERSION 3.20)
project(ConnectionPool)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/output)

find_package(hiredis 1.2.0 REQUIRED)
if (NOT hiredis_FOUND)
    message(FATAL_ERROR "not found hiredis!")
else ()
    message(STATUS ${hiredis_LIBRARIES})
    message(STATUS ${hiredis_INCLUDE_DIRS})
    include_directories(${hiredis_INCLUDE_DIRS})
endif ()

# find_package(mysqlclient REQUIRED)
# include_directories(/usr/local/mysql-8.0.30-macos12-arm64/include)
# link_directories(/usr/local/mysql-8.0.30-macos12-arm64/lib)

add_subdirectory(src)

add_subdirectory(example)
