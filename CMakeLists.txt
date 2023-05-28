cmake_minimum_required(VERSION 3.15...3.26)

project(test VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# configure files based on CMake configuration options
add_subdirectory(config)

add_executable(test src/main.cpp)


#move it to src ?
target_include_directories(test PRIVATE "${CMAKE_BINARY_DIR}/config" PUBLIC "include")