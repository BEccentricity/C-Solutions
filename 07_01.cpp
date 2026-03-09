#include <iostream>
#include <cmath>
#include <optional>
#include <variant>
#include <utility>

using Roots = std::variant<std::monostate, double, std::pair<double, double>>;
using Result = std::optional<Roots>;

Result solve(double a, double b, double c)
{
    const double eps = 1e-6;

    if (std::abs(a) < eps)
    {
        if (std::abs(b) < eps)
        {
            if (std::abs(c) < eps)
            {
                return Roots{std::monostate{}};
            }
            else
            {
                return std::nullopt;
            }
        }
        else
        {
            double root = -c / b;
            return Roots{root};
        }
    }
    else
    {
        double D = b * b - 4.0 * a * c;

        if (std::abs(D) < eps)
        {
            double root = -b / (2.0 * a);
            return Roots{root};
        }
        else if (D > 0.0)
        {
            double root1 = (-b + std::sqrt(D)) / (2.0 * a);
            double root2 = (-b - std::sqrt(D)) / (2.0 * a);
            return Roots{std::make_pair(root1, root2)};
        }
        else
        {
            return std::nullopt;
        }
    }
}

void print_result(const Result& result)
{
    if (!result.has_value())
    {
        std::cout << "There are no real roots\n";
        return;
    }

    const Roots& roots = result.value();

    if (std::holds_alternative<std::monostate>(roots))
    {
        std::cout << "x = any\n";
    }
    else if (std::holds_alternative<double>(roots))
    {
        std::cout << std::get<double>(roots) << "\n";
    }
    else if (std::holds_alternative<std::pair<double, double>>(roots))
    {
        auto [r1, r2] = std::get<std::pair<double, double>>(roots);
        std::cout << r1 << " " << r2 << "\n";
    }
}

int main()
{
    double a = 0.0, b = 0.0, c = 0.0;
    std::cin >> a >> b >> c;

    Result result = solve(a, b, c);
    print_result(result);

    return 0;
}