#include <cassert>
#include <string>

class Person {
public:
    std::string person_name;
    int person_age;
    int person_grade;
    
    Person(std::string n, int a, int g) : person_name(std::move(n)), person_age(a), person_grade(g) {}
};

class Builder {
private:
    std::string name_value;
    int age_value;
    int grade_value;
    
public:
    Builder() : name_value(""), age_value(0), grade_value(0) {}
    
    Builder& name(std::string n) {
        name_value = std::move(n);
        return *this;
    }
    
    Builder& age(int a) {
        age_value = a;
        return *this;
    }
    
    Builder& grade(int g) {
        grade_value = g;
        return *this;
    }
    
    Person get() {
        return Person(name_value, age_value, grade_value);
    }
};

int main() {
    Builder builder;
    auto person = builder.name("Ivan").age(25).grade(10).get();
    
    assert(person.person_name == "Ivan");
    assert(person.person_age == 25);
    assert(person.person_grade == 10);
    
    auto person2 = Builder().name("Anna").age(30).grade(15).get();
    assert(person2.person_name == "Anna");
    assert(person2.person_age == 30);
    assert(person2.person_grade == 15);
    
    auto person3 = Builder().name("Peter").age(20).grade(5).get();
    assert(person3.person_name == "Peter");
    assert(person3.person_age == 20);
    assert(person3.person_grade == 5);
    
    return 0;
}