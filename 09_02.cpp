#include <memory>
#include <queue>
#include <stack>
#include <iostream>

class Tree
{
public:
    struct Node
    {
        int value;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::weak_ptr<Node> parent;

        Node(int val) : value(val), left(nullptr), right(nullptr) {}
        
        ~Node()
        {
            std::cout << "Node::~Node with value = " << value << "\n";
        }
    };

    std::shared_ptr<Node> root;

    Tree() : root(nullptr) {}

    void traverse_v1() const
    {
        if (!root) return;
        
        std::queue<std::shared_ptr<Node>> queue;
        queue.push(root);
        
        while (!queue.empty())
        {
            auto node = queue.front();
            queue.pop();
            
            std::cout << node->value << " ";
            
            if (node->left) queue.push(node->left);
            if (node->right) queue.push(node->right);
        }
        std::cout << "\n";
    }

    void traverse_v2() const
    {
        if (!root) return;
        
        std::stack<std::shared_ptr<Node>> stack;
        stack.push(root);
        
        while (!stack.empty())
        {
            auto node = stack.top();
            stack.pop();
            
            std::cout << node->value << " ";
            
            if (node->right) stack.push(node->right);
            if (node->left) stack.push(node->left);
        }
        std::cout << "\n";
    }
};

int main()
{
    Tree tree;
    
    tree.root = std::make_shared<Tree::Node>(1);
    
    tree.root->left = std::make_shared<Tree::Node>(2);
    tree.root->left->parent = tree.root;
    
    tree.root->right = std::make_shared<Tree::Node>(3);
    tree.root->right->parent = tree.root;
    
    tree.root->left->left = std::make_shared<Tree::Node>(4);
    tree.root->left->left->parent = tree.root->left;
    
    tree.root->left->right = std::make_shared<Tree::Node>(5);
    tree.root->left->right->parent = tree.root->left;
    
    tree.root->right->left = std::make_shared<Tree::Node>(6);
    tree.root->right->left->parent = tree.root->right;
    
    tree.root->right->right = std::make_shared<Tree::Node>(7);
    tree.root->right->right->parent = tree.root->right;
    
    std::cout << "BFS traversal: ";
    tree.traverse_v1();
    
    std::cout << "DFS traversal: ";
    tree.traverse_v2();
    
    std::cout << "Tree destruction sequence:\n";
    
    return 0;
}