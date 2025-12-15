#include <cassert>
#include <memory>
#include <string>
#include <vector>

class Unit {
public:
    virtual ~Unit() = default;
    virtual void attack() const = 0;
    virtual void move() const = 0;
    virtual std::string name() const = 0;
    virtual int power() const = 0;
};

class InfantryUnit : public Unit {
public:
    void attack() const override {
    }
    
    void move() const override {
    }
    
    std::string name() const override {
        return "Infantry";
    }
    
    int power() const override {
        return 10;
    }
};

class CavalryUnit : public Unit {
public:
    void attack() const override {
    }
    
    void move() const override {
    }
    
    std::string name() const override {
        return "Cavalry";
    }
    
    int power() const override {
        return 25;
    }
};

class UnitBuilder {
public:
    virtual ~UnitBuilder() = default;
    virtual std::unique_ptr<Unit> build() = 0;
    virtual void reset() = 0;
};

class InfantryBuilder : public UnitBuilder {
public:
    InfantryBuilder() {
        reset();
    }
    
    std::unique_ptr<Unit> build() override {
        auto unit = std::make_unique<InfantryUnit>();
        reset();
        return unit;
    }
    
    void reset() override {
    }
};

class CavalryBuilder : public UnitBuilder {
public:
    CavalryBuilder() {
        reset();
    }
    
    std::unique_ptr<Unit> build() override {
        auto unit = std::make_unique<CavalryUnit>();
        reset();
        return unit;
    }
    
    void reset() override {
    }
};

class UnitGroup : public Unit {
private:
    std::vector<std::unique_ptr<Unit>> units;
    
public:
    void add(std::unique_ptr<Unit> unit) {
        units.push_back(std::move(unit));
    }
    
    void attack() const override {
        for (const auto& unit : units) {
            unit->attack();
        }
    }
    
    void move() const override {
        for (const auto& unit : units) {
            unit->move();
        }
    }
    
    std::string name() const override {
        return "UnitGroup";
    }
    
    int power() const override {
        int total = 0;
        for (const auto& unit : units) {
            total += unit->power();
        }
        return total;
    }
    
    size_t size() const {
        return units.size();
    }
};

class BattleTemplate {
protected:
    virtual void prepare() const {
    }
    
    virtual void executeAttack() const = 0;
    
    virtual void retreat() const {
    }
    
public:
    void battle() const {
        prepare();
        executeAttack();
        retreat();
    }
    
    virtual ~BattleTemplate() = default;
};

class AggressiveBattle : public BattleTemplate {
protected:
    void executeAttack() const override {
    }
};

class DefensiveBattle : public BattleTemplate {
protected:
    void executeAttack() const override {
    }
    
    void retreat() const override {
    }
};

int main() {
    InfantryBuilder infantryBuilder;
    CavalryBuilder cavalryBuilder;
    
    auto infantry = infantryBuilder.build();
    auto cavalry = cavalryBuilder.build();
    
    assert(infantry->name() == "Infantry");
    assert(infantry->power() == 10);
    assert(cavalry->name() == "Cavalry");
    assert(cavalry->power() == 25);
    
    UnitGroup army;
    army.add(std::move(infantry));
    army.add(std::move(cavalry));
    
    assert(army.size() == 2);
    assert(army.power() == 35);
    
    AggressiveBattle aggressiveBattle;
    aggressiveBattle.battle();
    
    DefensiveBattle defensiveBattle;
    defensiveBattle.battle();
    
    UnitGroup largeArmy;
    for (int i = 0; i < 5; ++i) {
        largeArmy.add(infantryBuilder.build());
        largeArmy.add(cavalryBuilder.build());
    }
    
    assert(largeArmy.size() == 10);
    assert(largeArmy.power() == 175);
    
    return 0;
}