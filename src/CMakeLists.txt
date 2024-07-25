
find_package(tomlplusplus 3.4 QUIET)
if(NOT tomlplusplus_FOUND)
    include(FetchContent)
    FetchContent_Declare(tomlplusplus
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
        GIT_TAG v3.4.0
    )
    fetchcontent_makeavailable(tomlplusplus)
    if(NOT tomlplusplus_POPULATED)
        message(FATAL_ERROR "fetch spdlog failed!")
    endif ()
endif ()

find_package(spdlog 1.14 QUIET)
if(NOT spdlog_FOUND)
    include(FetchContent)
    FetchContent_Declare(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.14.1
    )
    fetchcontent_makeavailable(spdlog)
    if(NOT spdlog_POPULATED)
        message(FATAL_ERROR "fetch spdlog failed!")
    endif ()
endif ()

# include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(singleton)

add_subdirectory(connection_pool)

# todo 添加选项控制是否生成
add_subdirectory(redis_pool)

# todo 添加选项控制是否生成
add_subdirectory(mysql_pool)
