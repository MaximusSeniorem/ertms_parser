#include "bit_buffers/bit_buffers.hpp"
#include <array>
#include <cstddef>
#include <climits>

#include "catch2/catch_test_macros.hpp"


const std::size_t sz_arr = 20;
std::array<std::byte, sz_arr> buf{};
bb::obitstream obs(buf);


TEST_CASE("bit_buffers - obitstream", "[obitstream init]"){
    REQUIRE( obs.bit_capacity() == 20 * CHAR_BIT );
    REQUIRE( obs.size() == 0 );
}