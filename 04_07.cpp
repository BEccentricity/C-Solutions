#include <cassert>
#include <numeric>
#include <type_traits>

template <int N = 0, int D = 1>
struct Ratio
{
    constexpr static auto num = N;
    constexpr static auto den = D;
};

template <typename R1, typename R2>
struct Sum
{
    constexpr static auto num = R1::num * R2::den + R2::num * R1::den;
    constexpr static auto den = R1::den * R2::den;
    
    static constexpr auto gcd = std::gcd(num, den);
    static constexpr auto reduced_num = num / gcd;
    static constexpr auto reduced_den = den / gcd;
    
    using type = Ratio<reduced_num, reduced_den>;
};

template <typename R1, typename R2>
struct Mul
{
    constexpr static auto num = R1::num * R2::num;
    constexpr static auto den = R1::den * R2::den;
    
    static constexpr auto gcd = std::gcd(num, den);
    static constexpr auto reduced_num = num / gcd;
    static constexpr auto reduced_den = den / gcd;
    
    using type = Ratio<reduced_num, reduced_den>;
};

template <typename R1, typename R2>
struct Div
{
    static_assert(R2::num != 0, "Division by zero");
    
    constexpr static auto num = R1::num * R2::den;
    constexpr static auto den = R1::den * R2::num;
    
    static constexpr auto gcd = std::gcd(num, den);
    static constexpr auto reduced_num = num / gcd;
    static constexpr auto reduced_den = den / gcd;
    
    using type = Ratio<reduced_num, reduced_den>;
};

template <typename R1, typename R2>
struct Sub
{
    using NegativeR2 = Ratio<-R2::num, R2::den>;
    using type = typename Sum<R1, NegativeR2>::type;
};

template <typename R1, typename R2>
using sum = typename Sum<R1, R2>::type;

template <typename R1, typename R2>
using mul = typename Mul<R1, R2>::type;

template <typename R1, typename R2>
using division = typename Div<R1, R2>::type;

template <typename R1, typename R2>
using sub = typename Sub<R1, R2>::type;

template <typename T, typename R = Ratio<1>>
struct Duration
{
    using RatioType = R;
    T x = T();
};

template <typename T1, typename R1, typename T2, typename R2>
constexpr auto operator+(Duration<T1, R1> const & lhs, Duration<T2, R2> const & rhs)
{
    using ratio_t = sum<R1, R2>;
    
    auto x = lhs.x * (ratio_t::den / R1::den) * R1::num + 
             rhs.x * (ratio_t::den / R2::den) * R2::num;
    
    return Duration<decltype(x), ratio_t>(x);
}

template <typename T1, typename R1, typename T2, typename R2>
constexpr auto operator-(Duration<T1, R1> const & lhs, Duration<T2, R2> const & rhs)
{
    using ratio_t = sub<R1, R2>;
    
    auto x = lhs.x * (ratio_t::den / R1::den) * R1::num - 
             rhs.x * (ratio_t::den / R2::den) * R2::num;
    
    return Duration<decltype(x), ratio_t>(x);
}

template <typename T1, typename R1, typename T2, typename R2>
constexpr auto operator*(Duration<T1, R1> const & lhs, Duration<T2, R2> const & rhs)
{
    using ratio_t = mul<R1, R2>;
    
    auto common_den = R1::den * R2::den;
    auto x = (lhs.x * rhs.x * ratio_t::den * R1::num * R2::num) / common_den;
    
    return Duration<decltype(x), ratio_t>(x);
}

template <typename T1, typename R1, typename T2, typename R2>
constexpr auto operator/(Duration<T1, R1> const & lhs, Duration<T2, R2> const & rhs)
{
    using ratio_t = division<R1, R2>;
    
    auto x = (lhs.x * ratio_t::den * R2::num) / (rhs.x * ratio_t::num * R2::den);
    
    return Duration<decltype(x), ratio_t>(x);
}

int main()
{
    static_assert(std::is_same_v<sum<Ratio<1,2>, Ratio<1,3>>, Ratio<5,6>>);
    static_assert(std::is_same_v<sub<Ratio<1,2>, Ratio<1,3>>, Ratio<1,6>>);
    static_assert(std::is_same_v<mul<Ratio<1,2>, Ratio<2,3>>, Ratio<1,3>>);
    static_assert(std::is_same_v<division<Ratio<1,2>, Ratio<2,3>>, Ratio<3,4>>);
    
    static_assert(std::is_same_v<sum<Ratio<2,4>, Ratio<2,6>>, Ratio<5,6>>);
    static_assert(std::is_same_v<mul<Ratio<3,9>, Ratio<6,8>>, Ratio<1,4>>);
    
    auto x = 1, y = 2;
    
    Duration<int, Ratio<1,2>> duration_1(x);
    Duration<int, Ratio<1,3>> duration_2(y);
    
    auto duration_3 = duration_1 + duration_2;
    static_assert(std::is_same_v<decltype(duration_3)::RatioType, Ratio<5,6>>);
    assert(duration_3.x == 7);
    
    auto duration_4 = duration_1 - duration_2;
    static_assert(std::is_same_v<decltype(duration_4)::RatioType, Ratio<1,6>>);
    assert(duration_4.x == -1);
    
    auto duration_5 = duration_1 * duration_2;
    static_assert(std::is_same_v<decltype(duration_5)::RatioType, Ratio<1,6>>);
    assert(duration_5.x == 2);
    
    auto duration_6 = duration_1 / duration_2;
    static_assert(std::is_same_v<decltype(duration_6)::RatioType, Ratio<3,2>>);
    assert(duration_6.x == 0);
    
    Duration<int, Ratio<3,4>> duration_7(4);
    Duration<int, Ratio<2,5>> duration_8(5);
    
    auto duration_9 = duration_7 * duration_8;
    assert(duration_9.x == 60);
    
    Duration<int, Ratio<1,2>> duration_10(6);
    Duration<int, Ratio<1,3>> duration_11(3);
    
    auto duration_12 = duration_10 * duration_11;
    assert(duration_12.x == 18);
    
    return 0;
}