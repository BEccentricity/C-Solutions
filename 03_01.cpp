#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <vector>

struct Rectangle {
    double left;   
    double top;     
    double right;  
    double bottom; 

    constexpr Rectangle() : left(0.0), top(0.0), right(0.0), bottom(0.0) {}

    constexpr Rectangle(double l, double t, double r, double b) 
        : left(l), top(t), right(r), bottom(b) {}
    
    constexpr double width() const {
        return right - left;
    }
    
    constexpr double height() const {
        return bottom - top;
    }
    
    constexpr double area() const {
        return width() * height();
    }
    
    bool is_degenerate(double epsilon = 1e-9) const {
        return std::fabs(width()) <= epsilon || std::fabs(height()) <= epsilon;
    }
    

    bool is_empty(double epsilon = 1e-9) const {
        return width() < -epsilon || height() < -epsilon;
    }
};

Rectangle intersection(const Rectangle& a, const Rectangle& b, double epsilon = 1e-9) {
    const double inter_left = std::max(a.left, b.left);
    const double inter_top = std::max(a.top, b.top);
    const double inter_right = std::min(a.right, b.right);
    const double inter_bottom = std::min(a.bottom, b.bottom);
    
    Rectangle result(inter_left, inter_top, inter_right, inter_bottom);
    
    if (result.width() < -epsilon) {
        result.right = result.left; 
    }
    if (result.height() < -epsilon) {
        result.bottom = result.top;
    }
    
    return result;
}

double intersection_area(const std::vector<Rectangle>& rectangles, double epsilon = 1e-9) {
    if (rectangles.empty()) {
        return 0.0;
    }
    
    Rectangle current_intersection = rectangles[0];
    
    for (std::size_t i = 1; i < rectangles.size(); ++i) {
        current_intersection = intersection(current_intersection, rectangles[i], epsilon);
        
        if (current_intersection.is_empty(epsilon)) {
            return 0.0;
        }
    }
    
    const double area = current_intersection.area();
    return area > epsilon ? area : 0.0;
}

Rectangle bounding_box(const std::vector<Rectangle>& rectangles) {
    if (rectangles.empty()) {
        return Rectangle(0.0, 0.0, 0.0, 0.0);
    }
    
    double min_left = std::numeric_limits<double>::max();
    double min_top = std::numeric_limits<double>::max();
    double max_right = std::numeric_limits<double>::lowest();
    double max_bottom = std::numeric_limits<double>::lowest();
    
    for (const auto& rect : rectangles) {
        min_left = std::min(min_left, rect.left);
        min_top = std::min(min_top, rect.top);
        max_right = std::max(max_right, rect.right);
        max_bottom = std::max(max_bottom, rect.bottom);
    }
    
    return Rectangle(min_left, min_top, max_right, max_bottom);
}

int main() {
    const double epsilon = 1e-9;
    
    {
        Rectangle rect1(0.0, 0.0, 10.0, 10.0);
        Rectangle rect2(5.0, 5.0, 15.0, 15.0);
        Rectangle rect3(2.0, 2.0, 8.0, 8.0);
        
        std::vector<Rectangle> rectangles = {rect1, rect2, rect3};
        
        assert(std::fabs(intersection_area(rectangles) - 9.0) < epsilon);
        
        Rectangle bbox = bounding_box(rectangles);
        assert(std::fabs(bbox.left - 0.0) < epsilon);
        assert(std::fabs(bbox.top - 0.0) < epsilon);
        assert(std::fabs(bbox.right - 15.0) < epsilon);
        assert(std::fabs(bbox.bottom - 15.0) < epsilon);
    }
    
    {
        Rectangle rect1(0.0, 0.0, 5.0, 5.0);
        Rectangle rect2(10.0, 10.0, 15.0, 15.0);
        
        std::vector<Rectangle> rectangles = {rect1, rect2};
        
        assert(std::fabs(intersection_area(rectangles) - 0.0) < epsilon);
        
        Rectangle bbox = bounding_box(rectangles);
        assert(std::fabs(bbox.left - 0.0) < epsilon);
        assert(std::fabs(bbox.top - 0.0) < epsilon);
        assert(std::fabs(bbox.right - 15.0) < epsilon);
        assert(std::fabs(bbox.bottom - 15.0) < epsilon);
    }
    
    {
        Rectangle rect1(0.0, 0.0, 5.0, 5.0);
        Rectangle rect2(5.0, 0.0, 10.0, 5.0);  
        
        std::vector<Rectangle> rectangles = {rect1, rect2};
        
        Rectangle inter = intersection(rect1, rect2);
        assert(inter.is_degenerate(epsilon));
        assert(std::fabs(inter.area()) < epsilon);
        assert(std::fabs(intersection_area(rectangles)) < epsilon);
    }
    
    {
        Rectangle rect(1.5, 1.5, 4.5, 4.5);
        std::vector<Rectangle> rectangles = {rect};
        
        assert(std::fabs(intersection_area(rectangles) - 9.0) < epsilon);
        
        Rectangle bbox = bounding_box(rectangles);
        assert(std::fabs(bbox.left - 1.5) < epsilon);
        assert(std::fabs(bbox.top - 1.5) < epsilon);
        assert(std::fabs(bbox.right - 4.5) < epsilon);
        assert(std::fabs(bbox.bottom - 4.5) < epsilon);
    }
    
    {
        std::vector<Rectangle> empty_vec;
        
        assert(std::fabs(intersection_area(empty_vec) - 0.0) < epsilon);
        
        Rectangle bbox = bounding_box(empty_vec);
        assert(bbox.is_degenerate(epsilon));
        assert(std::fabs(bbox.area()) < epsilon);
    }
    
    {
        Rectangle rect1(0.5, 0.5, 3.5, 3.5);
        Rectangle rect2(1.5, 1.5, 2.5, 2.5);
        
        std::vector<Rectangle> rectangles = {rect1, rect2};
        
        assert(std::fabs(intersection_area(rectangles) - 1.0) < epsilon);
    }
    
    {
        Rectangle rect1(-2.5, -2.5, 2.5, 2.5);
        Rectangle rect2(-1.5, -1.5, 1.5, 1.5);
        
        std::vector<Rectangle> rectangles = {rect1, rect2};
        
        assert(std::fabs(intersection_area(rectangles) - 9.0) < epsilon);
    }

    return 0;
}