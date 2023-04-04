#include <iostream>
#include <bitset>
#include <bit>
#include <config.hpp>

int main (int argc, char** argv)
{
    unsigned int i = 2;
    std::cout << "hello CMake!\n";

    using bin = std::bitset<6>;
 
    for (unsigned x{0}; x != 10; ++x)
    {
        unsigned const z = std::bit_ceil(x); // `ceil2` before P1956R1
 
        std::cout << "bit_ceil( " << bin(x) << " ) = " << bin(z) << " - sizeof(x): " << sizeof(bin(x)) << '\n';
    }

    std::cout << project_name << " " << project_version;

    return 0;
}