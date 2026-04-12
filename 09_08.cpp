#include <cstddef>
#include <new>
#include <print>

template < typename D >
class Entity
{
public:
    static auto operator new(std::size_t size) -> void *
    {
        std::print("Entity::operator new\n");

        return ::operator new(size);
    }

    static void operator delete(void * x)
    {
        std::print("Entity::operator delete\n");

        ::operator delete(x);
    }

    static auto operator new(std::size_t size, const std::nothrow_t & tag) noexcept -> void *
    {
        std::print("Entity::operator new(nothrow)\n");

        return ::operator new(size, tag);
    }

    static void operator delete(void * x, const std::nothrow_t & tag) noexcept
    {
        std::print("Entity::operator delete(nothrow)\n");

        ::operator delete(x, tag);
    }

    static auto operator new[](std::size_t size) -> void *
    {
        std::print("Entity::operator new[]\n");

        return ::operator new[](size);
    }

    static void operator delete[](void * x)
    {
        std::print("Entity::operator delete[]\n");

        ::operator delete[](x);
    }

    static auto operator new[](std::size_t size, const std::nothrow_t & tag) noexcept -> void *
    {
        std::print("Entity::operator new[](nothrow)\n");

        return ::operator new[](size, tag);
    }

    static void operator delete[](void * x, const std::nothrow_t & tag) noexcept
    {
        std::print("Entity::operator delete[](nothrow)\n");

        ::operator delete[](x, tag);
    }

protected:
    Entity() = default;
};

class Client : private Entity < Client >
{
public:
    Client() { std::print("Client:: Client\n"); }

   ~Client() { std::print("Client::~Client\n"); }

    using Entity < Client > ::operator new;
    using Entity < Client > ::operator delete;
    using Entity < Client > ::operator new[];
    using Entity < Client > ::operator delete[];
};

int main()
{
    delete new Client;

    Client * c2 = new (std::nothrow) Client;
    delete c2;

    Client * arr = new Client[3];
    delete[] arr;

    Client * arr2 = new (std::nothrow) Client[2];
    delete[] arr2;
}