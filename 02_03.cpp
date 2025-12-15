#include <iostream>

using std::cout;

int main() {
    char c;
    std::cin >> c;
    switch (c) {
        case 'A' ... 'Z':
            cout << "uppercase "<< c;
            break;
        case 'a' ... 'z':
            cout << "lowercase "<< c;
            break;
        case '0' ... '9':
            cout << "digit "<< c;
            break;
        case '!': 
        case '.': 
        case ',':
        case ';':
        case ':':
        case '-':
        case '?':
            cout << "punct "<< c;
            break;
                

        default:
            cout << "other "<< c;
            break;
    }

    return 0;
}