#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

std::string_view longest_palindrome(std::string_view text)
{
    std::size_t n = text.size();
    if (n == 0)
    {
        return std::string_view{};
    }

    std::vector<bool> cache(n * n, false);

    std::size_t max_length = 1;
    std::size_t start = 0;

    for (std::size_t i = 0; i < n; ++i)
    {
        cache[i * n + i] = true;
    }

    for (std::size_t i = 0; i < n - 1; ++i)
    {
        if (text[i] == text[i + 1])
        {
            cache[i * n + (i + 1)] = true;
            start = i;
            max_length = 2;
        }
    }

    for (std::size_t len = 3; len <= n; ++len)
    {
        for (std::size_t i = 0; i <= n - len; ++i)
        {
            std::size_t j = i + len - 1;

            if (text[i] == text[j] && cache[(i + 1) * n + (j - 1)])
            {
                cache[i * n + j] = true;
                if (len > max_length)
                {
                    start = i;
                    max_length = len;
                }
            }
        }
    }

    return text.substr(start, max_length);
}

void test()
{
    assert(longest_palindrome("babad") == "bab");
    assert(longest_palindrome("cbbd") == "bb");
    assert(longest_palindrome("a") == "a");
    assert(longest_palindrome("ac") == "a");
    assert(longest_palindrome("racecar") == "racecar");
    assert(longest_palindrome("").empty());
}

int main()
{
    test();
    std::cout << "Tests passed.\n";
    std::cout << "Demonstration:\n";
    std::cout << "Longest palindrome in 'babad': " << longest_palindrome("babad") << '\n';
    std::cout << "Longest palindrome in 'cbbd': " << longest_palindrome("cbbd") << '\n';
    std::cout << "Longest palindrome in 'racecar': " << longest_palindrome("racecar") << '\n';
    return 0;
}