
# 依赖项 hiredis 处理
find_package(hiredis 1.2.0 QUIET)
# 如果没找到则从仓库直接拉取
if (NOT hiredis_FOUND)
    # 包含拉取模块
    include(FetchContent)
    # 拉取声明
    fetchcontent_declare(hiredis
        GIT_REPOSITORY https://github.com/redis/hiredis.git
        GIT_TAG v1.2.0
        # 拉取的源码将被解压到哪个文件夹，hiredis target_include_directories bug 规避
        SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/hiredis
    )
    fetchcontent_makeavailable(hiredis)
    # 拉取失败
    if(NOT hiredis_POPULATED)
        # 还是当成致命错误吧
        message(FATAL_ERROR "fetch hiredis failed!")
    endif ()
    # 拉取成功
    # message(STATUS "fetch hiredis succeeded.")
    # message(STATUS ${hiredis_SOURCE_DIR})
    # message(STATUS ${hiredis_BINARY_DIR})
endif ()
# 如果找到了直接使用
# message(STATUS ${hiredis_LIBRARIES})
# message(STATUS ${hiredis_INCLUDE_DIRS})

# 创建动态库
add_library(redis_pool SHARED)

# 添加源文件
target_sources(redis_pool
    PRIVATE
    # aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} redis_src)
    redis_connection.cpp
)

# 很关键的地方: 自己在用，但更重要的是供别人使用！！！！！
# 不是 redis_pool 需要用到的 hiredis 头文件目录
# 而是 redis_pool 将向外提供的头文件目录
target_include_directories(redis_pool
    PUBLIC
    # 构建时导出的 include 目录
    # fetchcontent_makeavailable 成功后，自带以下包含目录
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    # 安装后导出的 include 目录，两者需要保持一致，否则会像 hiredis 那样尴尬
    # find_package 查找到结果，自带以下包含目录
    $<INSTALL_INTERFACE:include>
)

# find_package、fetchcontent_makeavailable 返回的不仅仅是一个库
# 还包含了库上定义的符号 INTERFACE_INCLUDE_DIRECTORIES 等
# 设置了库查询目录，找到的仅仅是一个什么都不带的库
# 使用只能指针管理内存，就不要在混合使用普通指针了，会出问题的
# target_link_directories(redis_pool
#     PUBLIC
#     $<BUILD_INTERFACE:${CMAKE_INSTALL_PREFIX}/lib>
#     $<INSTALL_INTERFACE:lib>
# )

target_link_libraries(redis_pool
    PUBLIC
    # find_package 和 fetchcontent_makeavailable 可以统一使用别名模式
    # 理论上 hiredis 与 hiredis::hiredis 都一样，但 hiredis cmake 可能有问题
    hiredis::hiredis
    # spdlog::spdlog_header_only
    $<BUILD_INTERFACE:spdlog::spdlog>
    $<BUILD_INTERFACE:tomlplusplus::tomlplusplus>
)

# 设置属性
set_target_properties(redis_pool
    PROPERTIES
    POSITION_INDEPENDENT_CODE 1
    SOVERSION ${PROJECT_VERSION}
    OUTPUT_NAME "redis_pool"
    DEBUG_POSTFIX "_d"
    # file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
    PUBLIC_HEADER "redis_connection.h"
    MACOSX_RPATH ON
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)


# 安装
install(
    TARGETS
    redis_pool
    # 安装的文件导出到哪个目标，后续用于生成 targets 文件
    EXPORT
    connection_pool_targets
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

# 获取别名
# get_target_property(_aliased hiredis::hiredis ALIASED_TARGET)
# if(_aliased)
#     message(STATUS "The name hiredis::hiredis is an ALIAS for ${_aliased}.")
# endif()

# 输出目标属性，通常只有 INTERFACE_INCLUDE_DIRECTORIES 设置了
# BUILD_INTERFACE INSTALL_INTERFACE
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
