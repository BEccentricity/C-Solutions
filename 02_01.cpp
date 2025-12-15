#include <iostream>
#include <cmath>

int main() {
    const double sqrt5 = std::sqrt(5);
    unsigned int n = 0;

    std::cin >> n;

    double phi1 = std::pow((1 + sqrt5)/2 , n);
    double phi2 = std::pow((1 - sqrt5)/2 , n);

    std::cout << static_cast <int> (std::round((phi1-phi2) / sqrt5));
    return 0;
}