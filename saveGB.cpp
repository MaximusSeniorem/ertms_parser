#include <array>
#include <climits>
#include <concepts>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <variant>

const uint64_t mask = 0xffffffffffffffff;

struct AbstractElem
{
    virtual ~AbstractElem() = 0;
    virtual void write(std::ostream & os) = 0;
};
AbstractElem::~AbstractElem() {}


template <std::unsigned_integral T, size_t N>
    requires(N > 0 && N <= (sizeof(T) * CHAR_BIT))
struct Var : public AbstractElem 
{
    Var(T _val) : val(_val) {}
    Var() : Var(0) {}
    T val : N;
    size_t len = N;

    void write(std::ostream & os) 
    {
        auto f = os.flags();
        os << std::setw(3) << len << " bits - " << std::setw(16) << std::hex
                << (uint64_t)val << " - "
                << "mask 0x" << std::setw(sizeof(T) * 2) << std::setfill('0')
                << (mask >> ((sizeof(uint64_t) * CHAR_BIT) - len)) << '\n'
                << std::setfill(' ');
        os.flags(f);
    }
};

//using VarVariant = std::variant<Var<uint8_t>,Var<uint16_t>,Var<uint32_t>Var<uint64_t>>

struct PacketTest : public AbstractElem
{
    Var<uint8_t, 8> type;
    Var<uint16_t, 16> length;
    
    PacketTest(uint8_t _t, uint16_t _l) : type(_t), length (_l) {}

    void write(std::ostream & os)
    {
        type.write(os);
        length.write(os);
    }
};

int main() 
{
    Var<uint8_t, 6> t{0x2f};
    t.val = 0x3f;
    Var<uint16_t, 15> t1;
    t1.val = 0x7fff;
    Var<uint32_t, 32> t2;
    t2.val = 0xffffffff;
    Var<uint64_t, 36> t3;
    t3.val = 0x1ffffff;


    t.write(std::cout);
    t1.write(std::cout);
    t2.write(std::cout);
    t3.write(std::cout);

    PacketTest p(15, 10);
    p.write(std::cout);

    
    std::array<AbstractElem *, 5> ae_array = {&t,&t1,&t2,&t3,&p};

    std::cout << "\n";
    for(auto e : ae_array){
        e->write(std::cout);
    }

}