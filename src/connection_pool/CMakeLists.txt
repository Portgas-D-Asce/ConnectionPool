
add_library(pool INTERFACE)

file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
set_target_properties(pool
    PROPERTIES
    # 会导致只安装一个文件，后续文件不会被安装
    # PUBLIC_HEADER ${headers}
    PUBLIC_HEADER "${headers}"
)

install(
    TARGETS
    pool
    PUBLIC_HEADER
    DESTINATION ${INSTALL_INCLUDEDIR}/connection_pool
    COMPONENT dev
)

