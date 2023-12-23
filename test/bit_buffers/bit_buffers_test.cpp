#include "bit_buffers/bit_buffers.hpp"
#include <array>
#include <cstddef>
#include <climits>

#include "catch2/catch_test_macros.hpp"

TEST_CASE("bitbuffer bit_capacity", "[bitbuffer]") {
    std::array<std::byte, 4> buffer;
    bb::bwriter writer(buffer);

    REQUIRE(writer.bit_capacity() == 32);
}

TEST_CASE("bitbuffer size and offset", "[bitbuffer]") {
    std::array<std::byte, 4> buffer;
    bb::bwriter writer(buffer);
    bb::breader reader(buffer);
    uint16_t val = 0;

    REQUIRE(writer.size() == 0);
    REQUIRE(writer.offset() == 0);
    REQUIRE(reader.size() == 0);
    REQUIRE(reader.offset() == 0);

    SECTION("after writing 8 bits") {
        writer.write(0b10101010, 8);

        REQUIRE(writer.size() == 1);
        REQUIRE(writer.offset() == 8);
    }

    SECTION("after writing 16 bits") {
        writer.write(0b1010101010101010, 16);

        REQUIRE(writer.size() == 2);
        REQUIRE(writer.offset() == 16);
    }

    SECTION("after reading 8 bits") {
        reader.read(val, 8);

        REQUIRE(reader.size() == 1);
        REQUIRE(reader.offset() == 8);
    }

    SECTION("after reading 16 bits") {
        reader.read(val, 16);

        REQUIRE(reader.size() == 2);
        REQUIRE(reader.offset() == 16);
    }
}


TEST_CASE("breader read", "[breader]") {
    std::array<std::byte, 4> buffer;
    buffer.fill(std::byte{0});

    bb::bwriter writer(buffer);
    writer.write<uint8_t>(0b10101010, 8);
    writer.write<uint16_t>(0b1010101010101010, 16);

    bb::breader reader(buffer);

    SECTION("reading 8 bits") {
        uint16_t val = 0;
        reader.read(val, 8);
        REQUIRE(val == 0b10101010);
    }

    SECTION("reading 16 bits") {
        uint16_t val = 0;
        reader.read(val, 16);
        REQUIRE(val == 0b1010101010101010);
    }
}


TEST_CASE("bwriter write", "[bwriter]") {
    std::array<std::byte, 4> buffer;
    buffer.fill(std::byte{0});

    bb::bwriter writer(buffer);

    SECTION("writing 8 bits") {
        writer.write<uint8_t>(0b10101010, 8);
        REQUIRE(buffer[0] == std::byte{0b10101010});
    }

    SECTION("writing 16 bits") {
        writer.write<uint16_t>(0b0000000010101010, 16);
        REQUIRE(buffer[0] == std::byte{0b00000000});
        REQUIRE(buffer[1] == std::byte{0b10101010});
    }
}