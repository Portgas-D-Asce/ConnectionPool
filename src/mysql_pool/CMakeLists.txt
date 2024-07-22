
project(mysql_pool VERSION 1.0.0)

# 追加模块搜索路径
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(mysql QUIET)
if (NOT MYSQL_VERSION)
    # 还是当作致命错误吧
    message(FATAL_ERROR "find mysql failed!")
else ()
    set(mysql_header ${MYSQL_INCLUDE_DIR})
    set(mysql_lib_dir ${MYSQL_LIB_DIR})
endif ()


add_library(mysql_pool SHARED)

target_sources(mysql_pool
    PRIVATE
    # aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} mysql_src)
    mysql_connection.cpp
    mysql_connection_option.cpp
)

target_include_directories(mysql_pool
    PUBLIC
    ${mysql_header}
)

target_link_directories(mysql_pool
    PUBLIC
    ${mysql_lib_dir}
)

target_link_libraries(mysql_pool
    PUBLIC
    mysqlclient
)

# 设置属性
set_target_properties(mysql_pool
    PROPERTIES
    POSITION_INDEPENDENT_CODE 1
    SOVERSION ${PROJECT_VERSION}
    OUTPUT_NAME "mysql_pool"
    DEBUG_POSTFIX "_d"
    # file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
    PUBLIC_HEADER "mysql_connection.h;mysql_connection_option.h"
    MACOSX_RPATH ON
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

# 安装
install(
    TARGETS
    mysql_pool
    ARCHIVE
    DESTINATION ${INSTALL_LIBDIR}
    COMPONENT lib
    RUNTIME
    DESTINATION ${INSTALL_BINDIR}
    COMPONENT bin
    LIBRARY
    DESTINATION ${INSTALL_LIBDIR}
    COMPONENT lib
    PUBLIC_HEADER
    # 目标属性中定义的头文件会自动安装
    DESTINATION ${INSTALL_INCLUDEDIR}/mysql_pool
    COMPONENT dev
)

add_executable(mysql_demo)
target_sources(mysql_demo
    PRIVATE
    mysql_demo.cpp
)

target_link_libraries(mysql_demo
    PRIVATE
    # 同级目录也是可以感知到的
    mysql_pool
    pthread
)
