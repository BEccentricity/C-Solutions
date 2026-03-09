#include <iostream>

extern "C"
{
    void test()
    {
        std::cout << "Hello from first library (version 1)\n";
    }
}