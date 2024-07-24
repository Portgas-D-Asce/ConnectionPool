
# include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(singleton)

add_subdirectory(connection_pool)

# todo 添加选项控制是否生成
add_subdirectory(redis_pool)

# todo 添加选项控制是否生成
add_subdirectory(mysql_pool)
