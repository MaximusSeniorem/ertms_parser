cmake_minimum_required(VERSION 3.15...3.26)
project(ertms_parser VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#configure files based on CMake configuration options
add_subdirectory(config)

add_executable(main src/main.cpp)
target_include_directories(main PRIVATE "${CMAKE_BINARY_DIR}/config" PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

find_package(Catch2 3 REQUIRED)
add_executable(parser_tests test/test_main.cpp test/bit_buffers/bit_buffers_test.cpp)
target_include_directories(parser_tests PRIVATE "${CMAKE_BINARY_DIR}/config" PUBLIC "include")
target_link_libraries(parser_tests PRIVATE Catch2::Catch2WithMain)

##not ready tu use ctest yet... :/
#enable_testing()
#add_test(
#    NAME parser_tests
#    COMMAND $<TARGET_FILE:parser_tests>
#)


