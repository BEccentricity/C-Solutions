#ifndef RATIONAL_H
#define RATIONAL_H

#include <compare>
#include <concepts>
#include <istream>
#include <ostream>

template <typename T>
concept Integral = std::is_integral_v<T>;

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
    Rational(T num = 0, T den = 1);
    
    explicit operator double() const;
    
    Rational& operator+=(const Rational& other);
    Rational& operator-=(const Rational& other);
    Rational& operator*=(const Rational& other);
    Rational& operator/=(const Rational& other);
    
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
    void reduce();
    
    T m_num = 0, m_den = 1;
};

#endif