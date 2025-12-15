#include <iostream>
#include <cmath>

const int MAX_SIZE = 1000;

int main() {
    double data[MAX_SIZE];
    int cnt = 0;

    double num;
    while (std::cin >> num && cnt < MAX_SIZE) {
        data[cnt++] = num;
    }
    
    if (cnt == 0) {
        std::cout << "aahahah";
        return 0;
    }
    
    double min = data[0];
    double max = data[0];
    double sum = 0;
    
    for (int i = 0; i < cnt; i++) {
        if (data[i] < min) min = data[i];
        if (data[i] > max) max = data[i];
        sum += data[i];
    }
    
    double avg = sum / cnt;

    double D = 0;
    for (int i = 0; i < cnt; i++) {
        D += (data[i] - avg) * (data[i] - avg);
    }
    D /= cnt;
    double std_dev = sqrt(D);
    
    std::cout << "Average: " << avg <<"\n";
    std::cout << "Sigma: " << std_dev ;
    
    return 0;
}