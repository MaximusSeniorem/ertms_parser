#include "utils.hpp"
#include <bitset>

void utils::print_array(std::span<std::byte> arr, std::ostream &os)
{
    for (auto i : arr){
        os << std::bitset<8>(std::to_integer<int>(i));
        //so << std::hex << std::to_integer<int>(i) << ' ';
    }
    os << '\n';
}
