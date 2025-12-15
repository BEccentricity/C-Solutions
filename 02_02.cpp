#include <iostream>
#include <cmath>
#include <format>

using std::cout;

int main() {
    const double eps = 1e-6;
    double a = 0, b = 0, c = 0;
    
    std::cin >> a >> b >> c;
    

    if (abs(a) < eps) {

        if (abs(b) < eps) {

            if (abs(c) < eps) {

                cout << "x = any" ;

            } else {

                cout << "linear no roots";

            }
        } else {

            
            double root = -c / b;
            cout << root;
        }
    } else {

        double D = b * b - 4 * a * c;
        
        if (abs(D) < eps) {

            double root = -b / (2 * a);
            cout << root ;

        } else if (D > 0) {

            double root1 = (-b + sqrt(D)) / (2 * a);
            double root2 = (-b - sqrt(D)) / (2 * a);
            cout <<std::format ("{:.6}", root1) << " " << std::format ("{:.6}", root2);

        } else {

            cout << "There are no real roots";
        }
    }
    
    return 0;
}