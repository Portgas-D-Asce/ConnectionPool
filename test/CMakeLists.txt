project(Test VERSION 1.0.0)

find_package(googletest 1.15.0 QUIET)
if (NOT googletest_FOUND)
    message(STATUS "not find googletest, try to fetch from github")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.15.0
    )
    fetchcontent_makeavailable(googletest)
    if(NOT googletest_POPULATED)
        message(FATAL_ERROR "fetch googletest failed!")
    else ()
        message(STATUS "fetch hiredis succeeded.")
        message(STATUS ${googletest_SOURCE_DIR})
        message(STATUS ${googletest_BINARY_DIR})
        set(googletest_header ${googletest_SOURCE_DIR}/googletest/include/)
    endif ()
else ()
    message(STATUS ${googletest_LIBRARIES})
    message(STATUS ${googletest_INCLUDE_DIRS})
    set(googletest_header ${googletest_INCLUDE_DIRS})
endif ()

add_executable(test_demo test.cpp)
target_include_directories(test_demo
    PUBLIC ${googletest_SOURCE_DIR}/googletest/in
)

target_link_libraries(test_demo
    PRIVATE gtest gtest_main
)

enable_testing()
add_test(
    NAME google_test
    COMMAND $<TARGET_FILE:test_demo>
)

# when building with Visual Studio
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# Prevent GoogleTest from using PThreads
# set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)
