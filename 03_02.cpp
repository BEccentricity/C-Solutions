#include <cmath>
#include <numbers>
#include <cassert>

class Triangle {
private:
    double side_a;
    double side_b;
    double side_c;

public:
    Triangle(double a, double b, double c) : side_a(a), side_b(b), side_c(c) {}

    double perimeter() const {
        return side_a + side_b + side_c;
    }

    double area() const {
        double semi_perimeter = perimeter() / 2.0;
        return std::sqrt(semi_perimeter * 
                        (semi_perimeter - side_a) * 
                        (semi_perimeter - side_b) * 
                        (semi_perimeter - side_c));
    }
};

class Square {
private:
    double side;

public:
    Square(double s) : side(s) {}

    double perimeter() const {
        return 4.0 * side;
    }

    double area() const {
        return side * side;
    }
};

class Circle {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}

    double perimeter() const {
        return 2.0 * std::numbers::pi * radius;
    }

    double area() const {
        return std::numbers::pi * radius * radius;
    }
};

int main() {
    Triangle triangle(3.0, 4.0, 5.0);
    assert(triangle.perimeter() == 12.0);
    assert(triangle.area() == 6.0);

    Square square(5.0);
    assert(square.perimeter() == 20.0);
    assert(square.area() == 25.0);

    Circle circle(2.0);
    assert(circle.perimeter() / (2.0 * std::numbers::pi) == 2.0);
    assert(circle.area() / std::numbers::pi == 4.0);

    return 0;
}