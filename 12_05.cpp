#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>

class Stream
{
public:
    using token_t = std::variant<char, double, std::string>;

    Stream(std::string const& string) : m_stream(string + ';') {}

    auto empty()
    {
        return m_stream.peek() == ';';
    }

    auto get()
    {
        if (m_has_token)
        {
            m_has_token = false;
            return m_token;
        }

        auto x = '\0';
        m_stream >> x;

        switch (x)
        {
            case '+': case '-': case '*': case '/': case '%':
            case '^': case '!': case '(': case ')': case '[':
            case ']': case '{': case '}': case ';':
            {
                return token_t(x);
            }

            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            case '.':
            {
                m_stream.unget();
                auto y = 0.0;
                m_stream >> y;
                return token_t(y);
            }

            default:
            {
                std::string string(1, x);
                while (m_stream.get(x) && (std::isalpha(x) || std::isdigit(x)))
                {
                    string += x;
                }
                if (!std::isspace(x))
                {
                    m_stream.unget();
                }
                return token_t(string);
            }
        }
    }

    void put(token_t const& token)
    {
        m_token = token;
        m_has_token = true;
    }

private:
    std::stringstream m_stream;
    token_t m_token;
    bool m_has_token = false;
};

class Calculator
{
public:
    double evaluate(const std::string& expression)
    {
        Stream stream(expression);
        if (!stream.empty())
        {
            return statement(stream);
        }
        return 0.0;
    }

private:
    double factorial(double n) const
    {
        if (n < 0.0) return 0.0;
        if (n == 0.0 || n == 1.0) return 1.0;
        double result = 1.0;
        for (int i = 2; i <= static_cast<int>(n); ++i)
        {
            result *= static_cast<double>(i);
        }
        return result;
    }

    auto statement(Stream& stream) -> double
    {
        auto token = stream.get();
        if (std::holds_alternative<std::string>(token))
        {
            if (std::get<std::string>(token) == "set")
            {
                return declaration(stream);
            }
        }
        stream.put(token);
        return expression(stream);
    }

    auto declaration(Stream& stream) -> double
    {
        auto string = std::get<std::string>(stream.get());
        m_variables[string] = expression(stream);
        return m_variables[string];
    }

    auto expression(Stream& stream) const -> double
    {
        auto x = term(stream);
        auto token = stream.get();
        while (true)
        {
            switch (std::get<char>(token))
            {
                case '+': { x += term(stream); break; }
                case '-': { x -= term(stream); break; }
                default:
                {
                    stream.put(token);
                    return x;
                }
            }
            token = stream.get();
        }
    }

    auto term(Stream& stream) const -> double
    {
        auto x = power(stream);
        auto token = stream.get();
        while (true)
        {
            switch (std::get<char>(token))
            {
                case '*': { x *= power(stream); break; }
                case '/': { x /= power(stream); break; }
                case '%':
                {
                    auto y = power(stream);
                    x = std::fmod(x, y);
                    break;
                }
                default:
                {
                    stream.put(token);
                    return x;
                }
            }
            token = stream.get();
        }
    }

    auto power(Stream& stream) const -> double
    {
        auto x = factorial_op(stream);
        auto token = stream.get();
        while (true)
        {
            switch (std::get<char>(token))
            {
                case '^': { x = std::pow(x, factorial_op(stream)); break; }
                default:
                {
                    stream.put(token);
                    return x;
                }
            }
            token = stream.get();
        }
    }

    auto factorial_op(Stream& stream) const -> double
    {
        auto x = primary(stream);
        auto token = stream.get();
        if (std::holds_alternative<char>(token) && std::get<char>(token) == '!')
        {
            return factorial(x);
        }
        stream.put(token);
        return x;
    }

    auto primary(Stream& stream) const -> double
    {
        auto token = stream.get();
        if (std::holds_alternative<char>(token))
        {
            switch (std::get<char>(token))
            {
                case '(':
                {
                    auto x = expression(stream);
                    stream.get();
                    return x;
                }

                case '[':
                {
                    auto x = expression(stream);
                    auto closing = stream.get();
                    if (std::holds_alternative<char>(closing) && std::get<char>(closing) == ']')
                    {
                        return x;
                    }
                    return x;
                }

                case '{':
                {
                    auto x = expression(stream);
                    auto closing = stream.get();
                    if (std::holds_alternative<char>(closing) && std::get<char>(closing) == '}')
                    {
                        return x;
                    }
                    return x;
                }

                case '+': { return factorial_op(stream); }
                case '-': { return -1 * factorial_op(stream); }
            }
        }

        if (std::holds_alternative<double>(token))
        {
            return std::get<double>(token);
        }

        return m_variables.at(std::get<std::string>(token));
    }

    std::unordered_map<std::string, double> m_variables;
};

void test()
{
    Calculator calc;
    const double epsilon = 1e-6;

    assert(std::abs(calc.evaluate("2 + 3") - 5.0) < epsilon);
    assert(std::abs(calc.evaluate("10 - 4") - 6.0) < epsilon);
    assert(std::abs(calc.evaluate("3 * 7") - 21.0) < epsilon);
    assert(std::abs(calc.evaluate("15 / 3") - 5.0) < epsilon);
    assert(std::abs(calc.evaluate("10 % 3") - 1.0) < epsilon);
    assert(std::abs(calc.evaluate("2 ^ 8") - 256.0) < epsilon);
    assert(std::abs(calc.evaluate("5!") - 120.0) < epsilon);
    assert(std::abs(calc.evaluate("2 + 3 * 4") - 14.0) < epsilon);
    assert(std::abs(calc.evaluate("(2 + 3) * 4") - 20.0) < epsilon);
    assert(std::abs(calc.evaluate("[1 + 2] * {3 + 4}") - 21.0) < epsilon);
    assert(std::abs(calc.evaluate("-5") - (-5.0)) < epsilon);
    assert(std::abs(calc.evaluate("+5") - 5.0) < epsilon);
    assert(std::abs(calc.evaluate("2 ^ 3 ^ 2") - 512.0) < epsilon);
    assert(std::abs(calc.evaluate("4! + 3") - 27.0) < epsilon);
}

int main()
{
    test();
    std::cout << "All tests passed.\n";

    Calculator calc;
    std::cout << "Demo: 2 + 3 * 4 = " << calc.evaluate("2 + 3 * 4") << '\n';
    std::cout << "Demo: 10 % 3 = " << calc.evaluate("10 % 3") << '\n';
    std::cout << "Demo: 2 ^ 8 = " << calc.evaluate("2 ^ 8") << '\n';
    std::cout << "Demo: 5! = " << calc.evaluate("5!") << '\n';
    std::cout << "Demo: [1 + 2] * {3 + 4} = " << calc.evaluate("[1 + 2] * {3 + 4}") << '\n';

    return 0;
}