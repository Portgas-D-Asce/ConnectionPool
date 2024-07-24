
add_library(connection_pool INTERFACE)

file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
set_target_properties(connection_pool
    PROPERTIES
    PUBLIC_HEADER ${headers}
)

install(
    TARGETS
    connection_pool
    PUBLIC_HEADER
    DESTINATION ${INSTALL_INCLUDEDIR}/connection_pool
    COMPONENT dev
)

