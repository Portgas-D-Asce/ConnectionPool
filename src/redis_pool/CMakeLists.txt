
project(redis_pool VERSION 1.0.0)

# 依赖项 hiredis 处理
find_package(hiredis 1.2.0 QUIET)
# 如果没找到则从仓库直接拉取
if (NOT hiredis_FOUND)
    # 包含拉取模块
    include(FetchContent)
    # 拉取声明
    fetchcontent_declare(hiredis
        GIT_REPOSITORY https://github.com/redis/hiredis.git
        GIT_TAG        v1.2.0
        # 拉取的源码将被解压到哪个文件夹
        SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/hiredis
    )
    fetchcontent_makeavailable(hiredis)
    # 拉取失败
    if(NOT hiredis_POPULATED)
        # 还是当成致命错误吧
        message(FATAL_ERROR "fetch hiredis failed!")
    # 拉取成功
    else ()
        message(STATUS "fetch hiredis succeeded.")
        message(STATUS ${hiredis_SOURCE_DIR})
        message(STATUS ${hiredis_BINARY_DIR})
        # 设置头文件包含目录 ！！！！！！
        set(hiredis_header ${hiredis_SOURCE_DIR}/..)
    endif ()
# 如果找到了直接使用
else ()
    message(STATUS ${hiredis_LIBRARIES})
    message(STATUS ${hiredis_INCLUDE_DIRS})
    # 设置头文件包含目录！！！！！！！
    set(hiredis_header ${hiredis_INCLUDE_DIRS})
endif ()

# 创建动态库
add_library(redis_pool SHARED)

# 添加源文件
target_sources(redis_pool
    PRIVATE
    # aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} redis_src)
    redis_connection.cpp
    redis_connection_option.cpp
)

# 追加 include 目录
# message(STATUS ${hiredis_header})
target_include_directories(redis_pool
    # 需要设置为 public 将其暴露给 redis-demo
    # 否则 redis-demo 需要显示包含
    PUBLIC
    ${hiredis_header}
)

# 添加依赖库
target_link_libraries(redis_pool
    # 需要设置为 public 将其暴露给 redis-demo
    # 否则 redis-demo 需要显示链接
    PUBLIC
    # 无论哪种方式，都可以使用该方式表示
    hiredis::hiredis
)

# 设置属性
set_target_properties(redis_pool
    PROPERTIES
    POSITION_INDEPENDENT_CODE 1
    SOVERSION ${PROJECT_VERSION}
    OUTPUT_NAME "redis_pool"
    DEBUG_POSTFIX "_d"
    # file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
    PUBLIC_HEADER "redis_connection.h;redis_connection_option.h"
    MACOSX_RPATH ON
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

# 安装
install(
    TARGETS
    redis_pool
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
    DESTINATION ${INSTALL_INCLUDEDIR}/redis_pool
    COMPONENT dev
)

# 测试 demo
add_executable(redis_demo)
target_sources(redis_demo
    PRIVATE
    redis_demo.cpp
)

target_link_libraries(redis_demo
    PRIVATE
    # 同级目录也是可以感知到的
    redis_pool
    pthread
)

# include(CMakePrintHelpers)
# cmake_print_properties(
#     TARGETS
#     redis_pool
#     PROPERTIES
#     INTERFACE_COMPILE_OPTIONS
#     INTERFACE_INCLUDE_DIRECTORIES
#     INTERFACE_LINK_LIBRARIES
# )

#[[ example 程序无需安装
# 设置可执行程序的 rpath，保证其在安装后也可以找到动态链接库，从而正常运行
file(RELATIVE_PATH _rel ${CMAKE_INSTALL_PREFIX}/${INSTALL_BINDIR} ${CMAKE_INSTALL_PREFIX})
if(APPLE)
    set(_rpath "@loader_path/${_rel}")
else()
    set(_rpath "\$ORIGIN/${_rel}")
endif()
file(TO_NATIVE_PATH "${_rpath}/${INSTALL_LIBDIR}" exec_rpath)
# message(STATUS ${_rpath})
# message(STATUS ${exec_rpath})

set_target_properties(redis_demo
    PROPERTIES
    MACOSX_RPATH ON
    SKIP_BUILD_RPATH OFF
    BUILD_WITH_INSTALL_RPATH OFF
    INSTALL_RPATH ${exec_rpath}
    INSTALL_RPATH_USE_LINK_PATH ON
)

# 安装
install(
    TARGETS
    redis_demo
    RUNTIME
    DESTINATION ${INSTALL_BINDIR}
    COMPONENT bin
)
]]
