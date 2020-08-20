#include <iostream>
#include <vector>
using std::cin;
using std::cout;
using std::vector;

struct Node {
    int key;
    int height;
    int count;
    Node* left;
    Node* right;
    explicit Node(int key) : key(key), left(nullptr), right(nullptr), height(1), count(1) {}
};

class AVLTree {
private:
    Node* root;
public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { deleteHelper(root); }

    AVLTree(const AVLTree&) = delete;
    AVLTree(AVLTree&&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;
    AVLTree& operator=(AVLTree&&) = delete;


    int getHeight(Node* node) const {
        return (node == nullptr) ? 0 : node->height;
    }

    int getCount(Node* node) const {
        return (node == nullptr) ? 0 : node->count;
    }


    // восстанавливает правильное значение для полей height и count
    void fixHeight(Node* node) {
        int rightHeight = getHeight(node-> right);
        int leftHeight  = getHeight(node->left);
        node->count = getCount(node->right) + getCount(node->left) + 1;
        node->height = std::max(rightHeight, leftHeight) + 1;

    }

    // возвращает разность высот поддеревьев для данной вершины
    int getDifference(Node* node) {
        return getHeight(node->right) - getHeight(node->left);
    }

    // малое вращение вправо
    Node* rotateRight(Node* node) {
        Node* left_son = node->left;
        node->left = left_son->right;
        left_son->right = node;
        fixHeight(node);
        fixHeight(left_son);
        return left_son;
    }

    // малое левое вращение
    Node* rotateLeft(Node* node) {
        Node* right_son = node->right;
        node->right = right_son->left;
        right_son->left = node;
        fixHeight(node);
        fixHeight(right_son);
        return right_son;
    }

    // балансировка узла
    Node* fixBalance(Node* node) {
        fixHeight(node);
        if (getDifference(node) == -2) {
            if (getDifference(node->left) > 0)
                node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (getDifference(node) == 2) {
            if (getDifference(node->right) < 0)
                node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }


    void insert(int key) {
        root = insertHelper(root, key);
    }



    static Node* findMin(Node* node) {
        while (node->left != nullptr)
            node = node->left;
        return node;

    }

    static Node* findMax(Node* node) {
        while (node->right != nullptr)
            node = node->right;
        return node;
    }


    Node* removeMin(Node* node) {
        if(node->left == nullptr)
            return node->right;
        node->left = removeMin(node->left);
        return fixBalance(node);
    }

    Node* removeMax(Node* node) {
        if(node->right == nullptr)
            return node->left;
        node->right = removeMax(node->right);
        return fixBalance(node);
    }

    void remove(int key) {
        if(root != nullptr)
            root = removeHelper(root, key);
    }

    int findOrderStat(size_t pos) {
        Node* node = root;
        while (getCount(node->left)  != pos) {
            if (pos > getCount(node->left)) {
                pos -= getCount(node->left) + 1;
                node = node->right;
            }
            else
                node = node->left;
        }
        return node->key;
    }



private:
    static void deleteHelper(Node* node) {
        if (node == nullptr)
            return;
        deleteHelper(node->left);
        deleteHelper(node->right);
        delete node;
    }

    Node* removeHelper(Node* node, int key) {
        if (node == nullptr)
            return nullptr;

        if (key > node->key)
            node->right = removeHelper(node->right, key);
        else{
            if (key < node->key)
                node->left = removeHelper(node->left, key);
            else{  // оказались здесь если нашли тот узел который нужно удалить
                // запоминаем двух детей найденной вершины
                Node* right_son = node->right;
                Node* left_son = node->left;
                delete node;

                if(right_son == nullptr ) // если правого или левого дерева у нашей вершины нет
                    return left_son;
                if(left_son == nullptr )
                    return right_son;

                Node* tmpNode;
                if (getHeight(right_son) >= getHeight(left_son)) {
                    tmpNode = findMin(right_son);
                    tmpNode->right = removeMin(right_son);
                    tmpNode->left = left_son;
                }
                else {
                    tmpNode = findMax(left_son);
                    tmpNode->left = removeMax(left_son);
                    tmpNode->right = right_son;
                }
                return fixBalance(tmpNode);
            }
        }
        return fixBalance(node);
    }

    Node* insertHelper(Node* node, int key) {
        if(node == nullptr) {
            Node* newNode = new Node(key);
            return newNode;
        }
        if(key >= node->key)
            node->right = insertHelper(node->right, key);
        else
            node->left = insertHelper(node->left, key);
        return fixBalance(node);
    }

    int findOrderHelper(Node* node, size_t pos) {
        if(getCount(node->left) == pos)
            return node->key;
        else if(pos > getCount(node->left)) {
            return findOrderHelper(node->right, pos - getCount(node->left) - 1);

        }
        else
            return findOrderHelper(node->left, pos);
    }

};


int main() {
    int n;
    cin >> n;
    AVLTree myTree;
    vector<int> answer;

    int a = 0, k = 0;
    for (int i = 0; i < n; ++i) {
        cin >> a >> k;
        if(a > 0)
            myTree.insert(a);
        else
            myTree.remove(-a);
        answer.push_back(myTree.findOrderStat(k));
    }

    for (int i = 0; i < n; ++i)
        cout << answer[i] << std::endl;
    return 0;
}

