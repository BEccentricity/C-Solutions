#include <cassert>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>

union FloatBits
{
    float f;
    unsigned int u;
};

int ilog2_int(int value)
{
    if (value <= 0)
    {
        return -1;
    }
    
    unsigned int u = static_cast<unsigned int>(value);
    int result = -1;
    
    while (u > 0)
    {
        u >>= 1;
        ++result;
    }
    
    return result;
}

int ilog2_float(float value)
{
    if (value <= 0.0f)
    {
        return -1;
    }
    
    FloatBits fb;
    fb.f = value;
    
    unsigned int bits = fb.u;
    
    unsigned int exponent = (bits >> 23) & 0xFF;
    unsigned int mantissa = bits & 0x7FFFFF;
    
    if (exponent == 0xFF)
    {
        return -1;
    }
    
    if (exponent == 0)
    {
        if (mantissa == 0)
        {
            return -1;
        }
        
        int count = -126;
        while ((mantissa & 0x400000) == 0)
        {
            mantissa <<= 1;
            --count;
        }
        return count;
    }
    
    return static_cast<int>(exponent) - 127;
}

void test_ilog2_int()
{
    assert(ilog2_int(1) == 0);
    assert(ilog2_int(2) == 1);
    assert(ilog2_int(3) == 1);
    assert(ilog2_int(4) == 2);
    assert(ilog2_int(7) == 2);
    assert(ilog2_int(8) == 3);
    assert(ilog2_int(15) == 3);
    assert(ilog2_int(16) == 4);
    assert(ilog2_int(31) == 4);
    assert(ilog2_int(32) == 5);
    assert(ilog2_int(1024) == 10);
    assert(ilog2_int(1025) == 10);
    assert(ilog2_int(2047) == 10);
    assert(ilog2_int(2048) == 11);
    
    assert(ilog2_int(0) == -1);
    assert(ilog2_int(-5) == -1);
    assert(ilog2_int(-2147483647) == -1);
    
    std::cout << "All int tests passed!\n";
}

void test_ilog2_float()
{
    assert(ilog2_float(1.0f) == 0);
    assert(ilog2_float(2.0f) == 1);
    assert(ilog2_float(3.0f) == 1);
    assert(ilog2_float(4.0f) == 2);
    assert(ilog2_float(7.0f) == 2);
    assert(ilog2_float(8.0f) == 3);
    assert(ilog2_float(15.0f) == 3);
    assert(ilog2_float(16.0f) == 4);
    assert(ilog2_float(31.0f) == 4);
    assert(ilog2_float(32.0f) == 5);
    assert(ilog2_float(1024.0f) == 10);
    assert(ilog2_float(1025.0f) == 10);
    assert(ilog2_float(2047.0f) == 10);
    assert(ilog2_float(2048.0f) == 11);
    
    assert(ilog2_float(0.5f) == -1);
    assert(ilog2_float(0.25f) == -2);
    assert(ilog2_float(0.125f) == -3);
    assert(ilog2_float(0.0625f) == -4);
    assert(ilog2_float(0.03125f) == -5);
    
    assert(ilog2_float(1.5f) == 0);
    assert(ilog2_float(2.5f) == 1);
    assert(ilog2_float(3.5f) == 1);
    assert(ilog2_float(5.5f) == 2);
    
    assert(ilog2_float(0.0f) == -1);
    assert(ilog2_float(-1.0f) == -1);
    assert(ilog2_float(-5.0f) == -1);
    
    std::cout << "All float normal tests passed!\n";
}

void test_ilog2_float_denormalized()
{
    union FloatBits fb;
    
    fb.f = 1.17549435e-38f;
    assert(ilog2_float(fb.f) == -126);
    
    fb.u = 0x00400000;
    float min_normal = fb.f;
    assert(ilog2_float(min_normal) == -126);
    
    fb.u = 0x003FFFFF;
    float max_denormal = fb.f;
    assert(ilog2_float(max_denormal) == -127);
    
    fb.u = 0x00000001;
    float smallest_denormal = fb.f;
    assert(ilog2_float(smallest_denormal) == -149);
    
    std::cout << "All float denormalized tests passed!\n";
}

void test_ilog2_float_special()
{
    float inf = std::numeric_limits<float>::infinity();
    assert(ilog2_float(inf) == -1);
    
    float neg_inf = -std::numeric_limits<float>::infinity();
    assert(ilog2_float(neg_inf) == -1);
    
    float nan = std::numeric_limits<float>::quiet_NaN();
    assert(ilog2_float(nan) == -1);
    
    float max_float = std::numeric_limits<float>::max();
    assert(ilog2_float(max_float) == 127);
    
    float min_positive = std::numeric_limits<float>::min();
    assert(ilog2_float(min_positive) == -126);
    
    std::cout << "All float special values tests passed!\n";
}

int main()
{
    std::cout << "Testing ilog2 for int type...\n";
    test_ilog2_int();
    
    std::cout << "\nTesting ilog2 for float type (normal numbers)...\n";
    test_ilog2_float();
    
    std::cout << "\nTesting ilog2 for float type (denormalized numbers)...\n";
    test_ilog2_float_denormalized();
    
    std::cout << "\nTesting ilog2 for float type (special values)...\n";
    test_ilog2_float_special();
    
    std::cout << "\n=== IEEE 754 Format Information ===\n";
    union FloatBits fb;
    
    fb.f = 1.0f;
    std::cout << "1.0f in hex: 0x" << std::hex << std::setw(8) << std::setfill('0') 
              << fb.u << std::dec << "\n";
    std::cout << "  sign: " << ((fb.u >> 31) & 1) << "\n";
    std::cout << "  exponent: " << ((fb.u >> 23) & 0xFF) << " (raw), " 
              << (((fb.u >> 23) & 0xFF) - 127) << " (actual)\n";
    std::cout << "  mantissa: 0x" << std::hex << (fb.u & 0x7FFFFF) << std::dec << "\n";
    
    return 0;
}