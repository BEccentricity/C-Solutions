#include <boost/noncopyable.hpp>
#include <array>
#include <cstddef>
#include <memory>
#include <new>
#include <print>
#include <bit>
#include <utility>

class Entity : private boost::noncopyable
{
private:
    class Implementation
    {
    public:
        Implementation(int x) : m_x(x)
        {
            std::print("Implementation::Implementation : m_x = {}\n", m_x);
        }

        ~Implementation()
        {
            std::print("Implementation::~Implementation : m_x = {}\n", m_x);
        }

        Implementation(Implementation && other) noexcept : m_x(other.m_x)
        {
            std::print("Implementation::Implementation(&&) : m_x = {}\n", m_x);
        }

        Implementation & operator=(Implementation && other) noexcept
        {
            if (this != &other)
            {
                m_x = other.m_x;
                std::print("Implementation::operator=(&&) : m_x = {}\n", m_x);
            }
            return *this;
        }

        void test() const
        {
            std::print("Implementation::test : m_x = {}\n", m_x);
        }

    private:
        int m_x;
    };

    struct alignas(std::max_align_t) Storage
    {
        std::array<std::byte, 16> data;
    } m_storage;

    static_assert(sizeof(Implementation) <= sizeof(m_storage.data),
                  "Implementation size exceeds storage capacity");
    static_assert(alignof(Implementation) <= alignof(Storage),
                  "Implementation alignment exceeds storage alignment");

    Implementation * ptr()
    {
        void * vp = m_storage.data.data();
        return std::launder(std::bit_cast<Implementation *>(vp));
    }

    const Implementation * ptr() const
    {
        const void * vp = m_storage.data.data();
        return std::launder(std::bit_cast<const Implementation *>(vp));
    }

public:
    Entity(int x)
    {
        new (m_storage.data.data()) Implementation(x);
    }

    Entity(Entity && other) noexcept
    {
        new (m_storage.data.data()) Implementation(std::move(*other.ptr()));
    }

    Entity & operator=(Entity && other) noexcept
    {
        if (this != &other)
        {
            std::destroy_at(ptr());
            new (m_storage.data.data()) Implementation(std::move(*other.ptr()));
        }
        return *this;
    }

    ~Entity()
    {
        std::destroy_at(ptr());
    }

    void test() const
    {
        ptr()->test();
    }

    Implementation * get()
    {
        return ptr();
    }

    const Implementation * get() const
    {
        return ptr();
    }
};

int main()
{
    Entity e1(42);
    e1.test();

    Entity e2(std::move(e1));
    e2.test();

    e2 = Entity(100);
    e2.test();

    auto * impl = e2.get();
    impl->test();

    const Entity e3(200);
    const auto * cimpl = e3.get();
    cimpl->test();
}