#include <cstdint>
#include <iostream>
#include <array>
#include <sstream>
#include <cassert>

class IPv4 {
private:
    std::array<std::uint8_t, 4> data;

public:
    IPv4() : data{0, 0, 0, 0} {}
    
    IPv4(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d) : data{a, b, c, d} {}
    
    IPv4& operator++() {
        for (int i = 3; i >= 0; --i) {
            if (data[i] < 255) {
                ++data[i];
                break;
            } else {
                data[i] = 0;
            }
        }
        return *this;
    }
    
    IPv4 operator++(int) {
        IPv4 temp = *this;
        ++(*this);
        return temp;
    }
    
    IPv4& operator--() {
        for (int i = 3; i >= 0; --i) {
            if (data[i] > 0) {
                --data[i];
                break;
            } else {
                data[i] = 255;
            }
        }
        return *this;
    }
    
    IPv4 operator--(int) {
        IPv4 temp = *this;
        --(*this);
        return temp;
    }
    
    friend bool operator==(const IPv4& lhs, const IPv4& rhs) {
        return lhs.data == rhs.data;
    }
    
    friend bool operator<(const IPv4& lhs, const IPv4& rhs) {
        return lhs.data < rhs.data;
    }
    
    friend bool operator>(const IPv4& lhs, const IPv4& rhs) {
        return rhs < lhs;
    }
    
    friend bool operator<=(const IPv4& lhs, const IPv4& rhs) {
        return !(rhs < lhs);
    }
    
    friend bool operator>=(const IPv4& lhs, const IPv4& rhs) {
        return !(lhs < rhs);
    }
    
    friend std::ostream& operator<<(std::ostream& os, const IPv4& ip) {
        os << static_cast<int>(ip.data[0]) << '.' 
           << static_cast<int>(ip.data[1]) << '.' 
           << static_cast<int>(ip.data[2]) << '.' 
           << static_cast<int>(ip.data[3]);
        return os;
    }
    
    friend std::istream& operator>>(std::istream& is, IPv4& ip) {
        int a, b, c, d;
        char dot1, dot2, dot3;
        
        is >> a >> dot1 >> b >> dot2 >> c >> dot3 >> d;
        
        if (dot1 == '.' && dot2 == '.' && dot3 == '.' &&
            a >= 0 && a <= 255 && b >= 0 && b <= 255 && 
            c >= 0 && c <= 255 && d >= 0 && d <= 255) {
            ip.data[0] = static_cast<std::uint8_t>(a);
            ip.data[1] = static_cast<std::uint8_t>(b);
            ip.data[2] = static_cast<std::uint8_t>(c);
            ip.data[3] = static_cast<std::uint8_t>(d);
        } else {
            is.setstate(std::ios::failbit);
        }
        
        return is;
    }
};

int main() {
    IPv4 ip1(192, 168, 1, 1);
    IPv4 ip2(192, 168, 1, 1);
    IPv4 ip3(192, 168, 1, 2);
    
    assert(ip1 == ip2);
    assert(ip1 != ip3);
    assert(ip1 < ip3);
    assert(ip3 > ip1);
    
    IPv4 ip4(192, 168, 1, 255);
    ++ip4;
    assert(ip4 == IPv4(192, 168, 2, 0));
    
    IPv4 ip5(192, 168, 2, 0);
    --ip5;
    assert(ip5 == IPv4(192, 168, 1, 255));
    
    IPv4 ip6(255, 255, 255, 255);
    ++ip6;
    assert(ip6 == IPv4(0, 0, 0, 0));
    
    IPv4 ip7(0, 0, 0, 0);
    --ip7;
    assert(ip7 == IPv4(255, 255, 255, 255));
    
    std::stringstream ss1;
    ss1 << IPv4(10, 20, 30, 40);
    assert(ss1.str() == "10.20.30.40");
    
    std::stringstream ss2("172.16.254.1");
    IPv4 ip8;
    ss2 >> ip8;
    assert(ip8 == IPv4(172, 16, 254, 1));

    std::cout<<ip8;
    
    return 0;
}