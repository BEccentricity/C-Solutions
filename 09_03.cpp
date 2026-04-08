#include <memory>
#include <vector>
#include <cassert>
#include <iostream>

struct Entity
{
    int x = 0, y = 0;
    virtual ~Entity() = default;
};

class Builder
{
public:
    virtual ~Builder() = default;

    auto make_entity()
    {
        m_entity = std::make_unique<Entity>();
        set_x();
        set_y();
        return std::move(m_entity);
    }

    virtual void set_x() const = 0;
    virtual void set_y() const = 0;

protected:
    std::unique_ptr<Entity> m_entity;
};

class Builder_Client : public Builder
{
public:
    void set_x() const override { m_entity->x = 1; }
    void set_y() const override { m_entity->y = 1; }
};

class Builder_Server : public Builder
{
public:
    void set_x() const override { m_entity->x = 1; }
    void set_y() const override { m_entity->y = 1; }
};

class Client : public Entity {};
class Server : public Entity {};

class Factory
{
public:
    virtual ~Factory() = default;
    virtual std::unique_ptr<Entity> make_entity() const = 0;
};

class Factory_Client : public Factory
{
public:
    std::unique_ptr<Entity> make_entity() const override
    {
        return std::make_unique<Client>();
    }
};

class Factory_Server : public Factory
{
public:
    std::unique_ptr<Entity> make_entity() const override
    {
        return std::make_unique<Server>();
    }
};

class PrototypeEntity
{
public:
    virtual ~PrototypeEntity() = default;
    virtual std::unique_ptr<PrototypeEntity> copy() const = 0;
};

class PrototypeClient : public PrototypeEntity
{
public:
    std::unique_ptr<PrototypeEntity> copy() const override
    {
        return std::make_unique<PrototypeClient>(*this);
    }
};

class PrototypeServer : public PrototypeEntity
{
public:
    std::unique_ptr<PrototypeEntity> copy() const override
    {
        return std::make_unique<PrototypeServer>(*this);
    }
};

class Prototype
{
public:
    Prototype()
    {
        m_entities.push_back(std::make_unique<PrototypeClient>());
        m_entities.push_back(std::make_unique<PrototypeServer>());
    }

    auto make_client() { return m_entities.at(0)->copy(); }
    auto make_server() { return m_entities.at(1)->copy(); }

private:
    std::vector<std::unique_ptr<PrototypeEntity>> m_entities;
};

class CompositeEntity
{
public:
    virtual ~CompositeEntity() = default;
    virtual int test() const = 0;
};

class CompositeClient : public CompositeEntity
{
public:
    int test() const override { return 1; }
};

class CompositeServer : public CompositeEntity
{
public:
    int test() const override { return 2; }
};

class Composite : public CompositeEntity
{
public:
    void add(std::unique_ptr<CompositeEntity> entity)
    {
        m_entities.push_back(std::move(entity));
    }

    int test() const override
    {
        auto x = 0;
        for (const auto& entity : m_entities)
        {
            if (entity)
            {
                x += entity->test();
            }
        }
        return x;
    }

private:
    std::vector<std::unique_ptr<CompositeEntity>> m_entities;
};

std::unique_ptr<CompositeEntity> make_composite(std::size_t size_1, std::size_t size_2)
{
    auto composite = std::make_unique<Composite>();
    for (auto i = 0uz; i < size_1; ++i)
    {
        composite->add(std::make_unique<CompositeClient>());
    }
    for (auto i = 0uz; i < size_2; ++i)
    {
        composite->add(std::make_unique<CompositeServer>());
    }
    return composite;
}

class Observer
{
public:
    virtual ~Observer() = default;
    virtual void test(int x) const = 0;
};

class ObserverEntity
{
public:
    void add(std::shared_ptr<Observer> observer)
    {
        m_observers.push_back(observer);
    }

    void set(int x)
    {
        m_x = x;
        notify_all();
    }

    void notify_all() const
    {
        for (const auto& observer : m_observers)
        {
            if (observer)
            {
                observer->test(m_x);
            }
        }
    }

private:
    int m_x = 0;
    std::vector<std::shared_ptr<Observer>> m_observers;
};

class ObserverClient : public Observer
{
public:
    void test(int x) const override
    {
        std::cout << "Client::test : x = " << x << "\n";
    }
};

class ObserverServer : public Observer
{
public:
    void test(int x) const override
    {
        std::cout << "Server::test : x = " << x << "\n";
    }
};

int main()
{
    {
        std::unique_ptr<Builder> builder = std::make_unique<Builder_Client>();
        auto entity = builder->make_entity();
    }

    {
        std::unique_ptr<Factory> factory = std::make_unique<Factory_Client>();
        auto entity = factory->make_entity();
    }

    {
        Prototype prototype;
        auto client = prototype.make_client();
    }

    {
        auto composite = std::make_unique<Composite>();
        for (auto i = 0uz; i < 5; ++i)
        {
            composite->add(make_composite(1, 1));
        }
        assert(composite->test() == 15);
    }

    {
        ObserverEntity entity;
        entity.add(std::make_shared<ObserverClient>());
        entity.add(std::make_shared<ObserverServer>());
        for (auto i = 0; i < 2; ++i)
        {
            entity.set(i + 1);
        }
    }

    return 0;
}