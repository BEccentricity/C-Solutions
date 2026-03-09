#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

template < typename D = std::chrono::duration < double > > class Timer
{
public:
    Timer(std::string const & scope) : m_scope(scope), m_is_running(false) {}

    void start()
    {
        if (!m_is_running)
        {
            m_begin = clock_t::now();
            m_is_running = true;
        }
    }

    void stop()
    {
        if (m_is_running)
        {
            auto end = clock_t::now();
            auto elapsed = std::chrono::duration_cast < D > (end - m_begin);
            m_measurements.push_back(elapsed);
            m_is_running = false;
        }
    }

    double average() const
    {
        if (m_measurements.empty())
        {
            return 0.0;
        }

        double sum = 0.0;
        for (const auto& measurement : m_measurements)
        {
            sum += measurement.count();
        }
        return sum / m_measurements.size();
    }

    void reset()
    {
        m_measurements.clear();
        m_is_running = false;
    }

    std::size_t count() const
    {
        return m_measurements.size();
    }

    void report() const
    {
        std::cout << m_scope << ":\n";
        std::cout << "  Measurements: " << count() << "\n";
        std::cout << "  Average time: " << average() << " s\n";
        
        if (!m_measurements.empty())
        {
            auto min_it = std::min_element(m_measurements.begin(), m_measurements.end());
            auto max_it = std::max_element(m_measurements.begin(), m_measurements.end());
            
            std::cout << "  Min time: " << min_it->count() << " s\n";
            std::cout << "  Max time: " << max_it->count() << " s\n";
            
            double sum = 0.0;
            for (const auto& m : m_measurements)
            {
                sum += m.count();
            }
            double mean = sum / m_measurements.size();
            
            double sq_sum = 0.0;
            for (const auto& m : m_measurements)
            {
                sq_sum += (m.count() - mean) * (m.count() - mean);
            }
            double stddev = std::sqrt(sq_sum / m_measurements.size());
            
            std::cout << "  Std deviation: " << stddev << " s\n";
        }
    }

    ~Timer()
    {
        if (m_is_running)
        {
            stop();
        }
        report();
    }

private:
    using clock_t = std::chrono::steady_clock;

    std::string m_scope;
    bool m_is_running;
    clock_t::time_point m_begin;
    std::vector<D> m_measurements;
};

auto calculate(std::size_t size)
{
    auto x = 0.0;
    for (auto i = 0uz; i < size; ++i)
    {
        x += std::pow(std::sin(i), 2) + std::pow(std::cos(i), 2);
    }
    return x;
}

auto equal(double x, double y, double epsilon = 1e-6)
{
    return std::abs(x - y) < epsilon;
}

int main()
{
    std::cout << "=== Test 1: Single measurement ===\n";
    {
        Timer timer("Single measurement");
        timer.start();
        auto result = calculate(1'000'000);
        timer.stop();
        assert(equal(result, 1'000'000));
        std::cout << "Calculation result: " << result << "\n\n";
    }

    std::cout << "\n=== Test 2: Series of measurements ===\n";
    {
        Timer timer("Series of measurements");
        
        for (int i = 0; i < 5; ++i)
        {
            timer.start();
            auto result = calculate(500'000);
            timer.stop();
            assert(equal(result, 500'000));
        }
    }

    std::cout << "\n=== Test 3: Measurements with different loads ===\n";
    {
        Timer timer("Different loads");
        std::vector<std::size_t> sizes = {100'000, 200'000, 500'000, 1'000'000, 2'000'000};
        
        for (std::size_t size : sizes)
        {
            timer.start();
            auto result = calculate(size);
            timer.stop();
            assert(equal(result, static_cast<double>(size)));
        }
    }

    std::cout << "\n=== Test 4: Reset and count methods ===\n";
    {
        Timer timer("Reset test");
        
        std::cout << "Initial measurements count: " << timer.count() << "\n";
        
        timer.start();
        calculate(1'000'000);
        timer.stop();
        
        std::cout << "After first measurement: " << timer.count() << "\n";
        
        timer.start();
        calculate(2'000'000);
        timer.stop();
        
        std::cout << "After second measurement: " << timer.count() << "\n";
        std::cout << "Average time: " << timer.average() << " s\n";
        
        timer.reset();
        std::cout << "After reset: " << timer.count() << "\n";
        
        timer.start();
        calculate(500'000);
        timer.stop();
        
        std::cout << "After new measurement: " << timer.count() << "\n";
    }

    std::cout << "\n=== Test 5: Double start/stop ===\n";
    {
        Timer timer("Double start/stop");
        
        timer.start();
        timer.start();
        calculate(1'000'000);
        timer.stop();
        timer.stop();
        
        std::cout << "Measurements after double operations: " << timer.count() << "\n";
    }

    std::cout << "\n=== Test 6: Manual report ===\n";
    {
        Timer timer("Manual report");
        
        for (int i = 0; i < 3; ++i)
        {
            timer.start();
            calculate(1'000'000);
            timer.stop();
        }
        
        std::cout << "Manual report before destructor:\n";
        timer.report();
        std::cout << "\n";
    }

    std::cout << "\n=== Test 7: Empty timer ===\n";
    {
        Timer timer("Empty timer");
        std::cout << "Measurements: " << timer.count() << "\n";
        std::cout << "Average: " << timer.average() << " s\n";
    }

    return 0;
}