#include <cassert>
#include <numeric>


constexpr int gcd_recursive(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd_recursive(b, a % b);
}

constexpr int gcd_iterative(int a, int b) {
    while (b != 0) {
        const int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

constexpr int lcm_custom(int a, int b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    return (a / gcd_iterative(a, b)) * b;
}

int main() {
    //compile-time)
    static_assert(gcd_recursive(100, 10) == 10);
    static_assert(gcd_iterative(100, 10) == 10);
    static_assert(lcm_custom(4, 6) == 12);
    
    static_assert(gcd_recursive(17, 13) == 1);
    static_assert(gcd_iterative(17, 13) == 1);
    static_assert(lcm_custom(17, 13) == 221);
    
    static_assert(gcd_recursive(0, 5) == 5);
    static_assert(gcd_iterative(5, 0) == 5);
    static_assert(lcm_custom(0, 5) == 0);

    // (run-time)
    assert(gcd_recursive(1071, 462) == 21);
    assert(gcd_iterative(1071, 462) == 21);
    assert(lcm_custom(21, 6) == 42);
    
    assert(gcd_recursive(100, 25) == 25);
    assert(gcd_iterative(100, 25) == 25);
    assert(lcm_custom(100, 25) == 100);
    
    assert(gcd_recursive(1, 1) == 1);
    assert(gcd_iterative(1, 1) == 1);
    assert(lcm_custom(1, 1) == 1);


    assert(gcd_recursive(48, 18) == std::gcd(48, 18));
    assert(gcd_iterative(48, 18) == std::gcd(48, 18));
    assert(lcm_custom(4, 6) == std::lcm(4, 6));
    
    assert(gcd_recursive(0, 7) == std::gcd(0, 7));
    assert(gcd_iterative(7, 0) == std::gcd(7, 0));
    assert(lcm_custom(0, 7) == std::lcm(0, 7));


    constexpr int demo_a = 56;
    constexpr int demo_b = 98;
    
    constexpr int demo_gcd_rec = gcd_recursive(demo_a, demo_b);
    constexpr int demo_gcd_iter = gcd_iterative(demo_a, demo_b);
    constexpr int demo_lcm = lcm_custom(demo_a, demo_b);
    
    static_assert(demo_gcd_rec == 14);
    static_assert(demo_gcd_iter == 14);
    static_assert(demo_lcm == 392);

    return 0;
}