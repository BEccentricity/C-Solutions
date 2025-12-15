#include <cassert>

class Entity_v1 {
public:
    virtual ~Entity_v1() = default;
    virtual int test(int value) {
        return value * 2;
    }
};

class Entity_v2 {
public:
    virtual ~Entity_v2() = default;
    virtual int test(int value) {
        return value + 10;
    }
};

class Adapter_v1 : public Entity_v1 {
public:
    virtual int test_v1(int value) = 0;
    
    int test(int value) override final {
        return test_v1(value);
    }
};

class Adapter_v2 : public Entity_v2 {
public:
    virtual int test_v2(int value) = 0;
    
    int test(int value) override final {
        return test_v2(value);
    }
};

class Client : public Adapter_v1, public Adapter_v2 {
public:
    int test_v1(int value) override {
        return value * 3;
    }
    
    int test_v2(int value) override {
        return value + 20;
    }
};

int main() {
    Client client;
    
    Entity_v1* entity_1 = &client;
    assert(entity_1->test(5) == 15);
    
    Entity_v2* entity_2 = &client;
    assert(entity_2->test(5) == 25);
    
    Adapter_v1* adapter_1 = &client;
    assert(adapter_1->test_v1(5) == 15);
    
    Adapter_v2* adapter_2 = &client;
    assert(adapter_2->test_v2(5) == 25);
    
    return 0;
}