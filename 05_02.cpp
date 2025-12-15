#include <cassert>
#include <string>

class Entity {
public:
    virtual ~Entity() = default;
    virtual std::string operation() const = 0;
};

class ConcreteEntity : public virtual Entity {
public:
    std::string operation() const override {
        return "ConcreteEntity";
    }
};

template<typename Base>
class Decorator : public virtual Entity, public Base {
public:
    using Base::Base;
    
    std::string operation() const override {
        return "Decorator(" + Base::operation() + ")";
    }
};

class ExtraDecorator : public virtual Entity {
public:
    ExtraDecorator(Entity* e) : entity(e) {}
    
    std::string operation() const override {
        return "ExtraDecorator(" + entity->operation() + ")";
    }
    
private:
    Entity* entity;
};

int main() {
    ConcreteEntity concrete;
    assert(concrete.operation() == "ConcreteEntity");
    
    Decorator<ConcreteEntity> decorated;
    assert(decorated.operation() == "Decorator(ConcreteEntity)");
    
    Decorator<Decorator<ConcreteEntity>> double_decorated;
    assert(double_decorated.operation() == "Decorator(Decorator(ConcreteEntity))");
    
    ExtraDecorator extra(&decorated);
    assert(extra.operation() == "ExtraDecorator(Decorator(ConcreteEntity))");
    
    return 0;
}