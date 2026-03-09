#include <bit>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>

class Entity_v1
{
private:
    int m_value;
    
public:
    explicit Entity_v1(int value) : m_value(value) {}
    
    int get_value() const
    {
        return m_value;
    }
};

class Entity_v2
{
public:
    int m_value;
    
    explicit Entity_v2(int value) : m_value(value) {}
};

void fraudulent_modification_v1(Entity_v1& entity, int new_value)
{
    Entity_v2& fake_entity = reinterpret_cast<Entity_v2&>(entity);
    fake_entity.m_value = new_value;
}

void fraudulent_modification_v2(Entity_v1& entity, int new_value)
{
    char* byte_ptr = reinterpret_cast<char*>(&entity);
    
    // Предполагаем, что приватное поле находится в начале объекта
    int* value_ptr = reinterpret_cast<int*>(byte_ptr);
    *value_ptr = new_value;
}

void fraudulent_modification_v3(Entity_v1& entity, int new_value)
{
    struct EntityPunned
    {
        int value;
    };
    
    EntityPunned pun = std::bit_cast<EntityPunned>(entity);
    pun.value = new_value;
    entity = std::bit_cast<Entity_v1>(pun);
}

void fraudulent_modification_v4(Entity_v1& entity, int new_value)
{
    Entity_v2 temp(new_value);
    
    std::memcpy(&entity, &temp, sizeof(Entity_v1));
}

union EntityPun
{
    Entity_v1 m_v1;
    Entity_v2 m_v2;
    
    EntityPun(const Entity_v1& e) : m_v1(e) {}
};

void fraudulent_modification_v5(Entity_v1& entity, int new_value)
{
    EntityPun pun(entity);
    pun.m_v2.m_value = new_value;
    entity = pun.m_v1;
}

template<typename T>
class ByteView
{
private:
    T& m_object;
    
public:
    explicit ByteView(T& obj) : m_object(obj) {}
    
    template<typename U>
    U& as()
    {
        static_assert(sizeof(T) >= sizeof(U));
        return *reinterpret_cast<U*>(&m_object);
    }
};


void fraudulent_modification_v6(Entity_v1& entity, int new_value)
{
    ByteView<Entity_v1> view(entity);
    view.as<int>() = new_value;
}

int main()
{
    Entity_v1 original(42);
    
    std::cout << "Original value: " << original.get_value() << "\n\n";
    
    fraudulent_modification_v1(original, 100);
    std::cout << "After method 1 (reinterpret_cast with struct trick): " 
              << original.get_value() << "\n";

    fraudulent_modification_v2(original, 200);
    std::cout << "After method 2 (pointer arithmetic): " 
              << original.get_value() << "\n";

    fraudulent_modification_v3(original, 300);
    std::cout << "After method 3 (std::bit_cast): " 
              << original.get_value() << "\n";

    fraudulent_modification_v4(original, 400);
    std::cout << "After method 4 (std::memcpy): " 
              << original.get_value() << "\n";

    fraudulent_modification_v5(original, 500);
    std::cout << "After method 5 (union type punning): " 
              << original.get_value() << "\n";

    fraudulent_modification_v6(original, 600);
    std::cout << "After method 6 (template byte view): " 
              << original.get_value() << "\n";
    
    assert(original.get_value() == 600);
    
    return 0;
}