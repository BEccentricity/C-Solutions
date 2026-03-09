#include <iostream>

extern "C"
{
    void test()
    {
        std::cout << "Hello from second library (version 2)\n";
    }
}