aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} mysql_src)

add_library(mysql_pool SHARED ${mysql_src})
target_link_libraries(mysql_pool mysqlclient)

install(TARGETS mysql_pool)

file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
install(FILES ${headers} DESTINATION include/mysql)
