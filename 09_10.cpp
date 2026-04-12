#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <new>
#include <print>
#include <random>
#include <utility>
#include <vector>

#include <boost/noncopyable.hpp>

#include <benchmark/benchmark.h>

class Allocator : private boost::noncopyable
{
public:
    virtual ~Allocator() = default;

    virtual void * allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t)) = 0;

    virtual void deallocate(void * ptr) = 0;

protected:
    template < typename T >
    T * get(void * ptr) const
    {
        return static_cast < T * > (ptr);
    }

    template < typename T >
    const T * get(const void * ptr) const
    {
        return static_cast < const T * > (ptr);
    }
};

class LinearAllocator : public Allocator
{
public:
    LinearAllocator(std::size_t size) : m_size(size)
    {
        m_begin = ::operator new(m_size, std::align_val_t(s_alignment));
    }

    ~LinearAllocator()
    {
        ::operator delete(m_begin, m_size, std::align_val_t(s_alignment));
    }

    void * allocate(std::size_t size, std::size_t alignment = s_alignment) override
    {
        void * begin = get < std::byte > (m_begin) + m_offset;

        auto free = m_size - m_offset;

        if (begin = std::align(alignment, size, begin, free); begin)
        {
            m_offset = m_size - free + size;

            return begin;
        }

        return nullptr;
    }

    void deallocate(void * /* ptr */) override
    {}

    void show() const
    {
        std::print
        (
            "LinearAllocator::show : m_size = {} m_begin = {:018} m_offset = {:0>4}\n",

            m_size, m_begin, m_offset
        );
    }

private:
    std::size_t m_size = 0;
    std::size_t m_offset = 0;
    void * m_begin = nullptr;

    static inline auto s_alignment = alignof(std::max_align_t);
};

class StackAllocator : public Allocator
{
public:
    StackAllocator(std::size_t size) : m_size(size)
    {
        m_begin = ::operator new(m_size, std::align_val_t(s_alignment));
    }

    ~StackAllocator()
    {
        ::operator delete(m_begin, m_size, std::align_val_t(s_alignment));
    }

    void * allocate(std::size_t size, std::size_t alignment = s_alignment) override
    {
        void * begin = get < std::byte > (m_begin) + m_offset + sizeof(header_t);

        auto free = m_size - m_offset - sizeof(header_t);

        if (begin = std::align(alignment, size, begin, free); begin)
        {
            auto header = get < header_t > (get < std::byte > (begin) - sizeof(header_t));

            *header = static_cast < header_t > (get < std::byte > (begin) - (get < std::byte > (m_begin) + m_offset));

            m_offset = get < std::byte > (begin) - get < std::byte > (m_begin) + size;

            return begin;
        }

        return nullptr;
    }

    void deallocate(void * ptr) override
    {
        auto header = get < header_t > (get < std::byte > (ptr) - sizeof(header_t));

        m_offset = get < std::byte > (ptr) - get < std::byte > (m_begin) - *header;
    }

    void show() const
    {
        std::print
        (
            "StackAllocator::show : m_size = {} m_begin = {:018} m_offset = {:0>4}\n",

            m_size, m_begin, m_offset
        );
    }

private:
    using header_t = std::uint8_t;

    std::size_t m_size = 0;
    std::size_t m_offset = 0;
    void * m_begin = nullptr;

    static inline auto s_alignment = alignof(std::max_align_t);
};

class ListAllocator : public Allocator
{
public:
    ListAllocator(std::size_t size, std::size_t step) : m_size(size), m_step(step)
    {
        assert(m_size % m_step == 0 && m_step >= sizeof(Node));

        make_list();

        m_begin = m_head;
    }

    ~ListAllocator()
    {
        for (auto list : m_lists)
        {
            ::operator delete(list, m_size, std::align_val_t(s_alignment));
        }
    }

    void * allocate(std::size_t size, std::size_t alignment = s_alignment) override
    {
        (void)size;
        (void)alignment;

        if (!m_head)
        {
            if (m_offset == std::size(m_lists))
            {
                make_list();
            }
            else
            {
                m_head = get < Node > (m_lists[++m_offset - 1]);
            }
        }

        auto node = m_head;

        if (!node->next)
        {
            auto next = get < std::byte > (node) + m_step;

            if (next != get < std::byte > (m_lists[m_offset - 1]) + m_size)
            {
                m_head = get < Node > (next);

                m_head->next = nullptr;
            }
            else
            {
                m_head = m_head->next;
            }
        }
        else
        {
            m_head = m_head->next;
        }

        return node;
    }

    void deallocate(void * ptr) override
    {
        auto node = get < Node > (ptr);

        node->next = m_head;

        m_head = node;
    }

    void show() const
    {
        std::print
        (
            "ListAllocator::show : m_size = {} m_step = {} m_begin = {:018} m_head = {:018} m_offset = {}\n",

            m_size, m_step, m_begin, static_cast < void * > (m_head), m_offset
        );
    }

private:
    struct Node
    {
        Node * next = nullptr;
    };

    void make_list()
    {
        m_head = get < Node > (::operator new(m_size, std::align_val_t(s_alignment)));

        m_head->next = nullptr;

        ++m_offset;

        m_lists.push_back(m_head);
    }

    std::size_t m_size = 0;
    std::size_t m_step = 0;
    std::size_t m_offset = 0;
    void * m_begin = nullptr;
    Node * m_head = nullptr;
    std::vector < void * > m_lists;

    static inline auto s_alignment = alignof(std::max_align_t);
};

class FreeListAllocator : public Allocator
{
public:
    FreeListAllocator(std::size_t size) : m_size(size)
    {
        assert(m_size >= sizeof(Node) + 1);

        m_begin = ::operator new(m_size, std::align_val_t(s_alignment));

        m_head = get < Node > (m_begin);

        m_head->size = m_size - sizeof(Header);

        m_head->next = nullptr;
    }

    ~FreeListAllocator()
    {
        ::operator delete(m_begin, m_size, std::align_val_t(s_alignment));
    }

    void * allocate(std::size_t size, std::size_t alignment = s_alignment) override
    {
        (void)alignment;

        void * end = get < std::byte > (m_begin) + sizeof(Header) + size;
        void * next = end;

        auto free = 2 * alignof(Header);

        if (next = std::align(alignof(Header), sizeof(Header), next, free); next)
        {
            auto padding = get < std::byte > (next) - get < std::byte > (end);

            if (auto [current, previous] = find(size + padding); current)
            {
                if (current->size >= size + padding + sizeof(Node) + 1)
                {
                    auto step = sizeof(Header) + size + padding;

                    auto node = get < Node > (get < std::byte > (current) + step);

                    node->size = current->size - step;

                    node->next = current->next;

                    current->next = node;
                }
                else
                {
                    padding += current->size - size - padding;
                }

                if (!previous)
                {
                    m_head = current->next;
                }
                else
                {
                    previous->next = current->next;
                }

                auto header = get < Header > (current);

                header->size = size + padding;

                return get < std::byte > (current) + sizeof(Header);
            }
        }

        return nullptr;
    }

    void deallocate(void * ptr) override
    {
        auto node = get < Node > (get < std::byte > (ptr) - sizeof(Header));

        Node * previous = nullptr;
        Node * current = m_head;

        while (current)
        {
            if (node < current)
            {
                node->next = current;

                if (!previous)
                {
                    m_head = node;
                }
                else
                {
                    previous->next = node;
                }

                break;
            }

            previous = current;

            current = current->next;
        }

        merge(previous, node);
    }

    void show() const
    {
        std::print
        (
            "FreeListAllocator::show : m_size = {} m_begin = {:018} m_head = {:018} ",

            m_size, m_begin, static_cast < void * > (m_head)
        );

        if (m_head->next)
        {
            std::print("m_head->next = {:018}\n", static_cast < void * > (m_head->next));
        }
        else
        {
            std::print("\n");
        }
    }

private:
    struct Node
    {
        std::size_t size = 0;
        Node * next = nullptr;
    };

    struct alignas(std::max_align_t) Header
    {
        std::size_t size = 0;
    };

    auto find(std::size_t size) const -> std::pair < Node *, Node * >
    {
        Node * current = m_head;
        Node * previous = nullptr;

        while (current && size > current->size)
        {
            previous = current;

            current = current->next;
        }

        return std::make_pair(current, previous);
    }

    void merge(Node * previous, Node * node) const
    {
        if (node->next && get < std::byte > (node) + sizeof(Header) + node->size == get < std::byte > (node->next))
        {
            node->size += sizeof(Header) + node->next->size;

            node->next = node->next->next;
        }

        if (previous && get < std::byte > (previous) + sizeof(Header) + previous->size == get < std::byte > (node))
        {
            previous->size += sizeof(Header) + node->size;

            previous->next = node->next;
        }
    }

    std::size_t m_size = 0;
    void * m_begin = nullptr;
    Node * m_head = nullptr;

    static inline auto s_alignment = alignof(std::max_align_t);
};

void test_linear_v1(benchmark::State & state)
{
    auto kb = 1uz << 10;
    auto mb = 1uz << 20;

    std::vector < void * > vector(kb, nullptr);

    for (auto element : state)
    {
        for (auto i = 0uz; i < kb; ++i)
        {
            vector[i] = ::operator new(mb);
        }

        for (auto i = 0uz; i < kb; ++i)
        {
            ::operator delete(vector[i], mb);
        }

        benchmark::DoNotOptimize(vector);
    }
}

void test_linear_v2(benchmark::State & state)
{
    auto kb = 1uz << 10;
    auto mb = 1uz << 20;
    auto gb = 1uz << 30;

    std::vector < void * > vector(kb, nullptr);

    for (auto element : state)
    {
        LinearAllocator allocator(gb);

        for (auto i = 0uz; i < kb; ++i)
        {
            vector[i] = allocator.allocate(mb);
        }

        benchmark::DoNotOptimize(vector);
    }
}

BENCHMARK(test_linear_v1);
BENCHMARK(test_linear_v2);

void test_stack_v1(benchmark::State & state)
{
    auto kb = 1uz << 10;
    auto mb = 1uz << 20;

    std::vector < void * > vector(kb, nullptr);

    for (auto element : state)
    {
        for (auto i = 0uz; i < kb; ++i)
        {
            vector[i] = ::operator new(mb);
        }

        for (auto i = 0uz; i < kb; ++i)
        {
            ::operator delete(vector[std::size(vector) - 1 - i], mb);
        }

        benchmark::DoNotOptimize(vector);
    }
}

void test_stack_v2(benchmark::State & state)
{
    auto kb = 1uz << 10;
    auto mb = 1uz << 20;
    auto gb = 1uz << 30;

    std::vector < void * > vector(kb, nullptr);

    for (auto element : state)
    {
        StackAllocator allocator(2 * gb);

        for (auto i = 0uz; i < kb; ++i)
        {
            vector[i] = allocator.allocate(mb);
        }

        for (auto i = 0uz; i < kb; ++i)
        {
            allocator.deallocate(vector[std::size(vector) - 1 - i]);
        }

        benchmark::DoNotOptimize(vector);
    }
}

BENCHMARK(test_stack_v1);
BENCHMARK(test_stack_v2);

void test_list_v1(benchmark::State & state)
{
    auto kb = 1uz << 10;
    auto mb = 1uz << 20;

    std::vector < void * > vector(kb, nullptr);

    for (auto element : state)
    {
        for (auto i = 0uz; i < kb; ++i)
        {
            vector[i] = ::operator new(mb);
        }

        for (auto i = 0uz; i < kb; i += 2)
        {
            ::operator delete(vector[i], mb);
        }

        for (auto i = 0uz; i < kb; i += 2)
        {
            vector[i] = ::operator new(mb);
        }

        for (auto i = 0uz; i < kb; ++i)
        {
            ::operator delete(vector[i], mb);
        }

        benchmark::DoNotOptimize(vector);
    }
}

void test_list_v2(benchmark::State & state)
{
    auto kb = 1uz << 10;
    auto mb = 1uz << 20;
    auto gb = 1uz << 30;

    std::vector < void * > vector(kb, nullptr);

    for (auto element : state)
    {
        ListAllocator allocator(gb, mb);

        for (auto i = 0uz; i < kb; ++i)
        {
            vector[i] = allocator.allocate(0);
        }

        for (auto i = 0uz; i < kb; i += 2)
        {
            allocator.deallocate(vector[i]);
        }

        for (auto i = 0uz; i < kb; i += 2)
        {
            vector[i] = allocator.allocate(0);
        }

        for (auto i = 0uz; i < kb; ++i)
        {
            allocator.deallocate(vector[i]);
        }

        benchmark::DoNotOptimize(vector);
    }
}

BENCHMARK(test_list_v1);
BENCHMARK(test_list_v2);

void test_freelist_v1(benchmark::State & state)
{
    auto kb = 1uz << 10;
    auto mb = 1uz << 20;

    std::uniform_int_distribution distribution(1, 16);
    std::default_random_engine engine;

    std::vector < std::pair < void *, std::size_t > > vector(kb);

    for (auto element : state)
    {
        for (auto i = 0uz; i < kb; ++i)
        {
            vector[i].second = distribution(engine) * mb;
            vector[i].first  = ::operator new(vector[i].second);
        }

        for (auto i = 0uz; i < kb; i += 32)
        {
            ::operator delete(vector[i].first, vector[i].second);
        }

        for (auto i = 0uz; i < kb; i += 32)
        {
            vector[i].second = distribution(engine) * mb;
            vector[i].first  = ::operator new(vector[i].second);
        }

        for (auto i = 0uz; i < kb; ++i)
        {
            ::operator delete(vector[i].first, vector[i].second);
        }

        benchmark::DoNotOptimize(vector);
    }
}

void test_freelist_v2(benchmark::State & state)
{
    auto kb = 1uz << 10;
    auto mb = 1uz << 20;
    auto gb = 1uz << 30;

    std::uniform_int_distribution distribution(1, 16);
    std::default_random_engine engine;

    std::vector < void * > vector(kb, nullptr);

    for (auto element : state)
    {
        FreeListAllocator allocator(16 * gb);

        for (auto i = 0uz; i < kb; ++i)
        {
            vector[i] = allocator.allocate(distribution(engine) * mb);
        }

        for (auto i = 0uz; i < kb; i += 32)
        {
            allocator.deallocate(vector[i]);
        }

        for (auto i = 0uz; i < kb; i += 32)
        {
            vector[i] = allocator.allocate(distribution(engine) * mb);
        }

        for (auto i = 0uz; i < kb; ++i)
        {
            allocator.deallocate(vector[i]);
        }

        benchmark::DoNotOptimize(vector);
    }
}

BENCHMARK(test_freelist_v1);
BENCHMARK(test_freelist_v2);

int main()
{
    std::vector < Allocator * > allocators;

    allocators.push_back(new LinearAllocator(1 << 10));
    allocators.push_back(new StackAllocator(1 << 10));
    allocators.push_back(new ListAllocator(32, 8));
    allocators.push_back(new FreeListAllocator(1 << 10));

    for (auto alloc : allocators)
    {
        void * p1 = alloc->allocate(16);
        void * p2 = alloc->allocate(16);
        void * p3 = alloc->allocate(16);

        alloc->deallocate(p2);
        alloc->deallocate(p1);
        alloc->deallocate(p3);
    }

    for (auto alloc : allocators)
    {
        delete alloc;
    }

    benchmark::RunSpecifiedBenchmarks();
}