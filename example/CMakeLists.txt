
include_directories(${CMAKE_SOURCE_DIR}/src)

# add_executable(mysql_demo mysql.cpp)
# target_link_libraries(mysql_demo mysql_pool pthread)

add_executable(redis_demo redis.cpp)
target_link_libraries(redis_demo redis_pool pthread)

# install(TARGETS mysql_demo)
install(TARGETS redis_demo)
