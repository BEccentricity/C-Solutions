#include <algorithm>
#include <cctype>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <sstream>
#include <string>

using namespace std::literals;

class Integer
{
public:
    Integer() = default;
    
    Integer(const std::string& str)
    {
        std::size_t start = 0;
        if (str[0] == '+' || str[0] == '-')
        {
            m_is_positive = (str[0] == '+');
            start = 1;
        }
        else
        {
            m_is_positive = true;
        }
        
        for (std::size_t i = start; i < str.size(); ++i)
        {
            if (std::isdigit(str[i]))
            {
                m_digits.push_back(str[i] - '0');
            }
        }
        
        remove_leading_zeros();
        if (m_digits.empty())
        {
            m_digits.push_back(0);
            m_is_positive = true;
        }
    }
    
    explicit operator std::string() const
    {
        std::string result;
        if (!m_is_positive && !(m_digits.size() == 1 && m_digits[0] == 0))
        {
            result += '-';
        }
        for (auto digit : m_digits)
        {
            result += static_cast<char>('0' + digit);
        }
        return result;
    }
    
    int sign() const
    {
        if (m_digits.size() == 1 && m_digits[0] == 0)
        {
            return 0;
        }
        return m_is_positive ? 1 : -1;
    }
    
    Integer abs() const
    {
        Integer result = *this;
        result.m_is_positive = true;
        return result;
    }
    
    Integer& operator+=(const Integer& other)
    {
        if (m_is_positive == other.m_is_positive)
        {
            add_absolute(other);
        }
        else
        {
            subtract_absolute(other);
        }
        return *this;
    }
    
    Integer& operator-=(const Integer& other)
    {
        if (m_is_positive != other.m_is_positive)
        {
            add_absolute(other);
        }
        else
        {
            subtract_absolute(other);
        }
        return *this;
    }
    
    Integer& operator*=(const Integer& other)
    {
        std::vector<int> result(m_digits.size() + other.m_digits.size(), 0);
        
        for (std::size_t i = 0; i < m_digits.size(); ++i)
        {
            int carry = 0;
            for (std::size_t j = 0; j < other.m_digits.size() || carry; ++j)
            {
                int product = result[i + j] + m_digits[i] * 
                             (j < other.m_digits.size() ? other.m_digits[j] : 0) + carry;
                result[i + j] = product % 10;
                carry = product / 10;
            }
        }
        
        while (result.size() > 1 && result.back() == 0)
        {
            result.pop_back();
        }
        
        m_digits = std::move(result);
        m_is_positive = (m_is_positive == other.m_is_positive);
        
        if (m_digits.size() == 1 && m_digits[0] == 0)
        {
            m_is_positive = true;
        }
        
        return *this;
    }
    
    Integer& operator/=(const Integer& other)
    {
        Integer remainder;
        divide_and_remainder(other, *this, remainder);
        return *this;
    }
    
    Integer& operator%=(const Integer& other)
    {
        Integer quotient;
        divide_and_remainder(other, quotient, *this);
        return *this;
    }
    
    Integer& operator++()
    {
        return *this += Integer("+1");
    }
    
    Integer operator++(int)
    {
        Integer temp = *this;
        ++*this;
        return temp;
    }
    
    Integer& operator--()
    {
        return *this -= Integer("+1");
    }
    
    Integer operator--(int)
    {
        Integer temp = *this;
        --*this;
        return temp;
    }
    
    friend Integer operator+(Integer lhs, const Integer& rhs)
    {
        lhs += rhs;
        return lhs;
    }
    
    friend Integer operator-(Integer lhs, const Integer& rhs)
    {
        lhs -= rhs;
        return lhs;
    }
    
    friend Integer operator*(Integer lhs, const Integer& rhs)
    {
        lhs *= rhs;
        return lhs;
    }
    
    friend Integer operator/(Integer lhs, const Integer& rhs)
    {
        lhs /= rhs;
        return lhs;
    }
    
    friend Integer operator%(Integer lhs, const Integer& rhs)
    {
        lhs %= rhs;
        return lhs;
    }
    
    friend std::strong_ordering operator<=>(const Integer& lhs, const Integer& rhs)
    {
        if (lhs.sign() != rhs.sign())
        {
            return lhs.sign() <=> rhs.sign();
        }
        
        if (lhs.m_digits.size() != rhs.m_digits.size())
        {
            return (lhs.sign() > 0) ? (lhs.m_digits.size() <=> rhs.m_digits.size())
                                    : (rhs.m_digits.size() <=> lhs.m_digits.size());
        }
        
        for (std::size_t i = 0; i < lhs.m_digits.size(); ++i)
        {
            if (lhs.m_digits[i] != rhs.m_digits[i])
            {
                return (lhs.sign() > 0) ? (lhs.m_digits[i] <=> rhs.m_digits[i])
                                        : (rhs.m_digits[i] <=> lhs.m_digits[i]);
            }
        }
        
        return std::strong_ordering::equal;
    }
    
    friend bool operator==(const Integer& lhs, const Integer& rhs)
    {
        return (lhs <=> rhs) == 0;
    }
    
    friend std::istream& operator>>(std::istream& stream, Integer& integer)
    {
        std::string str;
        stream >> str;
        integer = Integer(str);
        return stream;
    }
    
    friend std::ostream& operator<<(std::ostream& stream, const Integer& integer)
    {
        stream << static_cast<std::string>(integer);
        return stream;
    }
    
    friend Integer pow(Integer base, unsigned int exponent)
    {
        Integer result("+1");
        while (exponent > 0)
        {
            if (exponent & 1)
            {
                result *= base;
            }
            base *= base;
            exponent >>= 1;
        }
        return result;
    }
    
    friend Integer sqrt(const Integer& value)
    {
        if (value.sign() < 0)
        {
            return Integer("-1");
        }
        
        Integer low("+0");
        Integer high = value;
        Integer result("+0");
        
        while (low <= high)
        {
            Integer mid = (low + high) / Integer("+2");
            Integer mid_squared = mid * mid;
            
            if (mid_squared == value)
            {
                return mid;
            }
            else if (mid_squared < value)
            {
                result = mid;
                low = mid + Integer("+1");
            }
            else
            {
                high = mid - Integer("+1");
            }
        }
        
        return result;
    }
    
private:
    void add_absolute(const Integer& other)
    {
        std::size_t max_size = std::max(m_digits.size(), other.m_digits.size());
        std::vector<int> result(max_size + 1, 0);
        
        int carry = 0;
        for (std::size_t i = 0; i < max_size || carry; ++i)
        {
            int sum = carry;
            if (i < m_digits.size())
            {
                sum += m_digits[m_digits.size() - 1 - i];
            }
            if (i < other.m_digits.size())
            {
                sum += other.m_digits[other.m_digits.size() - 1 - i];
            }
            
            result[result.size() - 1 - i] = sum % 10;
            carry = sum / 10;
        }
        
        while (result.size() > 1 && result[0] == 0)
        {
            result.erase(result.begin());
        }
        
        m_digits = result;
    }
    
    void subtract_absolute(const Integer& other)
    {
        bool result_positive = compare_absolute(other) >= 0;
        
        const std::vector<int>& larger = result_positive ? m_digits : other.m_digits;
        const std::vector<int>& smaller = result_positive ? other.m_digits : m_digits;
        
        std::vector<int> result(larger.size(), 0);
        
        int borrow = 0;
        for (std::size_t i = 0; i < larger.size(); ++i)
        {
            int diff = borrow;
            if (i < smaller.size())
            {
                diff += larger[larger.size() - 1 - i] - smaller[smaller.size() - 1 - i];
            }
            else
            {
                diff += larger[larger.size() - 1 - i];
            }
            
            if (diff < 0)
            {
                diff += 10;
                borrow = -1;
            }
            else
            {
                borrow = 0;
            }
            
            result[result.size() - 1 - i] = diff;
        }
        
        while (result.size() > 1 && result[0] == 0)
        {
            result.erase(result.begin());
        }
        
        m_digits = result;
        m_is_positive = result_positive ? m_is_positive : !m_is_positive;
        
        if (m_digits.size() == 1 && m_digits[0] == 0)
        {
            m_is_positive = true;
        }
    }
    
    void divide_and_remainder(const Integer& divisor, Integer& quotient, Integer& remainder) const
    {
        if (divisor.m_digits.size() == 1 && divisor.m_digits[0] == 0)
        {
            return;
        }
        
        remainder = Integer("+0");
        quotient = Integer("+0");
        quotient.m_digits.resize(m_digits.size(), 0);
        
        for (std::size_t i = 0; i < m_digits.size(); ++i)
        {
            remainder.m_digits.push_back(m_digits[i]);
            remainder.remove_leading_zeros();
            
            int count = 0;
            while (remainder.compare_absolute(divisor) >= 0)
            {
                remainder.subtract_absolute(divisor);
                ++count;
            }
            
            quotient.m_digits[i] = count;
        }
        
        quotient.remove_leading_zeros();
        quotient.m_is_positive = (m_is_positive == divisor.m_is_positive);
        
        if (quotient.m_digits.empty())
        {
            quotient.m_digits.push_back(0);
            quotient.m_is_positive = true;
        }
        
        remainder.m_is_positive = m_is_positive;
        if (remainder.m_digits.empty())
        {
            remainder.m_digits.push_back(0);
            remainder.m_is_positive = true;
        }
    }
    
    int compare_absolute(const Integer& other) const
    {
        if (m_digits.size() != other.m_digits.size())
        {
            return m_digits.size() < other.m_digits.size() ? -1 : 1;
        }
        
        for (std::size_t i = 0; i < m_digits.size(); ++i)
        {
            if (m_digits[i] != other.m_digits[i])
            {
                return m_digits[i] < other.m_digits[i] ? -1 : 1;
            }
        }
        
        return 0;
    }
    
    void remove_leading_zeros()
    {
        auto it = std::find_if(m_digits.begin(), m_digits.end(), 
                               [](int digit) { return digit != 0; });
        m_digits.erase(m_digits.begin(), it);
        
        if (m_digits.empty())
        {
            m_digits.push_back(0);
        }
    }
    
    std::vector<int> m_digits;
    bool m_is_positive = true;
};

int main()
{
    Integer x = std::string(32, '1');
    Integer y = std::string(32, '2');

    // Тестирование операций из примера
    assert((x += y) == "+33333333333333333333333333333333"s);
    assert((x -= y) == "+11111111111111111111111111111111"s);
    assert((x *= y) == "+246913580246913580246913580246908641975308641975308641975308642"s);
    assert((x /= y) == "+11111111111111111111111111111111"s);

    assert((x++) == "+11111111111111111111111111111111"s);
    assert((x--) == "+11111111111111111111111111111112"s);
    assert((++y) == "+22222222222222222222222222222223"s);
    assert((--y) == "+22222222222222222222222222222222"s);

    assert((x + y) == "+33333333333333333333333333333333"s);
    assert((x - y) == "-11111111111111111111111111111111"s);
    assert((x * y) == "+246913580246913580246913580246908641975308641975308641975308642"s);
    assert((x / y) == "+0"s);

    assert((x < y) == 1);
    assert((x > y) == 0);
    assert((x <= y) == 1);
    assert((x >= y) == 0);
    assert((x == y) == 0);
    assert((x != y) == 1);

    std::stringstream stream_1(std::string(32, '1'));
    std::stringstream stream_2;

    stream_1 >> x;
    stream_2 << x;

    assert(stream_2.str() == stream_1.str());

    assert(sqrt(x * x) == x);

    {
        Integer a("+12345");
        Integer b("+7");
        
        a %= b;
        assert(a == "+3"s);
        
        Integer c("+12345");
        Integer d = c % b;
        assert(d == "+3"s);
        
        Integer e("+2");
        Integer f = pow(e, 10u);
        assert(f == "+1024"s);
        
        Integer g("+3");
        Integer h = pow(g, 5u);
        assert(h == "+243"s);
        
        Integer i("+10");
        Integer j = pow(i, 3u);
        assert(j == "+1000"s);
        
        Integer k("-2");
        Integer l = pow(k, 3u);
        assert(l == "-8"s);
        
        Integer m("-2");
        Integer n = pow(m, 4u);
        assert(n == "+16"s);
    }
    

    {
        Integer pos("+12345");
        assert(pos.sign() == 1);
        
        Integer neg("-12345");
        assert(neg.sign() == -1);
        
        Integer zero("+0");
        assert(zero.sign() == 0);
        
        Integer pos_abs = neg.abs();
        assert(pos_abs.sign() == 1);
        assert(pos_abs == "+12345"s);
        
        Integer zero_abs = zero.abs();
        assert(zero_abs.sign() == 0);
        assert(zero_abs == "+0"s);
    }
    
    {
        Integer a("+100");
        Integer b("+30");
        a %= b;
        assert(a == "+10"s);
        
        Integer c("+100");
        Integer d("-30");
        c %= d;
        assert(c == "+10"s);
        
        Integer e("-100");
        Integer f("+30");
        e %= f;
        assert(e == "-10"s);
        
        Integer g("-100");
        Integer h("-30");
        g %= h;
        assert(g == "-10"s);
    }
    
    {
        Integer a("+123456");
        Integer b("+1000");
        Integer c = a % b;
        assert(c == "+456"s);
        
        Integer d("+1");
        Integer e("+2");
        Integer f = d % e;
        assert(f == "+1"s);
    }

    {
        Integer a("+12345");
        Integer b("+6789");
        Integer c("+3");
        
        Integer result = (a + b) % c;
        assert(result == "+0"s);
        
        Integer d = pow(a % c + b % c, 2u);
        assert(d == "+4"s);
    }
    
    return 0;
}
