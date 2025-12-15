#include <array>
#include <cassert>
#include <cmath>
#include <format>
#include <iostream>
#include <limits>

consteval double calculate_e(double epsilon) {
    double e = 0.0;
    double term = 1.0;
    int i = 1;
    
    while (term > epsilon && i < 1000) {
        e += term;
        term /= i;
        ++i;
    }
    
    return e;
}

consteval double calculate_pi(double epsilon) {
    double pi = 3.0;
    double term = 4.0 / (2.0 * 3.0 * 4.0);
    int i = 2;
    int sign = 1;
    
    while (std::abs(term) > epsilon && i < 1000) {
        pi += sign * term;
        sign = -sign;
        i += 2;
        term = 4.0 / (static_cast<double>(i) * (i + 1) * (i + 2));
    }
    
    return pi;
}

int main() {
    constexpr std::array<double, 4> epsilons = {1e-1, 1e-3, 1e-6, 1e-9};
    
    constexpr double e1 = calculate_e(epsilons[0]);
    constexpr double e2 = calculate_e(epsilons[1]);
    constexpr double e3 = calculate_e(epsilons[2]);
    
    constexpr double pi1 = calculate_pi(epsilons[0]);
    constexpr double pi2 = calculate_pi(epsilons[1]);
    
    static_assert(e1 > 2.5 && e1 < 2.8);
    static_assert(e2 > 2.71 && e2 < 2.72);
    
    static_assert(pi1 > 3.0 && pi1 < 3.2);
    static_assert(pi2 > 3.14 && pi2 < 3.15);
    
    assert(e1 > 2.5 && e1 < 2.8);
    assert(e2 > 2.71 && e2 < 2.72);
    assert(e3 > 2.71828 && e3 < 2.71829);
    
    assert(pi1 > 3.0 && pi1 < 3.2);
    assert(pi2 > 3.14 && pi2 < 3.15);
    
    const double e4 = calculate_e(epsilons[3]);
    const double pi3 = calculate_pi(epsilons[2]);
    const double pi4 = calculate_pi(epsilons[3]);
    
    assert(e4 > 2.7182818 && e4 < 2.7182819);
    assert(pi3 > 3.14159 && pi3 < 3.14160);
    assert(pi4 > 3.1415926 && pi4 < 3.1415927);
    
    std::cout << std::format("e (epsilon={}): {:.6}\n", epsilons[0], e1);
    std::cout << std::format("e (epsilon={}): {:.6}\n", epsilons[1], e2);
    std::cout << std::format("e (epsilon={}): {:.6}\n", epsilons[2], e3);
    std::cout << std::format("e (epsilon={}): {:.6}\n", epsilons[3], e4);
    
    std::cout << std::format("pi (epsilon={}): {:.6}\n", epsilons[0], pi1);
    std::cout << std::format("pi (epsilon={}): {:.6}\n", epsilons[1], pi2);
    std::cout << std::format("pi (epsilon={}): {:.6}\n", epsilons[2], pi3);
    std::cout << std::format("pi (epsilon={}): {:.6}\n", epsilons[3], pi4);
    
    return 0;
}