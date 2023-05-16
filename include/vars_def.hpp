#pragma once

#include <climits> 
#include <concepts>
#include <cstdint>
#include "vars_enum.hpp"

template <std::unsigned_integral T, std::size_t N>
    requires (N> 0 && N <= (sizeof(T)*CHAR_BIT))
struct Var{
    T x : N;
    size_t len = N;
};
