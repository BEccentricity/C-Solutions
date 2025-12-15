#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>

int main() {
    std::vector <double> data; 
    int n = 0;
    double num = 0;

    std::cin >> n;
    
    for(auto i = 0; i < n; ++i) {
        std::cin >> num;
        data.push_back(num);
    }
    
    assert(data.empty());
    double min = data[0];
    double max = data[0];
    double sum = 0;
    
    for (int i = 0; i < n; i++) {
        if (data[i] < min) min = data[i];
        if (data[i] > max) max = data[i];

        sum += data[i];
    }
    
    double avg = sum / n;
    double D = 0;

    for (int i = 0; i < n; i++) {
        D += (data[i] - avg) * (data[i] - avg);
    }

    D /= n;
    double std_dev = sqrt(D);
    
    std::cout << "Average: " << avg <<"\n";
    std::cout << "Sigma: " << std_dev ;
    data.clear();
    return 0;
}