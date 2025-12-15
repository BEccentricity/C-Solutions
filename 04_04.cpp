#include <cassert>
#include <cmath>
#include <compare>
#include <concepts>
#include <istream>
#include <numeric>
#include <ostream>
#include <sstream>
#include <vector>

template <typename T>
concept Integral = std::is_integral_v<T>;

template <Integral T>
class Rational
{
public:
    Rational(T num = 0, T den = 1) : m_num(num), m_den(den)
    {
        reduce();
    }

    explicit operator double() const
    { 
        return 1.0 * m_num / m_den;
    }

    auto & operator+=(Rational const & other)
    {
        auto lcm = std::lcm(m_den, other.m_den);

        m_num = m_num * (lcm / m_den) + other.m_num * (lcm / other.m_den);
        
        m_den = lcm;

        reduce();

        return *this;
    }

    auto & operator-=(Rational const & other) 
    { 
        return *this += Rational(other.m_num * -1, other.m_den);
    }

    auto & operator*=(Rational const & other)
    {
        m_num *= other.m_num;

        m_den *= other.m_den;

        reduce();

        return *this;
    }
    
    auto & operator/=(Rational const & other) 
    { 
        return *this *= Rational(other.m_den, other.m_num);
    }

    auto const operator++(int) { auto x = *this; *this += 1; return x; }

    auto const operator--(int) { auto x = *this; *this -= 1; return x; }

    auto & operator++() { *this += 1; return *this; }

    auto & operator--() { *this -= 1; return *this; }

    friend auto operator+ (Rational lhs, Rational const & rhs) { return lhs += rhs; }

    friend auto operator- (Rational lhs, Rational const & rhs) { return lhs -= rhs; }

    friend auto operator* (Rational lhs, Rational const & rhs) { return lhs *= rhs; }

    friend auto operator/ (Rational lhs, Rational const & rhs) { return lhs /= rhs; }

    friend std::strong_ordering operator<=>(Rational const & lhs, Rational const & rhs)
    {
        auto lhs_num = lhs.m_num * rhs.m_den;
        auto rhs_num = rhs.m_num * lhs.m_den;
        
        if (lhs_num < rhs_num) return std::strong_ordering::less;
        if (lhs_num > rhs_num) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }

    friend bool operator==(Rational const & lhs, Rational const & rhs)
    {
        return (lhs <=> rhs) == std::strong_ordering::equal;
    }

    friend auto & operator>>(std::istream & stream, Rational & rational)
    {
        return (stream >> rational.m_num).ignore() >> rational.m_den;
    }

    friend auto & operator<<(std::ostream & stream, Rational const & rational)
    {
        return stream << rational.m_num << '/' << rational.m_den;
    }

private:
    void reduce()
    {
        if (m_den < 0)
        {
            m_num = -m_num;
            m_den = -m_den;
        }

        auto gcd = std::gcd(m_num, m_den);

        m_num /= gcd;
        m_den /= gcd;
    }

    T m_num = 0, m_den = 1;
};

auto equal(double x, double y, double epsilon = 1e-6)
{
    return std::abs(x - y) < epsilon;
}

int main()
{
    Rational<int> x = 1, y(2, 1);

    std::vector<int> vector_2(5);
    std::vector<int> vector_3 = {1, 2, 3, 4, 5};

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

    Rational<long long> a(123456789012LL, 987654321098LL);
    Rational<short> b(3, 4);
    Rational<char> c(1, 2);

    assert(equal(static_cast<double>(b), 0.75));
    assert(equal(static_cast<double>(c), 0.5));
    
    return 0;
}