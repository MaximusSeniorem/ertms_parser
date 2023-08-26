#define CATCH_CONFIG_MAIN //ask catch to provide a main function
#include "catch2/catch_test_macros.hpp"

std::string hello()
{
    return std::string("hello");
}


TEST_CASE("hello world", "[hello]"){
    REQUIRE( hello() == "hello" );
}