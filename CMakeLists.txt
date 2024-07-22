# 指定 cmake 版本
cmake_minimum_required(VERSION 3.20)

# 指定项目名称
project(ConnectionPool VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# 设置编译模式为 release，全局设置可保证其依赖项也是 release 模式
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
message(STATUS "Build type set to ${CMAKE_BUILD_TYPE}")


# 启用 GNUInstallDirs 模块
include(GNUInstallDirs)
# 设置目标生成目录
set(TARGET_DIR ${CMAKE_BINARY_DIR} CACHE PATH "target dir" FORCE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${TARGET_DIR}/${CMAKE_INSTALL_LIBDIR} CACHE PATH "lib output" FORCE)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${TARGET_DIR}/${CMAKE_INSTALL_LIBDIR} CACHE PATH "lib output" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TARGET_DIR}/${CMAKE_INSTALL_BINDIR} CACHE PATH "bin output" FORCE)
# set(LIBRARY_OUTPUT_PATH ${TARGET_DIR}/${CMAKE_INSTALL_LIBDIR} CACHE PATH "lib output" FORCE)
# set(EXECUTABLE_OUTPUT_PATH ${TARGET_DIR}/${CMAKE_INSTALL_BINDIR} CACHE PATH "bin output" FORCE)


# 设置安装目录，使用全局设置是因为 CMAKE_INSTALL_PREFIX 可能会失效
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "intstallation dir" FORCE)

# 定义 4 个全局目录 INSTALL_LIBDIR、INSTALL_BINDIR、INSTALL_INCLUDEDIR、INSTALL_CMAKEDIR
# Offer the user the choice of overriding the installation directories
set(INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR} CACHE PATH "Installation directory for libraries")
set(INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR} CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR} CACHE PATH "Installation directory for header files")
# 设置 CMAKE 安装目录
if(WIN32 AND NOT CYGWIN)
    set(DEF_INSTALL_CMAKEDIR CMake)
else()
    set(DEF_INSTALL_CMAKEDIR share/cmake/${PROJECT_NAME})
endif()
set(INSTALL_CMAKEDIR ${DEF_INSTALL_CMAKEDIR} CACHE PATH "Installation directory for CMake files")

# 打印所有类型安装目录
message(STATUS "Project will be installed to ${CMAKE_INSTALL_PREFIX}")
foreach(p LIB BIN INCLUDE CMAKE)
    file(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${INSTALL_${p}DIR} _path)
    message(STATUS "Installing ${p} components to ${_path}")
    unset(_path)
endforeach()

add_subdirectory(src)

# 顶层不开启 ctest 可以正常使用，make test 无法使用
# make: *** No rule to make target `test'.  Stop.
# 下层不开启 ctest 和 make test 均无法使用，提示无测试用例
option(ENABLE_UNIT_TESTS "Enable unit tests" OFF)
set(ENABLE_UNIT_TESTS OFF)
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")

if(ENABLE_UNIT_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()


include(CMakeCPack.cmake)

