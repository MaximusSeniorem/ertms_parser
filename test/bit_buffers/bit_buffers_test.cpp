#include "catch2/catch_test_macros.hpp"
#include "bit_buffers/bit_buffers.hpp"


std::string hello2()
{
    return std::string("hello from bit_buffers");
}


TEST_CASE("hello world bit_buffers", "[hello_bit_buffers]"){
    REQUIRE( hello2() == "hello from bit_buffers" );
}