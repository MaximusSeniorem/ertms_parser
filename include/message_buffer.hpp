#pragma once
#include <cstdint>
#include <concepts>

const uint64_t mask = 0xffffffffffffffff;

template <std::size_t N>
struct MessageBuffer{
    public:

    private:
    std::byte buffer[N];
};



