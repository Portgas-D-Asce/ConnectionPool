aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} redis_src)

add_library(redis_pool SHARED ${redis_src})
target_link_libraries(redis_pool ${hiredis_LIBRARIES})

install(TARGETS redis_pool)

file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
install(FILES ${headers} DESTINATION include/redis)