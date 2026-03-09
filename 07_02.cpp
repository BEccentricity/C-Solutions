#include <cassert>
#include <cmath>
#include <compare>
#include <exception>
#include <iostream>
#include <istream>
#include <memory>
#include <numeric>
#include <optional>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

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

class Exception : public std::exception
{
public:
    explicit Exception(const char* message) : m_message(message) {}
    
    const char* what() const noexcept override
    {
        return m_message;
    }
    
private:
    const char* m_message;
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
    Rational(T num = 0, T den = 1) : m_num(num), m_den(den)
    {
        if (den == 0)
        {
            throw Exception("Denominator cannot be zero");
        }
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

int main()
{

    try
    {

        std::cerr << "=== Test 1: Custom Exception ===\n";
        try
        {
            Rational<int> r1(1, 0); 
        }
        catch (const std::exception& e)
        {
            std::cerr << "Caught std::exception: " << e.what() << '\n';
        }
        

        std::cerr << "\n=== Test 2: std::bad_alloc ===\n";
        try
        {

            std::vector<int> huge_vector;
            huge_vector.reserve(1000000000000000000); 
        }
        catch (const std::bad_alloc& e)
        {
            std::cerr << "Caught std::bad_alloc: " << e.what() << " - insufficient memory\n";
        }
        

        std::cerr << "\n=== Test 3: std::bad_variant_access ===\n";
        try
        {
            std::variant<int, double> var = 42;

            double wrong_value = std::get<double>(var); 
        }
        catch (const std::bad_variant_access& e)
        {
            std::cerr << "Caught std::bad_variant_access: " << e.what() 
                      << " - type mismatch in variant\n";
        }

        std::cerr << "\n=== Test 4: std::bad_optional_access ===\n";
        try
        {
            std::optional<int> empty_opt;
            int value = empty_opt.value(); 
        }
        catch (const std::bad_optional_access& e)
        {
            std::cerr << "Caught std::bad_optional_access: " << e.what() 
                      << " - access to empty optional\n";
        }
        

        std::cerr << "\n=== Test 5: std::length_error ===\n";
        try
        {
            std::string str;

            str.resize(-1); 
        }
        catch (const std::length_error& e)
        {
            std::cerr << "Caught std::length_error: " << e.what() 
                      << " - invalid string length\n";
        }
        

        std::cerr << "\n=== Test 6: std::out_of_range ===\n";
        try
        {
            std::vector<int> vec = {1, 2, 3};
            int element = vec.at(10); 
        }
        catch (const std::out_of_range& e)
        {
            std::cerr << "Caught std::out_of_range: " << e.what() 
                      << " - index out of vector bounds\n";
        }

        std::cerr << "\n=== Test 7: Additional Rational tests ===\n";
        

        Rational<int> r2(3, 4);
        std::cout << "Rational(3,4) = " << r2 << '\n';
        

        try
        {
            Rational<int> r3(1, 2);
            Rational<int> r4(0, 1);
            auto r5 = r3 / r4; 
        }
        catch (const Exception& e)
        {
            std::cerr << "Caught Exception: " << e.what() << '\n';
        }
        

        std::cerr << "\n=== Test 8: Demonstration of catch(...) ===\n";
        try
        {
            throw 42;
        }
        catch (...)
        {
            std::cerr << "Caught unknown exception in catch(...)\n";
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Global catch of std::exception: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Global catch of unknown exception\n";
    }
    
    return 0;
}