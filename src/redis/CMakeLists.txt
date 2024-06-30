cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-O2)

include_directories(/usr/local/include)

link_directories(/usr/local/lib)

add_executable(redis redis.cpp)

target_link_libraries(redis hiredis)

# target_link_libraries(redis redis++)
