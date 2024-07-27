
# 追加模块搜索路径
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(mysql REQUIRED)

add_library(mysql_pool SHARED)
add_library(connection_pool::mysql_pool ALIAS mysql_pool)

target_sources(mysql_pool
    PRIVATE
    # aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} mysql_src)
    mysql_connection.cpp
)

target_include_directories(mysql_pool
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(mysql_pool
    PUBLIC
    mysqlclient
    spdlog::spdlog
    tomlplusplus::tomlplusplus
)

# 设置属性
set_target_properties(mysql_pool
    PROPERTIES
    POSITION_INDEPENDENT_CODE 1
    SOVERSION ${PROJECT_VERSION}
    OUTPUT_NAME "mysql_pool"
    DEBUG_POSTFIX "_d"
    # file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
    PUBLIC_HEADER "mysql_connection.h"
    MACOSX_RPATH ON
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)


install(
    TARGETS
    mysql_pool
    EXPORT
    ${PROJECT_NAME}-targets
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
    DESTINATION ${INSTALL_INCLUDEDIR}/mysql_pool
    COMPONENT dev
)
