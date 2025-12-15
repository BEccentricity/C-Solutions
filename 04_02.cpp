#include <algorithm>
#include <cassert>
#include <concepts>
#include <limits>

template <typename... Args>
concept AllDoubles = (std::same_as<Args, double> && ...);

template <AllDoubles... Args>
double max(Args... args);

template <AllDoubles... Args>
double min(Args... args);

template <AllDoubles... Args>
double sum(Args... args) {
    return (args + ...);
}

template <AllDoubles... Args>
double average(Args... args) {
    static_assert(sizeof...(args) > 0, "Argument pack must be non-empty");
    return (args + ...) / sizeof...(args);
}

template <AllDoubles... Args>
double max_recursive_impl(double current_max, double first, Args... rest) {
    double new_max = std::max(current_max, first);
    if constexpr (sizeof...(rest) == 0) {
        return new_max;
    } else {
        return max_recursive_impl(new_max, rest...);
    }
}

template <AllDoubles... Args>
double min_recursive_impl(double current_min, double first, Args... rest) {
    double new_min = std::min(current_min, first);
    if constexpr (sizeof...(rest) == 0) {
        return new_min;
    } else {
        return min_recursive_impl(new_min, rest...);
    }
}


template <AllDoubles... Args>
double max(Args... args) {
    if constexpr (sizeof...(args) == 0) {
        return std::numeric_limits<double>::lowest();
    } else {
        return max_recursive_impl(std::numeric_limits<double>::lowest(), args...);
    }
}

template <AllDoubles... Args>
double min(Args... args) {
    if constexpr (sizeof...(args) == 0) {
        return std::numeric_limits<double>::max();
    } else {
        return min_recursive_impl(std::numeric_limits<double>::max(), args...);
    }
}

int main() {
    assert(max(1.0, 2.0, 3.0) == 3.0);
    assert(max(3.0, 2.0, 1.0) == 3.0);
    assert(max(-1.0, -2.0, -3.0) == -1.0);
    assert(max(5.0) == 5.0);
    assert(max(1.5, 2.5, 3.5, 4.5, 5.5) == 5.5);
    
    assert(min(1.0, 2.0, 3.0) == 1.0);
    assert(min(3.0, 2.0, 1.0) == 1.0);
    assert(min(-1.0, -2.0, -3.0) == -3.0);
    assert(min(5.0) == 5.0);
    assert(min(1.5, 2.5, 3.5, 4.5, 5.5) == 1.5);
    
    assert(sum(1.0, 2.0, 3.0) == 6.0);
    assert(sum(1.0) == 1.0);
    assert(sum(1.5, 2.5, 3.5) == 7.5);
    assert(sum(-1.0, 1.0, -2.0, 2.0) == 0.0);
    assert(sum(0.1, 0.2, 0.3) == 0.6);
    
    assert(average(1.0, 2.0, 3.0) == 2.0);
    assert(average(5.0) == 5.0);
    assert(average(2.0, 4.0, 6.0, 8.0) == 5.0);
    assert(average(-1.0, 0.0, 1.0) == 0.0);
    assert(average(1.0, 2.0, 3.0, 4.0, 5.0) == 3.0);
    
    return 0;
}