#include <cassert>
#include <cmath>
#include <optional>
#include <print>
#include <utility>
#include <variant>

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

class RootPrinter
{
public:
    void operator()(std::monostate) const
    {
        std::print("x = any\n");
    }

    void operator()(double root) const
    {
        std::print("{}\n", root);
    }

    void operator()(const std::pair<double, double>& roots) const
    {
        auto [r1, r2] = roots;
        std::print("{} {}\n", r1, r2);
    }
};

void print_result(const Result& result)
{
    if (!result.has_value())
    {
        std::print("There are no real roots\n");
        return;
    }

    std::visit(RootPrinter{}, result.value());
}

void test()
{
    const double epsilon = 1e-6;

    {
        auto res = solve(0.0, 0.0, 0.0);
        assert(res.has_value());
        assert(std::holds_alternative<std::monostate>(*res));
    }

    {
        auto res = solve(0.0, 0.0, 1.0);
        assert(!res.has_value());
    }

    {
        auto res = solve(0.0, 2.0, 4.0);
        assert(res.has_value());
        assert(std::holds_alternative<double>(*res));
        assert(std::abs(std::get<double>(*res) - (-2.0)) < epsilon);
    }

    {
        auto res = solve(1.0, -2.0, 1.0);
        assert(res.has_value());
        assert(std::holds_alternative<double>(*res));
        assert(std::abs(std::get<double>(*res) - 1.0) < epsilon);
    }

    {
        auto res = solve(1.0, -3.0, 2.0);
        assert(res.has_value());
        bool is_pair = std::holds_alternative<std::pair<double, double>>(*res);
        assert(is_pair);
        auto [r1, r2] = std::get<std::pair<double, double>>(*res);
        assert(std::abs(r1 - 2.0) < epsilon);
        assert(std::abs(r2 - 1.0) < epsilon);
    }

    {
        auto res = solve(1.0, 1.0, 1.0);
        assert(!res.has_value());
    }

    {
        auto res = solve(1.0, 0.0, -4.0);
        assert(res.has_value());
        bool is_pair = std::holds_alternative<std::pair<double, double>>(*res);
        assert(is_pair);
        auto [r1, r2] = std::get<std::pair<double, double>>(*res);
        assert(std::abs(r1 - 2.0) < epsilon && std::abs(r2 + 2.0) < epsilon);
    }
}

int main()
{
    test();
    std::print("All tests passed.\n");

    auto example = solve(1.0, -3.0, 2.0);
    std::print("Demonstration: ");
    print_result(example);
}