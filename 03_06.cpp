#include <cmath>
#include <numbers>
#include <vector>
#include <cassert>

class Shape {
public:
    virtual ~Shape() = default;
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
};

class Triangle final : public Shape {
private:
    double side_a;
    double side_b;
    double side_c;

public:
    Triangle(double a, double b, double c) : side_a(a), side_b(b), side_c(c) {}

    double perimeter() const override {
        return side_a + side_b + side_c;
    }

    double area() const override {
        double semi_perimeter = perimeter() / 2.0;
        return std::sqrt(semi_perimeter * 
                        (semi_perimeter - side_a) * 
                        (semi_perimeter - side_b) * 
                        (semi_perimeter - side_c));
    }
};

class Square final : public Shape {
private:
    double side;

public:
    Square(double s) : side(s) {}

    double perimeter() const override {
        return 4.0 * side;
    }

    double area() const override {
        return side * side;
    }
};

class Circle final : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}

    double perimeter() const override {
        return 2.0 * std::numbers::pi * radius;
    }

    double area() const override {
        return std::numbers::pi * radius * radius;
    }
};

int main() {
    std::vector<Shape*> shapes;

    Triangle triangle_1(3.0, 4.0, 5.0);
    shapes.push_back(&triangle_1);
    assert(triangle_1.perimeter() == 12.0);
    assert(triangle_1.area() == 6.0);

    Square square_1(5.0);
    shapes.push_back(&square_1);
    assert(square_1.perimeter() == 20.0);
    assert(square_1.area() == 25.0);

    Circle circle_1(2.0);
    shapes.push_back(&circle_1);
    assert(circle_1.perimeter() / (2.0 * std::numbers::pi) == 2.0);
    assert(circle_1.area() / std::numbers::pi == 4.0);

    Triangle triangle_2(5.0, 5.0, 5.0);
    shapes.push_back(&triangle_2);
    assert(triangle_2.perimeter() == 15.0);

    Square square_2(3.0);
    shapes.push_back(&square_2);
    assert(square_2.area() == 9.0);

    Circle circle_2(1.0);
    shapes.push_back(&circle_2);
    assert(circle_2.perimeter() / (2.0 * std::numbers::pi) == 1.0);

    double total_perimeter = 0.0;
    double total_area = 0.0;
    for (auto shape : shapes) {
        total_perimeter += shape->perimeter();
        total_area += shape->area();
    }

    return 0;
}