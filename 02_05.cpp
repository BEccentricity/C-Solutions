#include <iostream>
#include <format>

int main(){
    const double eps = 1e-6;
    double e = 0, cur_e = 1;

    for (auto i = 1; cur_e > eps; ++i) {
        e += cur_e;
        cur_e /= i; 
    }

    std::cout << std::format("{:.6}", e); 

    return 0;
}