#include "bit_buffers/bit_buffers.hpp"
#include <array>
#include <cstddef>
#include <climits>

#include "catch2/catch_test_macros.hpp"

/* global storage for buffers */
const std::size_t arr_sz = 20;
std::array<std::byte, arr_sz> buf{};

void reset_buffer(){
    buf.fill(std::byte{0});
}

TEST_CASE("bit_buffers - obitstream initialisation", "[obitstream init]"){
    reset_buffer();
    bb::obitstream obs(buf);

    REQUIRE( obs.bit_capacity() == 20 * CHAR_BIT );
    REQUIRE( obs.size() == 0 );
    REQUIRE( obs.offset() == 0 );
}

TEST_CASE("bit_buffers - obitstream writting is big-endian", "[obitstream big-endian]"){
    reset_buffer();
    bb::obitstream obs(buf);
    
    SECTION("Less than 8bit is unphased"){
        uint8_t v = 0x14;
        obs.write(v, 8);
        REQUIRE( buf[0] == std::byte{v} );
        REQUIRE( obs.size() == 1 );
        REQUIRE( obs.offset() == 8 );
    }

}