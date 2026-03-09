#include <algorithm>
#include <cstddef>
#include <concepts>
#include <utility>
#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

template <typename T>
concept Sortable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
    { a > b } -> std::convertible_to<bool>;
};

template <Sortable T>
void insertion_sort(std::vector<T>& vector, std::size_t left, std::size_t right) {
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

template <Sortable T>
T median_of_three(std::vector<T>& vector, std::size_t left, std::size_t right) {
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

template <Sortable T>
std::size_t hoare_partition(std::vector<T>& vector, std::size_t left, std::size_t right) {
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

template <Sortable T>
void hybrid_quick_sort(std::vector<T>& vector, std::size_t left, std::size_t right) {
    if (right - left <= 16) {
        insertion_sort(vector, left, right);
        return;
    }
    
    auto pivot_index = hoare_partition(vector, left, right);
    
    hybrid_quick_sort(vector, left, pivot_index);
    hybrid_quick_sort(vector, pivot_index + 1, right);
}

template <Sortable T>
void sort(std::vector<T>& vector) {
    if (vector.size() <= 1) {
        return;
    }
    hybrid_quick_sort(vector, 0, vector.size());
}

class SortTest : public ::testing::Test {
protected:
    void SetUp() override {
    }
    
    void TearDown() override {
    }
};

TEST_F(SortTest, EmptyVector) {
    std::vector<int> vec;
    sort(vec);
    EXPECT_TRUE(vec.empty());
}

TEST_F(SortTest, SingleElement) {
    std::vector<int> vec = {42};
    sort(vec);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], 42);
}

TEST_F(SortTest, AlreadySorted) {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    sort(vec);
    EXPECT_TRUE(std::is_sorted(vec.begin(), vec.end()));
}

TEST_F(SortTest, ReverseSorted) {
    std::vector<int> vec = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    sort(vec);
    EXPECT_TRUE(std::is_sorted(vec.begin(), vec.end()));
}

TEST_F(SortTest, AllEqual) {
    std::vector<int> vec = {5, 5, 5, 5, 5, 5};
    sort(vec);
    EXPECT_TRUE(std::is_sorted(vec.begin(), vec.end()));
}

TEST_F(SortTest, RandomOrder) {
    std::vector<int> vec = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    sort(vec);
    EXPECT_TRUE(std::is_sorted(vec.begin(), vec.end()));
}

TEST_F(SortTest, DoubleType) {
    std::vector<double> vec = {3.14, 2.71, 1.41, 1.62, 0.58};
    sort(vec);
    EXPECT_TRUE(std::is_sorted(vec.begin(), vec.end()));
}

TEST_F(SortTest, CharType) {
    std::vector<char> vec = {'z', 'a', 'm', 'k', 'b'};
    sort(vec);
    EXPECT_TRUE(std::is_sorted(vec.begin(), vec.end()));
}

TEST_F(SortTest, LargeVector) {
    const std::size_t size = 10000;
    std::vector<int> vec(size);
    for (std::size_t i = 0; i < size; ++i) {
        vec[i] = static_cast<int>(size - i);
    }
    sort(vec);
    EXPECT_TRUE(std::is_sorted(vec.begin(), vec.end()));
}

class SortParameterizedTest : public SortTest, public ::testing::WithParamInterface<std::size_t> {
};

TEST_P(SortParameterizedTest, DifferentSizes) {
    std::size_t size = GetParam();
    std::vector<int> vec(size);
    for (std::size_t i = 0; i < size; ++i) {
        vec[i] = static_cast<int>(size - i);
    }
    sort(vec);
    EXPECT_TRUE(std::is_sorted(vec.begin(), vec.end()));
}

INSTANTIATE_TEST_SUITE_P(
    SortTests,
    SortParameterizedTest,
    ::testing::Values(0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 100, 1000)
);

TEST(InsertionSortTest, SmallVector) {
    std::vector<int> vec = {5, 2, 4, 6, 1, 3};
    insertion_sort(vec, 0, vec.size());
    EXPECT_TRUE(std::is_sorted(vec.begin(), vec.end()));
}

TEST(MedianOfThreeTest, CorrectMedian) {
    std::vector<int> vec = {3, 1, 2};
    int median = median_of_three(vec, 0, vec.size());
    EXPECT_EQ(median, 2);
}

TEST(PartitionTest, PartitionIndex) {
    std::vector<int> vec = {3, 7, 8, 5, 2, 1, 9, 5, 4};
    std::size_t pivot_index = hoare_partition(vec, 0, vec.size());
    
    for (std::size_t i = 0; i < pivot_index; ++i) {
        EXPECT_LE(vec[i], vec[pivot_index]);
    }
    for (std::size_t i = pivot_index + 1; i < vec.size(); ++i) {
        EXPECT_GE(vec[i], vec[pivot_index]);
    }
}

TEST(EdgeCaseTest, ThresholdBoundary) {
    std::vector<int> vec(17);
    for (std::size_t i = 0; i < 17; ++i) {
        vec[i] = static_cast<int>(17 - i);
    }
    sort(vec);
    EXPECT_TRUE(std::is_sorted(vec.begin(), vec.end()));
}

TEST(StabilityTest, NotApplicable) {
    std::vector<std::pair<int, int>> vec = {{3, 1}, {2, 2}, {1, 3}, {2, 1}, {1, 2}};
    std::vector<int> simple_vec;
    for (const auto& p : vec) {
        simple_vec.push_back(p.first);
    }
    sort(simple_vec);
    EXPECT_TRUE(std::is_sorted(simple_vec.begin(), simple_vec.end()));
}

TEST(PerformanceTest, LargeRandomVector) {
    const std::size_t size = 10000;
    std::vector<int> vec(size);
    for (std::size_t i = 0; i < size; ++i) {
        vec[i] = rand() % 10000;
    }
    sort(vec);
    EXPECT_TRUE(std::is_sorted(vec.begin(), vec.end()));
}

TEST(InsertionSortThresholdTest, BelowThreshold) {
    std::vector<int> vec(15);
    for (std::size_t i = 0; i < 15; ++i) {
        vec[i] = static_cast<int>(15 - i);
    }
    sort(vec);
    EXPECT_TRUE(std::is_sorted(vec.begin(), vec.end()));
}

TEST(InsertionSortThresholdTest, AtThreshold) {
    std::vector<int> vec(16);
    for (std::size_t i = 0; i < 16; ++i) {
        vec[i] = static_cast<int>(16 - i);
    }
    sort(vec);
    EXPECT_TRUE(std::is_sorted(vec.begin(), vec.end()));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}