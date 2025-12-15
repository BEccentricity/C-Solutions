#include "rational.h"
#include <numeric>

template <Integral T>
Rational<T>::Rational(T num, T den) : m_num(num), m_den(den) {
    reduce();
}

template <Integral T>
Rational<T>::operator double() const {
    return 1.0 * m_num / m_den;
}

template <Integral T>
Rational<T>& Rational<T>::operator+=(const Rational& other) {
    auto lcm = std::lcm(m_den, other.m_den);
    m_num = m_num * (lcm / m_den) + other.m_num * (lcm / other.m_den);
    m_den = lcm;
    reduce();
    return *this;
}

template <Integral T>
Rational<T>& Rational<T>::operator-=(const Rational& other) {
    return *this += Rational(other.m_num * -1, other.m_den);
}

template <Integral T>
Rational<T>& Rational<T>::operator*=(const Rational& other) {
    m_num *= other.m_num;
    m_den *= other.m_den;
    reduce();
    return *this;
}

template <Integral T>
Rational<T>& Rational<T>::operator/=(const Rational& other) {
    return *this *= Rational(other.m_den, other.m_num);
}

template <Integral T>
void Rational<T>::reduce() {
    if (m_den < 0) {
        m_num = -m_num;
        m_den = -m_den;
    }
    auto gcd = std::gcd(m_num, m_den);
    m_num /= gcd;
    m_den /= gcd;
}

// Явные инстанцирования для нужных типов
template class Rational<int>;
template class Rational<long>;
template class Rational<short>;