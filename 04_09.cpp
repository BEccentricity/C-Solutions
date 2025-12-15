#include <cassert>
#include <type_traits>

template <typename T>
struct is_class {
private:
    template <typename U>
    static char test(int U::*);
    
    template <typename>
    static int test(...);
    
public:
    static constexpr bool value = sizeof(test<T>(0)) == sizeof(char);
};

template <typename T>
inline constexpr bool is_class_v = is_class<T>::value;

template <typename T>
struct add_const {
    using type = const T;
};

template <typename T>
using add_const_t = typename add_const<T>::type;

template <typename T>
struct remove_const {
    using type = T;
};

template <typename T>
struct remove_const<const T> {
    using type = T;
};

template <typename T>
using remove_const_t = typename remove_const<T>::type;

template <typename T>
struct decay {
private:
    using U = typename remove_const<typename std::remove_reference<T>::type>::type;
    
public:
    using type = U;
};

template <typename T>
struct decay<T[]> {
    using type = T*;
};

template <typename T, std::size_t N>
struct decay<T[N]> {
    using type = T*;
};

template <typename T>
struct decay<T&> {
    using type = typename decay<T>::type;
};

template <typename T>
struct decay<T&&> {
    using type = typename decay<T>::type;
};

template <typename T>
using decay_t = typename decay<T>::type;

template <bool B, typename T, typename F>
struct conditional {
    using type = T;
};

template <typename T, typename F>
struct conditional<false, T, F> {
    using type = F;
};

template <bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;

int main() {
    class TestClass {};
    struct TestStruct {};
    
    static_assert(is_class_v<TestClass>);
    static_assert(is_class_v<TestStruct>);
    static_assert(!is_class_v<int>);
    static_assert(!is_class_v<double>);
    static_assert(!is_class_v<int*>);
    static_assert(!is_class_v<void>);
    
    static_assert(std::is_same_v<add_const_t<int>, const int>);
    static_assert(std::is_same_v<add_const_t<const int>, const int>);
    static_assert(std::is_same_v<add_const_t<int*>, int* const>);
    
    static_assert(std::is_same_v<remove_const_t<const int>, int>);
    static_assert(std::is_same_v<remove_const_t<int>, int>);
    static_assert(std::is_same_v<remove_const_t<const int*>, const int*>);
    static_assert(std::is_same_v<remove_const_t<int* const>, int*>);
    
    static_assert(std::is_same_v<decay_t<int>, int>);
    static_assert(std::is_same_v<decay_t<const int>, int>);
    static_assert(std::is_same_v<decay_t<int[]>, int*>);
    static_assert(std::is_same_v<decay_t<int[10]>, int*>);
    static_assert(std::is_same_v<decay_t<int&>, int>);
    static_assert(std::is_same_v<decay_t<int&&>, int>);
    static_assert(std::is_same_v<decay_t<const int&>, int>);
    
    static_assert(std::is_same_v<conditional_t<true, int, double>, int>);
    static_assert(std::is_same_v<conditional_t<false, int, double>, double>);
    
    assert(is_class_v<TestClass>);
    assert(is_class_v<TestStruct>);
    assert(!is_class_v<int>);
    
    return 0;
}