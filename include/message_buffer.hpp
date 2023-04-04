#pragma once
#include <cstdint>
#include <concepts>

const uint64_t mask = 0xffffffffffffffff;

template <size_t N>
struct MessageBuffer{
    public:

    private:
    uint8_t buffer[N];
};



