#include <cassert>
#include <climits>
#include <type_traits>

template <int N>
struct Fibonacci {
    static_assert(N >= 1, "N must be positive");
    
    static constexpr int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
    
    static_assert(value > 0 && (value > Fibonacci<N - 1>::value || N <= 2), 
                  "Integer overflow detected");
};

template <>
struct Fibonacci<1> {
    static constexpr int value = 1;
};

template <>
struct Fibonacci<2> {
    static constexpr int value = 1;
};

template <int N>
constexpr int Fibonacci_v = Fibonacci<N>::value;

int main() {
    static_assert(Fibonacci_v<1> == 1);
    static_assert(Fibonacci_v<2> == 1);
    static_assert(Fibonacci_v<3> == 2);
    static_assert(Fibonacci_v<4> == 3);
    static_assert(Fibonacci_v<5> == 5);
    static_assert(Fibonacci_v<6> == 8);
    static_assert(Fibonacci_v<7> == 13);
    static_assert(Fibonacci_v<8> == 21);
    static_assert(Fibonacci_v<9> == 34);
    static_assert(Fibonacci_v<10> == 55);
    
    assert(Fibonacci_v<1> == 1);
    assert(Fibonacci_v<2> == 1);
    assert(Fibonacci_v<3> == 2);
    assert(Fibonacci_v<4> == 3);
    assert(Fibonacci_v<5> == 5);
    assert(Fibonacci_v<6> == 8);
    assert(Fibonacci_v<7> == 13);
    assert(Fibonacci_v<8> == 21);
    assert(Fibonacci_v<9> == 34);
    assert(Fibonacci_v<10> == 55);

    return 0;
}