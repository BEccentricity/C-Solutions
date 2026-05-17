#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <ranges>
#include <vector>

template <std::ranges::input_range R, typename Pred, typename Func>
auto transform_if(R&& range, Pred pred, Func func)
{
    std::vector<std::ranges::range_value_t<R>> result;

    auto filtered = range | std::views::filter(pred);

    std::ranges::transform(filtered, std::back_inserter(result), func);

    return result;
}

double mae(const std::vector<double>& actual, const std::vector<double>& predicted)
{
    auto zipped = std::views::zip(actual, predicted);

    auto absolute_errors = zipped | std::views::transform([](const auto& pair) {
        auto [a, p] = pair;
        return std::abs(a - p);
    });

    double sum = std::accumulate(std::ranges::begin(absolute_errors),
                                  std::ranges::end(absolute_errors), 0.0);

    return sum / static_cast<double>(actual.size());
}

double mse(const std::vector<double>& actual, const std::vector<double>& predicted)
{
    auto zipped = std::views::zip(actual, predicted);

    auto squared_errors = zipped | std::views::transform([](const auto& pair) {
        auto [a, p] = pair;
        double diff = a - p;
        return diff * diff;
    });

    double sum = std::accumulate(std::ranges::begin(squared_errors),
                                  std::ranges::end(squared_errors), 0.0);

    return sum / static_cast<double>(actual.size());
}

class Fibonacci : public std::ranges::view_interface<Fibonacci>
{
private:
    class Iterator
    {
    public:
        using iterator_concept = std::forward_iterator_tag;
        using iterator_category = std::forward_iterator_tag;
        using value_type = int;
        using difference_type = std::ptrdiff_t;
        using pointer = const int*;
        using reference = const int&;

        Iterator() : m_current(0), m_next(1), m_index(0) {}

        Iterator(int first, int second, std::size_t index)
            : m_current(first), m_next(second), m_index(index) {}

        value_type operator*() const { return m_current; }

        Iterator& operator++()
        {
            auto temp = m_next;
            m_next = m_current + m_next;
            m_current = temp;
            ++m_index;
            return *this;
        }

        Iterator operator++(int)
        {
            auto copy = *this;
            ++(*this);
            return copy;
        }

        bool operator==(const Iterator& other) const
        {
            return m_index == other.m_index;
        }

        bool operator!=(const Iterator& other) const
        {
            return !(*this == other);
        }

    private:
        int m_current;
        int m_next;
        std::size_t m_index;
    };

public:
    Fibonacci() = default;

    Fibonacci(int count) : m_count(static_cast<std::size_t>(count < 0 ? 0 : count)) {}

    Iterator begin() const { return Iterator(0, 1, 0); }

    Iterator end() const { return Iterator(0, 1, m_count); }

private:
    std::size_t m_count = 0;
};

void test_ranges_algorithms()
{
    std::vector<int> data = {1, 2, 3, 2, 4, 2, 5, 2};

    std::ranges::replace(data, 2, 9);
    assert(data[1] == 9);
    assert(data[3] == 9);

    std::ranges::fill(data, 0);
    for (int x : data) { assert(x == 0); }

    std::vector<int> unique_data = {1, 1, 2, 2, 3, 3};
    auto [first, last] = std::ranges::unique(unique_data);
    unique_data.erase(first, last);
    assert(unique_data == std::vector<int>({1, 2, 3}));

    std::vector<int> rotate_data = {1, 2, 3, 4, 5};
    std::ranges::rotate(rotate_data, rotate_data.begin() + 2);
    assert(rotate_data == std::vector<int>({3, 4, 5, 1, 2}));

    std::vector<int> population = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> sample_result(3);
    std::mt19937 engine{std::random_device{}()};
    std::ranges::sample(population, sample_result.begin(), 3, engine);
    assert(sample_result.size() == 3);
}

void test_transform_if()
{
    std::vector<int> data = {1, 2, 3, 4, 5, 6};

    auto is_even = [](int x) { return x % 2 == 0; };
    auto square = [](int x) { return x * x; };

    auto result = transform_if(data, is_even, square);
    assert(result == std::vector<int>({4, 16, 36}));
}

void test_mae_mse()
{
    std::vector<double> actual = {1.0, 2.0, 3.0};
    std::vector<double> predicted = {1.5, 2.5, 2.5};

    double mae_result = mae(actual, predicted);
    double expected_mae = (0.5 + 0.5 + 0.5) / 3.0;
    assert(std::abs(mae_result - expected_mae) < 1e-6);

    double mse_result = mse(actual, predicted);
    double expected_mse = (0.25 + 0.25 + 0.25) / 3.0;
    assert(std::abs(mse_result - expected_mse) < 1e-6);
}

void test_views()
{
    std::vector<int> data = {1, 2, 3, 4, 5, 6};

    auto filtered = data | std::views::filter([](int x) { return x % 2 == 0; });
    std::vector<int> filtered_vec;
    std::ranges::copy(filtered, std::back_inserter(filtered_vec));
    assert(filtered_vec == std::vector<int>({2, 4, 6}));

    auto dropped = data | std::views::drop(3);
    std::vector<int> dropped_vec;
    std::ranges::copy(dropped, std::back_inserter(dropped_vec));
    assert(dropped_vec == std::vector<int>({4, 5, 6}));

    std::vector<std::vector<int>> nested = {{1, 2}, {3, 4}, {5, 6}};
    auto joined = nested | std::views::join;
    std::vector<int> joined_vec;
    std::ranges::copy(joined, std::back_inserter(joined_vec));
    assert(joined_vec == std::vector<int>({1, 2, 3, 4, 5, 6}));

    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {4, 5, 6};
    auto zipped = std::views::zip(a, b);
    int expected_pairs[3][2] = {{1, 4}, {2, 5}, {3, 6}};
    int index = 0;
    for (auto [x, y] : zipped)
    {
        assert(x == expected_pairs[index][0]);
        assert(y == expected_pairs[index][1]);
        ++index;
    }

    auto strided = data | std::views::stride(2);
    std::vector<int> strided_vec;
    std::ranges::copy(strided, std::back_inserter(strided_vec));
    assert(strided_vec == std::vector<int>({1, 3, 5}));
}

void test_fibonacci_view()
{
    Fibonacci fib(10);
    int expected[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};
    int index = 0;
    for (int value : fib)
    {
        assert(value == expected[index]);
        ++index;
    }
    assert(index == 10);

    Fibonacci empty(0);
    int count = 0;
    for ([[maybe_unused]] int value : empty)
    {
        ++count;
    }
    assert(count == 0);

    Fibonacci negative(-5);
    count = 0;
    for ([[maybe_unused]] int value : negative)
    {
        ++count;
    }
    assert(count == 0);
}

int main()
{
    test_ranges_algorithms();
    test_transform_if();
    test_mae_mse();
    test_views();
    test_fibonacci_view();

    std::cout << "All tests passed.\n";

    std::cout << "Fibonacci sequence (10 elements): ";
    for (int value : Fibonacci(10))
    {
        std::cout << value << ' ';
    }
    std::cout << '\n';

    std::cout << "transform_if demo (even squares): ";
    std::vector<int> demo_data = {1, 2, 3, 4, 5, 6};
    auto demo = transform_if(demo_data,
                              [](int x) { return x % 2 == 0; },
                              [](int x) { return x * x; });
    for (int x : demo)
    {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    std::cout << "MAE/MSE demo: ";
    std::vector<double> actual = {1.0, 2.0, 3.0};
    std::vector<double> predicted = {1.5, 2.5, 2.5};
    std::cout << "MAE = " << mae(actual, predicted)
              << ", MSE = " << mse(actual, predicted) << '\n';

    return 0;
}