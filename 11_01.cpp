#include <cassert>
#include <type_traits>

auto test(int x)
{
    return x;
}

auto invoke(int(*function)(int), int x)
{
    return function(x);
}

class Wrapper
{
public:
    Wrapper(int(*func)(int)) : m_func(func) {}

    using FuncPtr = int(*)(int);

    operator FuncPtr() const
    {
        return m_func;
    }

    Wrapper operator*()
    {
        return *this;
    }

    Wrapper* operator->()
    {
        return this;
    }

private:
    int(*m_func)(int);
};

int main()
{
    static_assert(std::is_same_v < decltype( test), int   (int) > );
    static_assert(std::is_same_v < decltype(&test), int(*)(int) > );

    assert(invoke(test, 1) == 1);

    Wrapper function = test;
    int(*ptr)(int) = function;
    ptr(42);
    (*function)->operator Wrapper::FuncPtr()(42);

    return 0;
}