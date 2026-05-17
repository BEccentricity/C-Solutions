#include <cassert>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

const long double kRubToUsdRate = 0.011L;

std::string convert_currency(const std::string& input_rub)
{
    std::istringstream in(input_rub);
    in.imbue(std::locale("ru_RU.utf8"));

    long double rub_amount = 0.0L;
    in >> std::get_money(rub_amount);

    long double usd_amount = rub_amount * kRubToUsdRate;

    std::ostringstream out;
    out.imbue(std::locale("en_US.utf8"));
    out << std::put_money(usd_amount);

    return out.str();
}

void test()
{
    auto usd1 = convert_currency("1000 RUB");
    assert(!usd1.empty());

    auto usd2 = convert_currency("RUB 500");
    assert(!usd2.empty());

    auto usd3 = convert_currency("1 500,75 RUB");
    assert(!usd3.empty());
}

int main()
{
    test();
    std::cout << "Tests passed.\n";
    std::cout << "Demonstration:\n";
    std::string demo_input = "2000 RUB";
    std::cout << "Input: " << demo_input << '\n';
    std::cout << "Output: " << convert_currency(demo_input) << '\n';
    return 0;
}