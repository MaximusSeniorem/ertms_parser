#include <iostream>

#include <cstdint>
#include <climits>
#include <array>
#include <bitset>
#include <concepts>
#include <cstddef>


template <std::size_t N>
void print_array(std::array<std::byte, N> &arr)
{
    for (auto i : arr)
        std::cout << std::bitset<8>(std::to_integer<int>(i)) << ' ';
    std::cout << '\n';
}

template <std::size_t N, std::unsigned_integral T, std::size_t val_len>
    requires(val_len > 0 && val_len <= (sizeof(T) * CHAR_BIT))
void write(std::array<std::byte, N> &arr, T val, std::size_t offset)
{
    const std::size_t val_size = sizeof(T);
    std::size_t index = offset / CHAR_BIT;
    std::size_t arr_shift = offset % CHAR_BIT;
    T val_shifted = val << (val_size * CHAR_BIT - val_len);
    std::byte *val_ptr = reinterpret_cast<std::byte *>(& val_shifted);
    
    val_shifted >>= arr_shift;
    for (auto i = 0; i < val_size; ++i){
        arr[i + index] |= val_ptr[val_size - i - 1];
    }
    //if not aligned and array shift to big, need to write the last sub-byte
    if (arr_shift != 0 && arr_shift + val_len > val_size * CHAR_BIT){
        val_shifted = val << val_size * CHAR_BIT - arr_shift;
        arr[val_size + index] |= val_ptr[val_size - 1];
    }

    std::cout << "wrote b" << std::bitset<val_len>(val) << " @ " << offset << '\n';
    print_array(arr);
}

template <std::size_t N, std::unsigned_integral T, std::size_t val_len>
    requires(val_len > 0 && val_len <= (sizeof(T) * CHAR_BIT))
T read(std::array<std::byte, N> &arr, std::size_t offset)
{
    T val = 0;
    const std::size_t val_size = sizeof(T);
    std::size_t index = offset / CHAR_BIT;
    std::size_t arr_shift = offset % CHAR_BIT;
    std::byte *val_ptr = reinterpret_cast<std::byte *>(& val);
    
    for (auto i = 0; i < val_size; ++i){
        val_ptr[val_size - i - 1] = arr[i + index];
    }
    //if not aligned and array shift to big, need to write the last sub-byte
    if (arr_shift != 0){
        val <<= arr_shift;
        val_ptr[val_size - 1] |= arr[val_size + index] >> arr_shift;
    }
    val >>= (val_size * CHAR_BIT - val_len);
    std::cout << "read = b" << std::bitset<val_len>(val) << " @ " << offset << '\n';
    return val;
}


int main (int argc, char** argv)
{
    const std::size_t arr_sz = 6;
    std::array<std::byte, arr_sz> arr{};
    
    std::cout << '\n';
    uint16_t j = 0x01FA;
    write<arr_sz, uint16_t, 10>(arr, j, 0);
    j = 0xFFF;
    write<arr_sz, uint16_t, 12>(arr, j, 10);
    j = 0x01FF;
    write<arr_sz, uint16_t, 10>(arr, j, 22);
    std::cout << '\n';
    read<arr_sz, uint16_t, 10>(arr, 0);
    read<arr_sz, uint16_t, 12>(arr, 10);
    read<arr_sz, uint16_t, 10>(arr, 22);

    return 0;
}