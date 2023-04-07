#include <iostream>
#include <iomanip>
#include <cstdint>
#include <climits>
#include <array>

#include "config.hpp"

template <std::size_t N, std::unsigned_integral T, std::size_t val_len>
    requires(val_len > 0 && val_len <= (sizeof(T) * CHAR_BIT))
void write(std::array<uint8_t, N> &arr, T val, std::size_t offset)
{
    const uint8_t val_size = sizeof(T);
    std::size_t index = offset / CHAR_BIT;
    std::size_t arr_shift = offset % CHAR_BIT;

    T val_shifted = val << (val_size * CHAR_BIT - val_len);
    val_shifted = val_shifted >> arr_shift;
    uint8_t *val_ptr = reinterpret_cast<uint8_t *>(& val_shifted);
    
    for (auto i = 0; i < val_size; ++i){
        arr[i + index] |= val_ptr[val_size - i - 1];
    }
    //if not aligned and array shift to big, need to write the last sub-byte
    if (arr_shift != 0 && arr_shift + val_len > val_size * CHAR_BIT){
        val_shifted = val << val_size * CHAR_BIT - arr_shift;
        arr[val_size + index] |= val_ptr[val_size - 1];
    }
}

int main (int argc, char** argv)
{
    const std::size_t arr_sz = 20;
    std::array<uint8_t, arr_sz> arr{uint8_t{0}};
    uint16_t j = 0x03FF;
    std::cout << project_name << " " << project_version << '\n';

    write<arr_sz, uint16_t, 10>(arr, j, 0);
    j = 0xFFF;
    write<arr_sz, uint16_t, 12>(arr, j, 10);
    j = 0x03FF;
    write<arr_sz, uint16_t, 10>(arr, j, 22);
    std::cout << '\n' << std::hex << (uint32_t) j << '\n';
    for (auto i : arr)
        std::cout << std::setfill('0') << std::setw(2) << (uint16_t) i << ' ';
    std::cout << '\n';
    
    return 0;
}