#include <functional>
#include <iostream>
#include <string>

#include <boost/dll.hpp>
#include <boost/dll/import.hpp>

int main()
{
    std::cout << "Enter library name (libfirst.so or libsecond.so): ";
    std::string library_name;
    std::cin >> library_name;

    try
    {
        auto test_function = boost::dll::import_symbol<void()>(
            library_name,
            "test",
            boost::dll::load_mode::default_mode
        );

        std::cout << "Calling test() from " << library_name << ": ";
        test_function();
    }
    catch (const boost::system::system_error& e)
    {
        std::cerr << "Error loading library or symbol: " << e.what() << '\n';
        return 1;
    }

    return 0;
}