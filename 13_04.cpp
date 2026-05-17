#include <cassert>
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <print>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

auto make_type(std::filesystem::file_status const& status)
{
    if (std::filesystem::is_directory(status)) { return 'd'; }
    if (std::filesystem::is_regular_file(status)) { return 'f'; }
    if (std::filesystem::is_symlink(status)) { return 'l'; }
    return '?';
}

auto make_permissions(std::filesystem::perms permissions) -> std::string
{
    auto lambda = [permissions](auto x, auto y)
    {
        return (permissions & x) == std::filesystem::perms::none ? '-' : y;
    };

    return {
        lambda(std::filesystem::perms::owner_read,  'r'),
        lambda(std::filesystem::perms::owner_write, 'w'),
        lambda(std::filesystem::perms::owner_exec,  'x')
    };
}

auto size(std::filesystem::path const& path)
{
    auto size = 0uz;
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
    {
        for (auto const& entry : std::filesystem::recursive_directory_iterator(path))
        {
            if (!std::filesystem::is_directory(entry.status()))
            {
                size += std::filesystem::file_size(entry);
            }
        }
    }
    return size;
}

auto size(std::filesystem::directory_entry const& entry)
{
    auto size = 0uz;
    if (std::filesystem::is_regular_file(entry.status()))
    {
        size = std::filesystem::file_size(entry);
    }
    else
    {
        size = ::size(entry.path());
    }

    std::vector<char> vector = { 'B', 'K', 'M', 'G' };
    auto i = 0uz;
    while (i < std::size(vector) && size >= (1 << 10))
    {
        size /= (1 << 10);
        ++i;
    }

    return (std::stringstream() << std::format("{: >4} ({})", size, vector[i])).str();
}

std::vector<std::string> filter_entries(std::filesystem::path const& path, const std::regex& pattern)
{
    std::vector<std::string> matched;

    if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
    {
        for (auto const& entry : std::filesystem::directory_iterator(path))
        {
            std::string filename = entry.path().filename().string();

            if (std::regex_search(filename, pattern))
            {
                matched.push_back(filename);
            }
        }
    }

    return matched;
}

void test()
{
    auto test_dir = std::filesystem::temp_directory_path() / "test_filter_dir";
    std::filesystem::create_directory(test_dir);

    std::ofstream(test_dir / "hello.txt");
    std::ofstream(test_dir / "world.txt");
    std::ofstream(test_dir / "hello.cpp");
    std::ofstream(test_dir / "test.hpp");

    {
        std::regex pattern("hello");
        auto results = filter_entries(test_dir, pattern);
        assert(results.size() == 2);
    }

    {
        std::regex pattern(R"(\.txt$)");
        auto results = filter_entries(test_dir, pattern);
        assert(results.size() == 2);
    }

    {
        std::regex pattern(R"(\.cpp$)");
        auto results = filter_entries(test_dir, pattern);
        assert(results.size() == 1);
        assert(results[0] == "hello.cpp");
    }

    {
        std::regex pattern("nonexistent");
        auto results = filter_entries(test_dir, pattern);
        assert(results.empty());
    }

    std::filesystem::remove_all(test_dir);
}

int main()
{
    test();
    std::cout << "All tests passed.\n";

    std::regex pattern(R"(\.cpp$)");
    std::cout << "Demonstration: filtering current directory for *.cpp files:\n";

    auto matched = filter_entries(std::filesystem::current_path(), pattern);
    for (const auto& name : matched)
    {
        std::cout << "  " << name << '\n';
    }

    return 0;
}