cmake_minimum_required(VERSION 3.15...3.26)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#configure files based on CMake configuration options
add_subdirectory(config)
#include_directories(PRIVATE "${CMAKE_BINARY_DIR}/config" PUBLIC "include")


project(test VERSION 0.1.0 LANGUAGES CXX)

find_package(Catch2 3 REQUIRED)
add_executable(test test/test_main.cpp test/bit_buffers/bit_buffers_test.cpp)
target_include_directories(test PRIVATE "${CMAKE_BINARY_DIR}/config" PUBLIC "include")
target_link_libraries(test PRIVATE Catch2::Catch2WithMain)


project(main VERSION 0.1.0 LANGUAGES CXX)
add_executable(main src/main.cpp)
target_include_directories(main PRIVATE "${CMAKE_BINARY_DIR}/config" PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
