#include <cassert>
#include <concepts>
#include <vector>

template <typename T>
concept HasPushBack = requires(T container, typename T::value_type value) {
    container.push_back(value);
};

template <typename Container, typename Arg>
void handle(Container& container, Arg arg) {
    if constexpr (std::same_as<Arg, int>) {
        container.push_back(arg);
    }
}

template <typename Container, typename... Args>
void insert(Container& container, Args... args) {
    static_assert(HasPushBack<Container>, "Container must have push_back method");
    (handle(container, args), ...);
}

int main() {
    std::vector<int> vec;
    
    insert(vec, 1, 2, 3);
    assert(vec.size() == 3);
    assert(vec[0] == 1);
    assert(vec[1] == 2);
    assert(vec[2] == 3);
    
    std::vector<int> vec2;
    insert(vec2, 1, 3.14, 'a', 2, "hello", 3, 4.5);
    assert(vec2.size() == 3);
    assert(vec2[0] == 1);
    assert(vec2[1] == 2);
    assert(vec2[2] == 3);
    
    std::vector<int> vec3;
    insert(vec3);
    assert(vec3.empty());
    
    std::vector<int> vec4;
    insert(vec4, 10, 20, 30, 40, 50);
    assert(vec4.size() == 5);
    assert(vec4[0] == 10);
    assert(vec4[1] == 20);
    assert(vec4[2] == 30);
    assert(vec4[3] == 40);
    assert(vec4[4] == 50);
    
    std::vector<int> vec5;
    insert(vec5, 1.5, 2.7, 3.9);
    assert(vec5.empty());
    
    return 0;
}