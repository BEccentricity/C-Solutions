#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

template <typename... Ts> 
class Tuple 
{};

template <typename T, typename... Ts> 
class Tuple <T, Ts...>
{
public:
    constexpr Tuple(T && x, Ts && ... xs)
    :
        m_head(std::forward<T>(x)),
        m_tail(std::forward<Ts>(xs)...)
    {}

    constexpr Tuple(const T & x, const Ts & ... xs)
    :
        m_head(x),
        m_tail(xs...)
    {}

    template <std::size_t I> 
    constexpr auto get() const
    {
        if constexpr (I == 0)
        {
            return m_head;
        }
        else
        {
            return m_tail.template get<I - 1>();
        }
    }

    constexpr std::size_t size() const
    {
        return sizeof...(Ts) + 1;
    }

private:
    T m_head;
    Tuple<Ts...> m_tail;
};

template <>
class Tuple<>
{
public:
    constexpr Tuple() = default;
    
    constexpr std::size_t size() const
    {
        return 0;
    }
};

int main()
{

    constexpr Tuple<int, double, char> tuple(1, 2.0, 'A');
    
    static_assert(tuple.get<0>() == 1);
    static_assert(tuple.get<1>() == 2.0);
    static_assert(tuple.get<2>() == 'A');
    static_assert(tuple.size() == 3);
    
    constexpr Tuple<int> single_tuple(42);
    static_assert(single_tuple.get<0>() == 42);
    static_assert(single_tuple.size() == 1);
    
    constexpr Tuple<int, double, char, float> four_tuple(10, 3.14, 'X', 2.5f);
    static_assert(four_tuple.get<0>() == 10);
    static_assert(four_tuple.get<1>() == 3.14);
    static_assert(four_tuple.get<2>() == 'X');
    static_assert(four_tuple.get<3>() == 2.5f);
    static_assert(four_tuple.size() == 4);
    
    constexpr Tuple<> empty_tuple;
    static_assert(empty_tuple.size() == 0);

    Tuple<int, double, std::string> runtime_tuple(1, 2.0, "aaaaa");
    assert(runtime_tuple.get<0>() == 1);
    assert(runtime_tuple.get<1>() == 2.0);
    assert(runtime_tuple.get<2>() == "aaaaa");
    assert(runtime_tuple.size() == 3);
    
    constexpr Tuple<int, double, const char*> cstring_tuple(1, 2.0, "aaaaa");
    static_assert(cstring_tuple.get<0>() == 1);
    static_assert(cstring_tuple.get<1>() == 2.0);

    static_assert(cstring_tuple.get<2>()[0] == 'a');
    static_assert(cstring_tuple.size() == 3);
    
    return 0;
}