#include <iostream>
#include <stack>
using std::cin;
using std::cout;
using std::stack;


template<typename T, class TLess = std::less<T>>
class SearchTree {
public:
    SearchTree() : root(nullptr) {}
    ~SearchTree() { deleteHelper(root); }

    SearchTree(const SearchTree&) = delete;
    SearchTree(SearchTree&&) = delete;
    SearchTree& operator=(const SearchTree&) = delete;
    SearchTree& operator=(SearchTree&&) = delete;


    void insert(const T& Key, const TLess& isLess = TLess());

    template <class Visit>
    void DFSWithStack(Visit visit) {
        stack<Node*> NewStack;
        Node* tempNode = root;
        while (!NewStack.empty() || tempNode != nullptr) {
            if (tempNode != nullptr) {
                NewStack.push(tempNode);
                tempNode = tempNode->left;
            }
            else {
                visit(NewStack.top()->key);
                tempNode = NewStack.top()->right;
                NewStack.pop();
            }
        }
    }
private:

    struct Node {
        T key;
        Node* left;
        Node* right;
        explicit Node(const T& key) : left(nullptr), right(nullptr), key(key) {}
    };

    Node* root = nullptr;
    void deleteHelper(Node*& temp);

};

template <typename T, class TLess>
void SearchTree<T, TLess>::insert(const T &Key, const TLess& isLess) {
    Node* newNode = new Node(Key);

    Node* temp = root;
    if (root == nullptr) {
        root =  newNode;
        return;
    }
    while (temp != nullptr) {
        if (isLess(temp->key, Key)) {
            if (temp->right != nullptr)
                temp = temp->right;
            else {
                temp->right = newNode;
                break;
            }
        }
        else {
            if (temp->left != nullptr)
                temp = temp->left;
            else {
                temp->left = newNode;
                break;
            }
        }
    }
}

template <typename T, class TLess>
void SearchTree<T, TLess>::deleteHelper(Node*& temp) {
    if(temp == nullptr)
        return;
    deleteHelper(temp->left);
    deleteHelper(temp->right);
    delete temp;
}


int main() {
    int n, temp;
    cin >> n;
    SearchTree<int> myTree;
    for (int i = 0; i < n; ++i) {
        cin >> temp;
        myTree.insert(temp);
    }

    myTree.DFSWithStack([](const int& temp_key) { cout << temp_key << " ";});
    return 0;
}






