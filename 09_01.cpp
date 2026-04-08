#include <iostream>
#include <source_location>

class Tracer
{
public:
    Tracer(const std::source_location location = std::source_location::current())
        : m_location(location)
    {
        std::cout << "Enter: " << m_location.function_name() << " (" 
                  << m_location.file_name() << ":" << m_location.line() << ")\n";
    }

    ~Tracer()
    {
        std::cout << "Exit: " << m_location.function_name() << "\n";
    }

private:
    std::source_location m_location;
};

#ifndef NDEBUG
    #define trace Tracer tracer##__LINE__(std::source_location::current())
#else
    #define trace
#endif

void foo()
{
    trace;
    std::cout << "  foo body\n";
}

void bar(int value)
{
    trace;
    std::cout << "  bar body with value = " << value << "\n";
    foo();
}

int main()
{
    trace;
    std::cout << "main body start\n";
    bar(42);
    std::cout << "main body end\n";
    return 0;
}