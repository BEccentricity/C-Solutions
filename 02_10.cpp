#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

void insertion_sort(std::vector<int>& vector, std::size_t left, std::size_t right) {
    for (auto i = left + 1; i < right; ++i) {
        auto key = vector[i];
        auto j = i;
        while (j > left && vector[j - 1] > key) {
            vector[j] = vector[j - 1];
            --j;
        }
        vector[j] = key;
    }
}

int median_of_three(std::vector<int>& vector, std::size_t left, std::size_t right) {
    auto middle = left + (right - left - 1) / 2;
    auto last = right - 1;
    
    if (vector[left] > vector[middle]) {
        std::swap(vector[left], vector[middle]);
    }
    if (vector[middle] > vector[last]) {
        std::swap(vector[middle], vector[last]);
    }
    if (vector[left] > vector[middle]) {
        std::swap(vector[left], vector[middle]);
    }
    
    return vector[middle];
}

std::size_t hoare_partition(std::vector<int>& vector, std::size_t left, std::size_t right) {
    auto pivot = median_of_three(vector, left, right);
    auto middle = left + (right - left - 1) / 2;
    std::swap(vector[middle], vector[right - 1]);
    
    auto i = left;
    auto j = right - 2;
    
    while (i <= j) {
        while (vector[i] < pivot) {
            ++i;
        }
        while (j >= left && vector[j] > pivot) {
            --j;
        }
        if (i <= j) {
            std::swap(vector[i], vector[j]);
            ++i;
            --j;
        }
    }
    
    std::swap(vector[i], vector[right - 1]);
    return i;
}

void hybrid_quick_sort(std::vector<int>& vector, std::size_t left, std::size_t right) {
    if (right - left <= 16) {
        insertion_sort(vector, left, right);
        return;
    }
    
    auto pivot_index = hoare_partition(vector, left, right);
    
    hybrid_quick_sort(vector, left, pivot_index);
    hybrid_quick_sort(vector, pivot_index + 1, right);
}

void sort(std::vector<int>& vector) {
    if (vector.size() <= 1) {
        return;
    }
    hybrid_quick_sort(vector, 0, vector.size());
}

int main() {
    auto size = 1000uz;
    std::vector<int> data(size, 0);
        
    for (auto i = 0uz; i < size; ++i) {
        data[i] = static_cast<int>(size - i);
    }
        
    sort(data);
    assert(std::ranges::is_sorted(data));
    
    std::vector<int> data_1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    sort(data_1);
    assert(std::ranges::is_sorted(data_1));
    
    std::vector<int> data_2 = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    sort(data_2);
    assert(std::ranges::is_sorted(data_2));
    
    std::vector<int> data_3;
    sort(data_3);
    assert(data_3.empty());
    
    std::vector<int> data_4 = {42};
    sort(data_4);
    assert(data_4.size() == 1 && data_4[0] == 42);
    
    std::vector<int> data_5 = {5, 5, 5, 5, 5, 5};
    sort(data_5);
    assert(std::ranges::is_sorted(data_5));

    auto size_6 = 10000uz;
    std::vector<int> data_6(size_6);
        
    for (auto i = 0uz; i < size_6; ++i) {
        data_6[i] = static_cast<int>(size_6 - i);
    }
        
    sort(data_6);
    assert(std::ranges::is_sorted(data_6));
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////

/*
Временная сложность:
 O(n log n) в лучшем случае в худшем O(n^2)

Оптимизация:
Медиана трех: предотвращает вырожденные случаи для отсортированных массивов
Сортировка вставками для n ≤ 16: уменьшает накладные расходы рекурсии
*/