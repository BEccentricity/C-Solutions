#include <cassert>
#include <cmath>
#include <compare>
#include <istream>
#include <numeric>
#include <ostream>
#include <sstream>

template <typename T>
concept Integral = std::is_integral_v<T>;

template <Integral T>
class Rational;

template <typename Derived>
class addable {
protected:
    ~addable() = default;
    
    friend Derived operator+(const Derived& lhs, const Derived& rhs) {
        Derived result = lhs;
        result += rhs;
        return result;
    }
};

template <typename Derived>
class subtractable {
protected:
    ~subtractable() = default;
    
    friend Derived operator-(const Derived& lhs, const Derived& rhs) {
        Derived result = lhs;
        result -= rhs;
        return result;
    }
};

template <typename Derived>
class multipliable {
protected:
    ~multipliable() = default;
    
    friend Derived operator*(const Derived& lhs, const Derived& rhs) {
        Derived result = lhs;
        result *= rhs;
        return result;
    }
};

template <typename Derived>
class dividable {
protected:
    ~dividable() = default;
    
    friend Derived operator/(const Derived& lhs, const Derived& rhs) {
        Derived result = lhs;
        result /= rhs;
        return result;
    }
};

template <typename Derived>
class incrementable {
protected:
    ~incrementable() = default;
    
    friend Derived& operator++(Derived& obj) {
        obj += Derived(1);
        return obj;
    }
    
    friend Derived operator++(Derived& obj, int) {
        Derived temp = obj;
        ++obj;
        return temp;
    }
};

template <typename Derived>
class decrementable {
protected:
    ~decrementable() = default;
    
    friend Derived& operator--(Derived& obj) {
        obj -= Derived(1);
        return obj;
    }
    
    friend Derived operator--(Derived& obj, int) {
        Derived temp = obj;
        --obj;
        return temp;
    }
};

template <Integral T>
class Rational : 
    public addable<Rational<T>>,
    public subtractable<Rational<T>>,
    public multipliable<Rational<T>>,
    public dividable<Rational<T>>,
    public incrementable<Rational<T>>,
    public decrementable<Rational<T>>
{
public:
    Rational(T num = 0, T den = 1) : m_num(num), m_den(den) {
        reduce();
    }
    
    explicit operator double() const {
        return 1.0 * m_num / m_den;
    }
    
    Rational& operator+=(const Rational& other) {
        auto lcm = std::lcm(m_den, other.m_den);
        m_num = m_num * (lcm / m_den) + other.m_num * (lcm / other.m_den);
        m_den = lcm;
        reduce();
        return *this;
    }
    
    Rational& operator-=(const Rational& other) {
        return *this += Rational(other.m_num * -1, other.m_den);
    }
    
    Rational& operator*=(const Rational& other) {
        m_num *= other.m_num;
        m_den *= other.m_den;
        reduce();
        return *this;
    }
    
    Rational& operator/=(const Rational& other) {
        return *this *= Rational(other.m_den, other.m_num);
    }
    
    std::strong_ordering operator<=>(const Rational& other) const {
        auto lhs_num = m_num * other.m_den;
        auto rhs_num = other.m_num * m_den;
        
        if (lhs_num < rhs_num) return std::strong_ordering::less;
        if (lhs_num > rhs_num) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }
    
    bool operator==(const Rational& other) const {
        return (m_num == other.m_num) && (m_den == other.m_den);
    }
    
    friend std::istream& operator>>(std::istream& stream, Rational& rational) {
        return (stream >> rational.m_num).ignore() >> rational.m_den;
    }
    
    friend std::ostream& operator<<(std::ostream& stream, const Rational& rational) {
        return stream << rational.m_num << '/' << rational.m_den;
    }
    
private:
    void reduce() {
        if (m_den < 0) {
            m_num = -m_num;
            m_den = -m_den;
        }
        auto gcd = std::gcd(m_num, m_den);
        m_num /= gcd;
        m_den /= gcd;
    }
    
    T m_num = 0, m_den = 1;
};

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
}