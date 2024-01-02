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

    SECTION("after writing 8 bits and then 14 bits") {
        writer.write(0b10101010, 8);
        REQUIRE(writer.size() == 1);
        REQUIRE(writer.offset() == 8);

        writer.write(0b10101010101010, 14);
        REQUIRE(writer.size() == 3);
        REQUIRE(writer.offset() == 22);
    }

    SECTION("after writing overflow") {
        writer.write(42, 32);
        REQUIRE(writer.size() == 4);
        REQUIRE(writer.offset() == 32);

        writer.write(0, 1);
        REQUIRE(writer.size() == 4);
        REQUIRE(writer.offset() == 32);
    }


    SECTION("after reading 8 bits") {
        reader.read(val, 8);
        REQUIRE(reader.size() == 1);
        REQUIRE(reader.offset() == 8);
    }

    SECTION("after reading 16 bits and then 12 bits") {
        reader.read(val, 16);
        REQUIRE(reader.size() == 2);
        REQUIRE(reader.offset() == 16);

        reader.read(val, 12);
        REQUIRE(reader.size() == 4);
        REQUIRE(reader.offset() == 28);
    }

    SECTION("after reading overflow") {
        reader.read(val, 32);
        REQUIRE(reader.size() == 4);
        REQUIRE(reader.offset() == 32);

        reader.read(val, 1);
        REQUIRE(reader.size() == 4);
        REQUIRE(reader.offset() == 32);
    }
}


TEST_CASE("bwriter write", "[bwriter]") {
    std::array<std::byte, 6> buffer;
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


    SECTION("writing 32 bits") {
        writer.write<uint32_t>(0b00000000101010100000000010101010, 32);
        REQUIRE(buffer[0] == std::byte{0b00000000});
        REQUIRE(buffer[1] == std::byte{0b10101010});
        REQUIRE(buffer[2] == std::byte{0b00000000});
        REQUIRE(buffer[3] == std::byte{0b10101010});
    }

    SECTION("writing is left align - 12 bits") {
        writer.write<uint16_t>(0b000000001010, 12);
        REQUIRE(buffer[0] == std::byte{0b00000000});
        REQUIRE(buffer[1] == std::byte{0b10100000});
    }

    SECTION("writing is bit-wise packed - 12 bits then 18 bits") {
        writer.write<uint16_t>(0b000000001010, 12);
        REQUIRE(buffer[0] == std::byte{0b00000000});
        REQUIRE(buffer[1] == std::byte{0b10100000});

        writer.write<uint32_t>(0b101010101010101010, 18);
        REQUIRE(buffer[0] == std::byte{0b00000000});
        REQUIRE(buffer[1] == std::byte{0b10101010});
        REQUIRE(buffer[2] == std::byte{0b10101010});
        REQUIRE(buffer[3] == std::byte{0b10101000});
    }
}

TEST_CASE("breader read", "[breader]") {
    std::array<std::byte, 4> buffer;
    buffer.fill(std::byte{0b10101010});

    bb::breader reader(buffer);

    SECTION("reading 8 bits") {
        uint16_t val = 0;
        reader.read(val, 8);
        REQUIRE(val == 0b10101010);
    }

    SECTION("reading 6 bits") {
        uint16_t val = 0;
        reader.read(val, 6);
        REQUIRE(val == 0b101010);
    }

    SECTION("reading 16 bits") {
        uint16_t val = 0;
        reader.read(val, 16);
        REQUIRE(val == 0b1010101010101010);
    }
    
    SECTION("reading 32 bits") {
        uint32_t val = 0;
        reader.read(val, 32);
        REQUIRE(val == 0b10101010101010101010101010101010);
    }

    SECTION("reading 10 bits then 7 bits") {
        uint16_t val = 0;
        reader.read(val, 10);
        REQUIRE(val == 0b1010101010);
        reader.read(val, 7);
        REQUIRE(val == 0b1010101);
    }

    SECTION("reading 7 bits then 10 bits") {
        uint16_t val = 0;
        reader.read(val, 7);
        REQUIRE(val == 0b1010101);
        reader.read(val, 10);
        REQUIRE(val == 0b0101010101);
    }
}

