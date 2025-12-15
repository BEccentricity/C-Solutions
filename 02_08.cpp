#include <iostream>
#include <vector>
#include <algorithm>


unsigned long long nextCollatz(unsigned long long n) {
    if (n % 2 == 0) {
        return n / 2;
    } else {
        return 3 * n + 1;
    }
}

int main() {
    const size_t max = 100;
    std::vector <size_t> cache(max * 100, 0); 
    
    size_t maxLength = 0;
    unsigned long long start = 1;
    
    for (unsigned long long i = 1; i <= max; ++i) {
        unsigned long long n = i;
        size_t length = 0;
        
        while (n != 1) {
            if (n < cache.size() && cache[n] != 0) {
                length += cache[n];
                break;
            }
            
            n = nextCollatz(n);
            length++;
        }
        
        if (i < cache.size()) {
            cache[i] = length;
        }
        
        if (length > maxLength) {
            maxLength = length;
            start = i;
        }
    }
    
    std::cout << "value: " << start<< "\n";
    std::cout << "length: " << maxLength + 1;
    cache.clear();
    return 0;
}