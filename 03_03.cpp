#include <iostream>
#include <cassert>

class List {
private:
    struct Node {
        int value;
        Node* next;
        
        Node(int val) : value(val), next(nullptr) {}
    };
    
    Node* head;
    Node* tail;

public:
    List() : head(nullptr), tail(nullptr) {}
    
    ~List() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    bool empty() const {
        return head == nullptr;
    }
    
    void push_front(int value) {
        Node* new_node = new Node(value);
        if (empty()) {
            head = tail = new_node;
        } else {
            new_node->next = head;
            head = new_node;
        }
    }
    
    void push_back(int value) {
        Node* new_node = new Node(value);
        if (empty()) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }
    
    void pop_front() {
        if (empty()) return;
        
        Node* temp = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        delete temp;
    }
    
    void pop_back() {
        if (empty()) return;
        
        if (head == tail) {
            delete head;
            head = tail = nullptr;
            return;
        }
        
        Node* current = head;
        while (current->next != tail) {
            current = current->next;
        }
        
        delete tail;
        tail = current;
        tail->next = nullptr;
    }
    
    void show() const {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
    
    int get() const {
        if (empty()) {
            return 0;
        }
        
        Node* slow = head;
        Node* fast = head;
        
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        return slow->value;
    }
};

int main() {
    //Пустой список
    List list_1;
    assert(list_1.empty());
    assert(list_1.get() == 0);
    
    // в начало
    List list_2;
    list_2.push_front(1);
    list_2.push_front(2);
    list_2.push_front(3);
    assert(!list_2.empty());
    assert(list_2.get() == 2);
    
    // Добавление в конец
    List list_3;
    list_3.push_back(1);
    list_3.push_back(2);
    list_3.push_back(3);
    assert(!list_3.empty());
    assert(list_3.get() == 2);
    
    //Смешанное добавление
    List list_4;
    list_4.push_front(2);
    list_4.push_back(3);
    list_4.push_front(1);
    list_4.push_back(4);
    assert(list_4.get() == 3);
    
    //Удаление из начала
    List list_5;
    list_5.push_back(1);
    list_5.push_back(2);
    list_5.push_back(3);
    list_5.pop_front();
    assert(list_5.get() == 3);
    
    //Удаление из конца
    List list_6;
    list_6.push_back(1);
    list_6.push_back(2);
    list_6.push_back(3);
    list_6.pop_back();
    assert(list_6.get() == 2);
    
    //Один элемент
    List list_7;
    list_7.push_front(42);
    assert(list_7.get() == 42);
    list_7.pop_front();
    assert(list_7.empty());
    
    //Два элемента
    List list_8;
    list_8.push_back(10);
    list_8.push_back(20);
    assert(list_8.get() == 20);
    
    //Четыре элемента
    List list_9;
    list_9.push_back(1);
    list_9.push_back(2);
    list_9.push_back(3);
    list_9.push_back(4);
    assert(list_9.get() == 3);
    //что-то
    List list_10;
    list_10.push_front(3);
    list_10.push_back(4);
    list_10.push_front(2);
    list_10.push_back(5);
    list_10.push_front(1);
    assert(list_10.get() == 3);
    list_10.show();
    list_10.pop_front();
    list_10.pop_back();
    assert(list_10.get() == 3);
    list_10.pop_front();
    list_10.pop_back();
    list_10.pop_front();
    assert(list_10.empty());
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}