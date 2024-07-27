

list(APPEND CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(connection_pool REQUIRED)

if (CONNECTION_POOL_REDIS)
    add_executable(redis_demo)
    target_sources(redis_demo
        PRIVATE
        redis.cpp
    )

    target_link_libraries(redis_demo
        PRIVATE
        connection_pool::redis_pool
        pthread
    )
endif ()

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

#[[
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
)]]

if (CONNECTION_POOL_MYSQL)
    add_executable(mysql_demo)
    target_sources(mysql_demo
        PRIVATE
        mysql.cpp
    )

    target_link_libraries(mysql_demo
        PRIVATE
        connection_pool::mysql_pool
        pthread
    )
endif ()

#[[
set_target_properties(mysql_demo
    PROPERTIES
    MACOSX_RPATH ON
    SKIP_BUILD_RPATH OFF
    BUILD_WITH_INSTALL_RPATH OFF
    INSTALL_RPATH ${exec_rpath}
    INSTALL_RPATH_USE_LINK_PATH ON
)

install(
    TARGETS
    mysql_demo
    RUNTIME
    DESTINATION ${INSTALL_BINDIR}
    COMPONENT bin
)
]]
