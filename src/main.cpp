#include <iostream>
#include <iomanip>
#include <cassert>
#include <cstdint>
#include <climits>
#include <array>
#include <bitset>
#include <concepts>
#include <cstddef>
#include <span>
#include "bit_buffers.hpp"

//utils
void print_array(std::span<std::byte> arr)
{
    for (auto i : arr){
        std::cout << std::bitset<8>(std::to_integer<int>(i));
        //std::cout << std::hex << std::to_integer<int>(i) << ' ';
    }
    std::cout << '\n';
}

template <std::unsigned_integral T, std::size_t sz>
void binany_print(T val){
    std::cout << std::bitset<sz>(val);
}

//elements, i.e. var, packets, messages
struct abstract_elem
{
    virtual ~abstract_elem() = default;
    virtual void write(bb::obitstream& obb) = 0;
    virtual void read(bb::ibitstream& ibb) = 0; 

};


template <std::unsigned_integral T, size_t N>
    requires(N > 0 && N <= (sizeof(T) * CHAR_BIT))
struct var : abstract_elem
{
    var(T _val) : m_val(_val) {}
    var() : var(0) {}

    void write(bb::obitstream& obb)  
    {
        obb.write(m_val, m_len);
    }
    
    void read(bb::ibitstream& ibb) 
    {
        T tmp_val;
        ibb.read(tmp_val, m_len);
        m_val = tmp_val;
    }

    T getVal() { return m_val; }

    T m_val : N;
    const size_t m_len = N;
};

struct PacketTest : abstract_elem
{
    var<uint8_t, 8> type;
    var<uint16_t, 16> length;
    
    PacketTest() : type(0), length(0) {}
    PacketTest(uint8_t _t, uint16_t _l) : type(_t), length(_l) {}

    void write(bb::obitstream& obb)
    {
        type.write(obb);
        length.write(obb);
    }
    
    void read(bb::ibitstream& ibb) 
    {
        type.read(ibb);
        length.read(ibb);
    }
};


int main (int argc, char** argv)
{
    const std::size_t buf_sz = 6;
    std::array<std::byte, 20> buf{};
    bb::obitstream obb(buf);
    
    std::cout << '\n';

    uint16_t j1 = 0x01FA;
    uint16_t j2 = 0x7FFe;
    uint16_t j3 = 0x01FF;

    obb.write(j1, 10)
       .write(j2, 15)
       .write(j3, 10);

    std::cout << '\n';
    print_array(buf);
    std::cout << '\n';

    uint16_t j_out;
    bb::ibitstream ibb(buf);

    ibb.read(j_out, 10);
    assert(j_out == j1);
    ibb.read(j_out, 15);
    assert(j_out == j2);
    ibb.read(j_out, 10);
    assert(j_out == j3);
    std::cout << "first test ok \n";    

    var<uint8_t, 6> t{0x2f};
    t.m_val = 0x3f;
    var<uint16_t, 15> t1;
    t1.m_val = 0x7fff;
    var<uint32_t, 32> t2;
    t2.m_val = 0xffffffff;
    var<uint64_t, 36> t3;
    t3.m_val = 0x1ffffff;
    
    bb::obitstream obb1(buf);
    t.write(obb1);
    t1.write(obb1);
    t2.write(obb1);
    t3.write(obb1);

    PacketTest p(15, 10);
    p.write(obb1);
    
    print_array(buf);
    std::cout << '\n';
    
    bb::obitstream obb2(buf);
    std::array<abstract_elem *, 5> ae_array = {&t,&t1,&t2,&t3,&p};

    for(auto e : ae_array){
        e->write(obb2);
    }
    print_array(buf);
    
    bb::ibitstream ibb2(buf);
    var<uint8_t, 6> tb;
    var<uint16_t, 15> t1b;
    var<uint32_t, 32> t2b;
    var<uint64_t, 36> t3b;
    PacketTest pb;

    tb.read(ibb2);
    t1b.read(ibb2);
    t2b.read(ibb2);
    t3b.read(ibb2);
    pb.read(ibb2);

    assert(tb.m_val == t.m_val);
    assert(t1b.m_val == t1.m_val);
    assert(t2b.m_val == t2.m_val);
    assert(t3b.m_val == t3.m_val);
    assert(pb.length.m_val == p.length.m_val);
    assert(pb.type.m_val == p.type.m_val);
    
    return 0;
}