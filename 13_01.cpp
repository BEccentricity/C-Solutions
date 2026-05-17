#include <cassert>
#include <cstdint>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string to_hex_string(const std::vector<std::uint8_t>& data)
{
    std::stringstream stream;
    stream << std::hex << std::setfill('0');

    for (std::size_t i = 0; i < data.size(); ++i)
    {
        stream << std::setw(2) << std::right
               << static_cast<unsigned int>(data[i]);
    }

    return stream.str();
}

std::vector<std::uint8_t> from_hex_string(const std::string& hex_string)
{
    std::vector<std::uint8_t> result;

    for (std::size_t i = 0; i < hex_string.length(); i += 2)
    {
        char high_char = hex_string[i];
        char low_char = hex_string[i + 1];

        std::uint8_t high_nibble = 0;
        std::uint8_t low_nibble = 0;

        if (high_char >= '0' && high_char <= '9')
        {
            high_nibble = static_cast<std::uint8_t>(high_char - '0');
        }
        else if (high_char >= 'a' && high_char <= 'f')
        {
            high_nibble = static_cast<std::uint8_t>(high_char - 'a' + 10);
        }

        if (low_char >= '0' && low_char <= '9')
        {
            low_nibble = static_cast<std::uint8_t>(low_char - '0');
        }
        else if (low_char >= 'a' && low_char <= 'f')
        {
            low_nibble = static_cast<std::uint8_t>(low_char - 'a' + 10);
        }

        std::uint8_t byte = static_cast<std::uint8_t>((high_nibble << 4) | low_nibble);
        result.push_back(byte);
    }

    return result;
}

void test()
{
    std::vector<std::uint8_t> data = {0x00, 0x1a, 0x2b, 0x3c, 0x4d, 0x5e, 0x6f,
                                      0x7a, 0x8b, 0x9c, 0xad, 0xbe, 0xcf, 0xff};

    std::string hex = to_hex_string(data);
    assert(hex == "001a2b3c4d5e6f7a8b9cadbecfff");

    auto restored = from_hex_string(hex);
    assert(restored == data);

    std::vector<std::uint8_t> empty_data;
    assert(to_hex_string(empty_data).empty());
    assert(from_hex_string("").empty());
}

int main()
{
    test();
    std::cout << "Tests passed.\n";

    std::vector<std::uint8_t> demo = {0xca, 0xfe, 0xba, 0xbe};
    std::string hex_demo = to_hex_string(demo);
    std::cout << "Hex: " << hex_demo << '\n';

    auto restored_demo = from_hex_string(hex_demo);
    std::cout << "Restored: ";
    for (auto byte : restored_demo)
    {
        std::cout << static_cast<unsigned int>(byte) << ' ';
    }
    std::cout << '\n';

    return 0;
}