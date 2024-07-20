
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(singleton)

add_subdirectory(connection_pool)

add_subdirectory(redis)

# add_subdirectory(mysql)