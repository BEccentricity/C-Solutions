#include <cassert>

class Entity {
private:
    int test_v1(int a, int b) {
        return a + b;
    }
    
    double test_v2(double x, double y) {
        return x * y;
    }

    friend class Tester_v1;
    friend class Tester_v2;
};

class Tester_v1 {
private:
    static int call_test_v1(Entity& entity, int a, int b) {
        return entity.test_v1(a, b);
    }
    
    friend class Client_v1;
};

class Tester_v2 {
private:
    static double call_test_v2(Entity& entity, double x, double y) {
        return entity.test_v2(x, y);
    }
    
    friend class Client_v2;
};

class Client_v1 {
public:
    static void test() {
        Entity entity_1;
        assert(Tester_v1::call_test_v1(entity_1, 2, 3) == 5);
        
        Entity entity_2;
        assert(Tester_v1::call_test_v1(entity_2, -5, 10) == 5);
        
        Entity entity_3;
        assert(Tester_v1::call_test_v1(entity_3, 0, 0) == 0);
    }
};

class Client_v2 {
public:
    static void test() {
        Entity entity_1;
        assert(Tester_v2::call_test_v2(entity_1, 2.0, 3.0) == 6.0);
        
        Entity entity_2;
        assert(Tester_v2::call_test_v2(entity_2, 1.5, 2.0) == 3.0);
        
        Entity entity_3;
        assert(Tester_v2::call_test_v2(entity_3, 5.0, 0.0) == 0.0);
    }
};

int main() {
    Client_v1::test();
    Client_v2::test();
    
    return 0;
}