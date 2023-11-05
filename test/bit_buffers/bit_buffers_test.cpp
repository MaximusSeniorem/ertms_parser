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

TEST_CASE("bit_buffers - bwriter initialisation", "[bwriter init]"){
    reset_buffer();
    bb::bwriter obs(buf);

    REQUIRE( obs.bit_capacity() == arr_sz * CHAR_BIT );
    REQUIRE( obs.size() == 0 );
    REQUIRE( obs.offset() == 0 );
}

TEST_CASE("bit_buffers - bwriter writting is big-endian", "[bwriter big-endian]"){
    reset_buffer();
    bb::bwriter obs(buf);
    
    SECTION("Less than 8bit is unphased"){
        uint8_t v = 0x14;
        obs.write(v, 8);
        REQUIRE( buf[0] == std::byte{v} );
        REQUIRE( obs.size() == 1 );
        REQUIRE( obs.offset() == 8 );
    }

}