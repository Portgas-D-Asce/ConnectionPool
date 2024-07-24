

add_library(singleton INTERFACE)

file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
set_target_properties(singleton
    PROPERTIES
    PUBLIC_HEADER ${headers}
)

install(
    TARGETS
    singleton
    PUBLIC_HEADER
    DESTINATION ${INSTALL_INCLUDEDIR}/singleton
    COMPONENT dev
)
