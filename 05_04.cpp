#include <cassert>
#include <string>

class AttackBehavior {
public:
    std::string attack() const {
        return "basic attack";
    }
};

class StrongAttackBehavior {
public:
    std::string attack() const {
        return "strong attack";
    }
};

class MagicAttackBehavior {
public:
    std::string attack() const {
        return "magic attack";
    }
};

template<typename Behavior>
class Entity : public Behavior {
public:
    using Behavior::Behavior;
    
    std::string performAttack() const {
        return this->attack();
    }
};

int main() {
    Entity<AttackBehavior> entity1;
    assert(entity1.performAttack() == "basic attack");
    
    Entity<StrongAttackBehavior> entity2;
    assert(entity2.performAttack() == "strong attack");
    
    Entity<MagicAttackBehavior> entity3;
    assert(entity3.performAttack() == "magic attack");
    
    return 0;
}