#include <algorithm>
#include <cstddef>
#include <concepts>
#include <numeric>
#include <utility>
#include <vector>

#include <benchmark/benchmark.h>

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
void hybrid_quick_sort(std::vector<T>& vector, std::size_t left, std::size_t right, std::size_t threshold) {
    if (right - left <= threshold) {
        insertion_sort(vector, left, right);
        return;
    }
    
    auto pivot_index = hoare_partition(vector, left, right);
    
    hybrid_quick_sort(vector, left, pivot_index, threshold);
    hybrid_quick_sort(vector, pivot_index + 1, right, threshold);
}

template <Sortable T>
void sort(std::vector<T>& vector, std::size_t threshold = 16) {
    if (vector.size() <= 1) {
        return;
    }
    hybrid_quick_sort(vector, 0, vector.size(), threshold);
}

static void BM_InsertionSortOnly(benchmark::State& state) {
    const std::size_t size = state.range(0);
    
    for (auto _ : state) {
        std::vector<double> vec(size);
        std::iota(vec.rbegin(), vec.rend(), 1.0);
        
        insertion_sort(vec, 0, vec.size());
        
        benchmark::DoNotOptimize(vec);
    }
}

static void BM_HybridSortThreshold(benchmark::State& state) {
    const std::size_t size = 10000;
    const std::size_t threshold = state.range(0);
    
    for (auto _ : state) {
        std::vector<double> vec(size);
        std::iota(vec.rbegin(), vec.rend(), 1.0);
        
        sort(vec, threshold);
        
        benchmark::DoNotOptimize(vec);
    }
}

static void BM_HybridSortThreshold_Args(benchmark::internal::Benchmark* b) {
    for (int threshold = 2; threshold <= 64; threshold *= 2) {
        b->Args({threshold});
    }
}

static void BM_QuickSortOnly(benchmark::State& state) {
    const std::size_t size = state.range(0);
    const std::size_t threshold = 0;
    
    for (auto _ : state) {
        std::vector<double> vec(size);
        std::iota(vec.rbegin(), vec.rend(), 1.0);
        
        sort(vec, threshold);
        
        benchmark::DoNotOptimize(vec);
    }
}

static void BM_DifferentSizes(benchmark::State& state) {
    const std::size_t size = state.range(0);
    const std::size_t threshold = 16;
    
    for (auto _ : state) {
        std::vector<double> vec(size);
        std::iota(vec.rbegin(), vec.rend(), 1.0);
        
        sort(vec, threshold);
        
        benchmark::DoNotOptimize(vec);
    }
}

static void BM_CompareAlgorithms(benchmark::State& state) {
    const std::size_t size = state.range(0);
    const int algorithm = state.range(1);
    
    for (auto _ : state) {
        std::vector<double> vec(size);
        std::iota(vec.rbegin(), vec.rend(), 1.0);
        
        if (algorithm == 0) {
            insertion_sort(vec, 0, vec.size());
        } else if (algorithm == 1) {
            std::vector<double> temp = vec;
            std::sort(temp.begin(), temp.end());
        } else {
            sort(vec, 16);
        }
        
        benchmark::DoNotOptimize(vec);
    }
}

static void BM_ThresholdImpactDetailed(benchmark::State& state) {
    const std::size_t threshold = state.range(0);
    const std::size_t size = 10000;
    
    for (auto _ : state) {
        std::vector<double> vec(size);
        std::iota(vec.rbegin(), vec.rend(), 1.0);
        
        sort(vec, threshold);
        
        benchmark::DoNotOptimize(vec);
    }
}

BENCHMARK(BM_InsertionSortOnly)
    ->RangeMultiplier(10)
    ->Range(10, 1000)
    ->Unit(benchmark::kMicrosecond);

BENCHMARK(BM_QuickSortOnly)
    ->RangeMultiplier(10)
    ->Range(100, 10000)
    ->Unit(benchmark::kMicrosecond);

BENCHMARK(BM_HybridSortThreshold)
    ->Apply(BM_HybridSortThreshold_Args)
    ->Unit(benchmark::kMicrosecond);

BENCHMARK(BM_DifferentSizes)
    ->RangeMultiplier(2)
    ->Range(128, 131072)
    ->Unit(benchmark::kMillisecond);

BENCHMARK(BM_CompareAlgorithms)
    ->Args({100, 0})
    ->Args({100, 1})
    ->Args({100, 2})
    ->Args({1000, 0})
    ->Args({1000, 1})
    ->Args({1000, 2})
    ->Args({10000, 0})
    ->Args({10000, 1})
    ->Args({10000, 2})
    ->Unit(benchmark::kMicrosecond);

BENCHMARK(BM_ThresholdImpactDetailed)
    ->DenseRange(2, 32, 2)
    ->Unit(benchmark::kMicrosecond);

static void BM_MemoryAccess(benchmark::State& state) {
    const std::size_t size = 10000;
    
    for (auto _ : state) {
        std::vector<double> vec(size);
        std::iota(vec.rbegin(), vec.rend(), 1.0);
        
        state.PauseTiming();
        std::vector<double> copy = vec;
        state.ResumeTiming();
        
        sort(copy, 16);
        
        benchmark::DoNotOptimize(copy);
    }
}

BENCHMARK(BM_MemoryAccess)->Unit(benchmark::kMicrosecond);

static void BM_WorstCaseScenario(benchmark::State& state) {
    const std::size_t threshold = state.range(0);
    const std::size_t size = 10000;
    
    std::vector<double> vec(size);
    std::iota(vec.rbegin(), vec.rend(), 1.0);
    
    for (auto _ : state) {
        std::vector<double> test_vec = vec;
        sort(test_vec, threshold);
        benchmark::DoNotOptimize(test_vec);
    }
}

BENCHMARK(BM_WorstCaseScenario)
    ->Arg(0)
    ->Arg(4)
    ->Arg(8)
    ->Arg(16)
    ->Arg(32)
    ->Arg(64)
    ->Unit(benchmark::kMicrosecond);

static void BM_BestCaseScenario(benchmark::State& state) {
    const std::size_t threshold = state.range(0);
    const std::size_t size = 10000;
    
    std::vector<double> vec(size);
    std::iota(vec.begin(), vec.end(), 1.0);
    
    for (auto _ : state) {
        std::vector<double> test_vec = vec;
        sort(test_vec, threshold);
        benchmark::DoNotOptimize(test_vec);
    }
}

BENCHMARK(BM_BestCaseScenario)
    ->Arg(0)
    ->Arg(4)
    ->Arg(8)
    ->Arg(16)
    ->Arg(32)
    ->Arg(64)
    ->Unit(benchmark::kMicrosecond);

int main(int argc, char** argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}