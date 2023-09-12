#pragma once
#include <span>
#include <bit>
#include <iostream>

namespace utils
{
    void print_array(std::span<std::byte> arr, std::ostream &os);
}
