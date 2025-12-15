#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <utility>
#include <cassert>

class Vector {
private:
    int* m_array = nullptr;
    std::size_t m_capacity = 0;
    std::size_t m_size = 0;

    void resize() {
        if (m_size == m_capacity) {
            std::size_t new_capacity = m_capacity == 0 ? 1 : m_capacity * 2;
            int* new_array = new int[new_capacity]{};
            
            if (m_array) {
                std::copy(m_array, m_array + m_size, new_array);
                delete[] m_array;
            }
            
            m_array = new_array;
            m_capacity = new_capacity;
        }
    }

public:
    Vector() = default;

    Vector(std::initializer_list<int> list) : m_capacity(list.size()), m_size(list.size()) {
        m_array = m_capacity ? new int[m_capacity]{} : nullptr;
        std::copy(list.begin(), list.end(), m_array);
    }

    Vector(const Vector& other) : m_capacity(other.m_size), m_size(other.m_size) {
        m_array = m_capacity ? new int[m_capacity]{} : nullptr;
        std::copy(other.m_array, other.m_array + other.m_size, m_array);
    }

    Vector(Vector&& other) 
        : m_array(std::exchange(other.m_array, nullptr))
        , m_capacity(std::exchange(other.m_capacity, 0))
        , m_size(std::exchange(other.m_size, 0)) {}

    ~Vector() {
        delete[] m_array;
    }

    Vector& operator=(Vector other) {
        swap(other);
        return *this;
    }

    void swap(Vector& other) {
        std::swap(m_array, other.m_array);
        std::swap(m_capacity, other.m_capacity);
        std::swap(m_size, other.m_size);
    }

    void push_back(int value) {
        resize();
        m_array[m_size++] = value;
    }

    void clear() {
        m_size = 0;
    }

    bool empty() const {
        return m_size == 0;
    }

    std::size_t capacity() const {
        return m_capacity;
    }

    std::size_t size() const {
        return m_size;
    }
};

void swap(Vector& lhs, Vector& rhs) {
    lhs.swap(rhs);
}

int main() {
    Vector vector_1;
    assert(vector_1.empty());
    assert(vector_1.capacity() == 0);
    assert(vector_1.size() == 0);

    vector_1.push_back(1);
    assert(!vector_1.empty());
    assert(vector_1.capacity() == 1);
    assert(vector_1.size() == 1);

    vector_1.push_back(2);
    assert(vector_1.capacity() == 2);
    assert(vector_1.size() == 2);

    vector_1.push_back(3);
    assert(vector_1.capacity() == 4);
    assert(vector_1.size() == 3);

    vector_1.clear();
    assert(vector_1.empty());
    assert(vector_1.capacity() == 4);
    assert(vector_1.size() == 0);

    Vector vector_2 = {1, 2, 3};
    assert(vector_2.capacity() == 3);
    assert(vector_2.size() == 3);

    Vector vector_3 = vector_2;
    assert(vector_3.capacity() == 3);
    assert(vector_3.size() == 3);

    Vector vector_4 = std::move(vector_3);
    assert(vector_4.capacity() == 3);
    assert(vector_4.size() == 3);
    assert(vector_3.capacity() == 0);
    assert(vector_3.size() == 0);

    return 0;
}