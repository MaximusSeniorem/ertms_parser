cmake_minimum_required(VERSION 3.15...3.26)

project(hello VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# configure files based on CMake configuration options
add_subdirectory(config)

add_executable(hello src/main.cpp)

#move it to src ?
target_include_directories(hello PRIVATE "${CMAKE_BINARY_DIR}/config" PUBLIC "include")