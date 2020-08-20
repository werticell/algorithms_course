#include <iostream>
using std::cin;
using std::cout;

struct Node {
    int key;
    Node* left;
    Node* right;
    Node(int key) : left(nullptr), right(nullptr), key(key) {}
};

struct TNode {
    int key;
    int priority;
    TNode* left;
    TNode* right;
    TNode(int key, int priority) : left(nullptr), right(nullptr), key(key), priority(priority) {}
};

class Treap {
private:
    TNode* root;
    int maxHeight = 0;
public:
    Treap() : root(nullptr) {}
    ~Treap() { deleteHelper(root); }

    Treap(const Treap&) = delete;
    Treap(Treap&&) = delete;
    Treap& operator=(const Treap&) = delete;
    Treap& operator=(Treap&&) = delete;

    void insert(int key, int priority) {
        auto new_node = new TNode(key, priority);

        if (root == nullptr) {
            root = new_node;
            return;
        }

        TNode* temp_node = root;
        TNode* temp_node_father = nullptr; // предпоследняя вершина которая не участвует в split

        while (temp_node != nullptr && temp_node->priority >= priority) {  // поиск элемента приоритет которого меньше чем у нового
            temp_node_father = temp_node;
            temp_node = (key >= temp_node->key) ? temp_node->right : temp_node->left;
        }


        TNode* left = nullptr;
        TNode* right = nullptr;
        split(temp_node, key, left, right); // split по найденной вершине

        if(temp_node_father == nullptr)
            root = new_node;
        else {
            if (key >= temp_node_father->key)
                temp_node_father->right = new_node;
            else
                temp_node_father->left = new_node;
        }

        new_node->left = left;
        new_node->right = right;
    }



    int height() {
        int height = 0;
        heightCountHelper(root, height);
        return maxHeight;
    }


private:

    TNode* merge(TNode* left, TNode* right) {
        if(left == nullptr) return right;
        if(right == nullptr) return left;

        if (right->priority >= left->priority) {
            right->left = merge(left, right->left);
            return right;
        }
        else {
            left->right = merge(left->right, right);
            return left;
        }
    }

    void split(TNode* currentRoot, int Key, TNode*& left, TNode*& right) {
        if(currentRoot == nullptr) {
            right = nullptr;
            left = nullptr;
        }
        else {
            if (Key >= currentRoot->key) {
                split(currentRoot->right, Key, currentRoot->right, right);
                left = currentRoot;
            }
            else {
                split(currentRoot->left, Key, left, currentRoot->left);
                right = currentRoot;
            }
        }
    }

    void deleteHelper(TNode* temp) {
        if (temp == nullptr)
            return;
        deleteHelper(temp->left);
        deleteHelper(temp->right);
        delete temp;
    }
    void heightCountHelper(TNode* node, int height) {
        if (node == nullptr)
            return;
        if (height > maxHeight)
            maxHeight = height;
        if (node->right != nullptr)
            heightCountHelper(node->right, height + 1);
        if (node->left != nullptr)
            heightCountHelper(node->left, height + 1);
    }
};

class SearchTree {
public:
    SearchTree() = default;
    ~SearchTree() { deleteHelper(root); }

    SearchTree(const SearchTree&) = delete;
    SearchTree(SearchTree&&) = delete;
    SearchTree& operator=(const SearchTree&) = delete;
    SearchTree& operator=(SearchTree&&) = delete;


    void insert(int Key) {
        Node* new_node = new Node(Key);
        Node* temp = root;

        if (root == nullptr) {
            root =  new_node;
            return;
        }

        while (temp != nullptr) {
            if (Key >= temp->key) {
                if(temp->right != nullptr)
                    temp = temp->right;
                else {
                    temp->right = new_node;
                    break;
                }
            }
            else {
                if (temp->left != nullptr)
                    temp = temp->left;
                else {
                    temp->left = new_node;
                    break;
                }
            }
        }

    }


    int height() {
        int height = 0;
        heightCountHelper(root, height);
        return maxHeight;
    }
private:
    int maxHeight = 0;
    Node* root = nullptr;
    void deleteHelper(Node* temp) {
        if (temp == nullptr)
            return;
        deleteHelper(temp->left);
        deleteHelper(temp->right);
        delete temp;
    }
    void heightCountHelper(Node* node, int height) {
        if (node == nullptr)
            return;
        if (height > maxHeight)
            maxHeight = height;
        if (node->right != nullptr)
            heightCountHelper(node->right, height + 1);
        if (node->left != nullptr)
            heightCountHelper(node->left, height + 1);
    }
};

int main() {
    int n;
    cin >> n;
    SearchTree mySTree;
    Treap myTreap;
    int x = 0, y = 0;
    for (int i = 0; i < n; ++i) {
        cin >> x >> y;
        mySTree.insert(x);
        myTreap.insert(x, y);
    }
    cout << abs(mySTree.height() - myTreap.height());

    return 0;
}





