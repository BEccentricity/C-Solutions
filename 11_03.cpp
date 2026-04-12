#include <algorithm>
#include <cassert>
#include <cstddef>
#include <concepts>
#include <functional>
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

template <typename T, typename Compare>
concept SortableWith = requires(T a, T b, Compare comp) {
    { comp(a, b) } -> std::convertible_to<bool>;
};

template <std::random_access_iterator Iterator, typename Compare>
requires SortableWith<typename std::iterator_traits<Iterator>::value_type, Compare>
void insertion_sort(Iterator left, Iterator right, Compare comp) {
    if (left == right) return;
    
    for (auto i = std::next(left); i != right; ++i) {
        auto key = *i;
        auto j = i;
        while (j != left && comp(key, *std::prev(j))) {
            *j = *std::prev(j);
            --j;
        }
        *j = key;
    }
}

template <std::random_access_iterator Iterator, typename Compare>
requires SortableWith<typename std::iterator_traits<Iterator>::value_type, Compare>
typename std::iterator_traits<Iterator>::value_type median_of_three(Iterator left, Iterator right, Compare comp) {
    auto distance = std::distance(left, right);
    auto middle = std::next(left, distance / 2);
    auto last = std::prev(right);
    
    if (comp(*middle, *left)) {
        std::iter_swap(left, middle);
    }
    if (comp(*last, *middle)) {
        std::iter_swap(middle, last);
    }
    if (comp(*middle, *left)) {
        std::iter_swap(left, middle);
    }
    
    return *middle;
}

template <std::random_access_iterator Iterator, typename Compare>
requires SortableWith<typename std::iterator_traits<Iterator>::value_type, Compare>
Iterator hoare_partition(Iterator left, Iterator right, Compare comp) {
    auto pivot = median_of_three(left, right, comp);
    auto distance = std::distance(left, right);
    auto middle = std::next(left, distance / 2);
    auto last = std::prev(right);
    std::iter_swap(middle, last);
    
    auto i = left;
    auto j = std::prev(last);
    
    while (std::distance(left, i) <= std::distance(left, j)) {
        while (comp(*i, pivot)) {
            ++i;
        }
        while (j != left && comp(pivot, *j)) {
            --j;
        }
        if (std::distance(left, i) <= std::distance(left, j)) {
            std::iter_swap(i, j);
            ++i;
            if (j != left) {
                --j;
            }
        }
    }
    
    std::iter_swap(i, last);
    return i;
}

template <std::random_access_iterator Iterator, typename Compare>
requires SortableWith<typename std::iterator_traits<Iterator>::value_type, Compare>
void hybrid_quick_sort(Iterator left, Iterator right, Compare comp) {
    auto distance = std::distance(left, right);
    if (distance <= 16) {
        insertion_sort(left, right, comp);
        return;
    }
    
    auto pivot_iterator = hoare_partition(left, right, comp);
    
    hybrid_quick_sort(left, pivot_iterator, comp);
    hybrid_quick_sort(std::next(pivot_iterator), right, comp);
}

template <std::random_access_iterator Iterator, typename Compare = std::less<>>
requires SortableWith<typename std::iterator_traits<Iterator>::value_type, Compare>
void my_sort(Iterator begin, Iterator end, Compare comp = Compare{}) {
    auto distance = std::distance(begin, end);
    if (distance <= 1) {
        return;
    }
    hybrid_quick_sort(begin, end, comp);
}

bool custom_compare(int a, int b) {
    return a > b;
}

int main() {
    auto size = 1000uz;
    std::vector<int> data(size, 0);
        
    for (auto i = 0uz; i < size; ++i) {
        data[i] = static_cast<int>(size - i);
    }
        
    my_sort(data.begin(), data.end());
    assert(std::ranges::is_sorted(data));
    
    for (auto i = 0uz; i < size; ++i) {
        data[i] = static_cast<int>(size - i);
    }
    my_sort(data.begin(), data.end(), std::less<int>());
    assert(std::ranges::is_sorted(data));
    
    for (auto i = 0uz; i < size; ++i) {
        data[i] = static_cast<int>(size - i);
    }
    my_sort(data.begin(), data.end(), [](int a, int b) { return a < b; });
    assert(std::ranges::is_sorted(data));
    
    std::vector<int> data_desc = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    my_sort(data_desc.begin(), data_desc.end(), custom_compare);
    assert(std::is_sorted(data_desc.begin(), data_desc.end(), std::greater<int>()));
    
    std::vector<int> data_1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    my_sort(data_1.begin(), data_1.end());
    assert(std::ranges::is_sorted(data_1));
    
    std::vector<int> data_2 = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    my_sort(data_2.begin(), data_2.end());
    assert(std::ranges::is_sorted(data_2));
    
    std::vector<int> data_3;
    my_sort(data_3.begin(), data_3.end());
    assert(data_3.empty());
    
    std::vector<int> data_4 = {42};
    my_sort(data_4.begin(), data_4.end());
    assert(data_4.size() == 1 && data_4[0] == 42);
    
    std::vector<int> data_5 = {5, 5, 5, 5, 5, 5};
    my_sort(data_5.begin(), data_5.end());
    assert(std::ranges::is_sorted(data_5));

    auto size_6 = 10000uz;
    std::vector<int> data_6(size_6);
        
    for (auto i = 0uz; i < size_6; ++i) {
        data_6[i] = static_cast<int>(size_6 - i);
    }
        
    my_sort(data_6.begin(), data_6.end());
    assert(std::ranges::is_sorted(data_6));
    
    std::vector<double> data_7 = {3.14, 2.71, 1.41, 1.62, 0.58};
    my_sort(data_7.begin(), data_7.end());
    assert(std::ranges::is_sorted(data_7));
    
    std::vector<char> data_8 = {'z', 'a', 'm', 'k', 'b'};
    my_sort(data_8.begin(), data_8.end());
    assert(std::ranges::is_sorted(data_8));
    
    std::vector<int> data_9 = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    my_sort(data_9.begin(), data_9.end(), [](int a, int b) { return a > b; });
    assert(std::is_sorted(data_9.begin(), data_9.end(), std::greater<int>()));
    
    return 0;
}