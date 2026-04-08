#include <boost/iterator/iterator_facade.hpp>

class FibonacciIterator : public boost::iterator_facade <
    FibonacciIterator,
    int,
    boost::forward_traversal_tag
>
{
public:
    FibonacciIterator() : m_current(0), m_next(1) {}
    
    FibonacciIterator(int first, int second) : m_current(first), m_next(second) {}

private:
    friend class boost::iterator_core_access;

    void increment()
    {
        auto temp = m_next;
        m_next = m_current + m_next;
        m_current = temp;
    }

    bool equal(const FibonacciIterator& other) const
    {
        return m_current == other.m_current && m_next == other.m_next;
    }

    int& dereference() const
    {
        return const_cast<int&>(m_current);
    }

    int m_current;
    int m_next;
};

class FibonacciSequence
{
public:
    FibonacciSequence(int count) : m_count(count) {}

    FibonacciIterator begin() const
    {
        return FibonacciIterator(0, 1);
    }

    FibonacciIterator end() const
    {
        auto it = FibonacciIterator(0, 1);
        for (int i = 0; i < m_count; ++i)
        {
            ++it;
        }
        return it;
    }

private:
    int m_count;
};

int main()
{
    FibonacciSequence fib(10);
    
    for (auto value : fib)
    {
        static_cast<void>(value);
    }
    
    FibonacciIterator it(0, 1);
    FibonacciIterator end_it(55, 89);
    
    while (it != end_it)
    {
        static_cast<void>(*it);
        ++it;
    }
    
    return 0;
}