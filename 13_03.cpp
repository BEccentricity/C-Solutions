#include <cctype>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <print>
#include <sstream>
#include <string>

void transform(const std::string& path_1, const std::string& path_2)
{
    auto string = (std::stringstream() << std::fstream(path_1, std::ios::in).rdbuf()).str();

    for (auto iterator = std::begin(string); iterator != std::end(string); ++iterator)
    {
        if (*iterator == 'R' && std::next(iterator) != std::end(string) &&
            *std::next(iterator) == '\"')
        {
            auto delimiter_start = std::next(iterator, 2);
            auto delimiter_end = std::find(delimiter_start, std::end(string), '(');

            if (delimiter_end != std::end(string))
            {
                std::string delimiter(delimiter_start, delimiter_end);
                std::string end_marker = ")" + delimiter + "\"";

                auto raw_end = std::search(delimiter_end + 1, std::end(string),
                                            end_marker.begin(), end_marker.end());

                if (raw_end != std::end(string))
                {
                    iterator = raw_end + end_marker.size() - 1;
                    continue;
                }
            }
        }

        if (*iterator == '\'')
        {
            do
            {
                ++iterator;
            }
            while (!(*iterator == '\'' && *std::prev(iterator) != '\\'));
        }

        if (*iterator == '\"')
        {
            do
            {
                ++iterator;
            }
            while (!(*iterator == '\"' && *std::prev(iterator) != '\\'));
        }

        if (*iterator == '/')
        {
            if (*std::next(iterator) == '/')
            {
                auto end = std::next(iterator, 2);

                while (end != std::end(string) && *end != '\n')
                {
                    ++end;
                }

                iterator = string.erase(iterator, end);
            }
            else if (*std::next(iterator) == '*')
            {
                auto end = std::next(iterator, 3);

                while (!(*end == '/' && *std::prev(end) == '*'))
                {
                    ++end;
                }

                iterator = string.erase(iterator, ++end);
            }
        }

        if (iterator == std::end(string))
        {
            break;
        }
    }

    std::string result;
    std::istringstream input(string);
    std::string line;

    while (std::getline(input, line, '\n'))
    {
        bool only_whitespace = true;
        for (char ch : line)
        {
            if (!std::isspace(static_cast<unsigned char>(ch)))
            {
                only_whitespace = false;
                break;
            }
        }

        if (!only_whitespace)
        {
            result += line + '\n';
        }
    }

    std::fstream(path_2, std::ios::out) << result;
}

int main()
{
    const std::string path_1 = "test_source.cpp";
    const std::string path_2 = "output_clean.cpp";

    {
        std::fstream out(path_1, std::ios::out);
        out << "// comment\n";
        out << "int main()\n";
        out << "{\n";
        out << "    \n";
        out << "    int x = 0;\n";
        out << "\t\t\n";
        out << "    auto s = R\"(raw\nstring\n)\";\n";
        out << "    return 0;\n";
        out << "}\n";
        out << "    \n";
    }

    transform(path_1, path_2);

    std::print("Transformation complete.\n");

    std::print("Output file contents:\n");
    std::fstream result(path_2, std::ios::in);
    std::string line;
    while (std::getline(result, line, '\n'))
    {
        std::print("{}\n", line);
    }

    std::filesystem::remove(path_1);
    std::filesystem::remove(path_2);

    return 0;
}