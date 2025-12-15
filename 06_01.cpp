#include "rational.h"
#include <cassert>
#include <cmath>
#include <sstream>

auto equal(double x, double y, double epsilon = 1e-6) {
    return std::abs(x - y) < epsilon;
}

int main() {
    Rational<int> x = 1, y(2, 1);
    
    assert(equal(static_cast<double>(x), 1));
    
    assert((x += y) == Rational<int>(3, 1));
    assert((x -= y) == Rational<int>(1, 1));
    assert((x *= y) == Rational<int>(2, 1));
    assert((x /= y) == Rational<int>(1, 1));
    
    assert((x++) == Rational<int>(1, 1));
    assert((x--) == Rational<int>(2, 1));
    assert((++y) == Rational<int>(3, 1));
    assert((--y) == Rational<int>(2, 1));
    
    [[maybe_unused]] auto z = 0;
    
    assert((x + y) == Rational<int>(3, 1));
    assert((x - y) == Rational<int>(-1, 1));
    assert((x * y) == Rational<int>(2, 1));
    assert((x / y) == Rational<int>(1, 2));
    
    assert((x += 1) == Rational<int>(2, 1));
    assert((x + 1) == Rational<int>(3, 1));
    assert((1 + y) == Rational<int>(3, 1));
    assert((1 + 1) == Rational<int>(2, 1));
    
    assert((x <=> y) == std::strong_ordering::equal);
    assert((x <=> Rational<int>(1, 2)) == std::strong_ordering::greater);
    assert((x <=> Rational<int>(3, 1)) == std::strong_ordering::less);
    
    assert(x == y);
    assert(x != Rational<int>(1, 2));
    assert(x < Rational<int>(3, 1));
    assert(x > Rational<int>(1, 2));
    assert(x <= y);
    assert(x >= y);
    
    std::stringstream stream_1("1/2");
    std::stringstream stream_2;
    
    stream_1 >> x;
    stream_2 << x;
    
    assert(stream_2.str() == stream_1.str());
    
    Rational<long> a(123456789L, 987654321L);
    Rational<short> b(3, 4);
    
    assert(equal(static_cast<double>(b), 0.75));
    
    return 0;
}