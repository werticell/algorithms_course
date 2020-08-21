/* Декартово дерево по неявному ключу. Операции:
 * 1) Вставка элемента на место pos
 * 2) Удаление элементов по диапазону индексов
 * 3) Получение элемента по pos
 */
#include <iostream>
#include <vector>
#include <algorithm>
using std::cout;
using std::cin;
using std::pair;
using std::vector;
using std::make_pair;


template<typename T>
struct Node{
    template <typename... Args>
    explicit Node(Args&&... data) : left_son(nullptr), right_son(nullptr), data(std::forward<Args...>(data...)), priority(rand()), size(1) {}
    T data;
    int priority;
    int size;
    Node* left_son;
    Node* right_son;

    void correctSize() {
        size = 1;
        if(right_son != nullptr)
            size += right_son->size;
        if(left_son != nullptr)
            size += left_son->size;
    }

};

template<typename T>
class Treap {
public:
    Treap() : root(nullptr) {};
    ~Treap() { DeleteHelper(root); }
    Treap(const Treap&) = delete;
    Treap(Treap&&) = delete;
    Treap& operator=(const Treap&) = delete;
    Treap& operator=(Treap&&) = delete;

    void InsertAt(int position, const T& value);
    void DeleteAt(int l, int r);
    T GetAt(int position);


private:
    Node<T>* root;

    pair<Node<T>*, Node<T>*> split(Node<T>* current_root, int position);
    Node<T>* merge(Node<T>* left, Node<T>* right);

    void DeleteHelper(Node<T>* current_node);

};


template<typename T>
void Treap<T>::DeleteHelper(Node<T> *current_node) {
    if(current_node == nullptr)
        return;
    DeleteHelper(current_node->left_son);
    DeleteHelper(current_node->right_son);
    delete current_node;
}


template<typename T>
Node<T>* Treap<T>::merge(Node<T>* left, Node<T>* right) {
    if(left == nullptr)
        return right;
    if(right == nullptr)
        return left;

    if(left->priority <= right->priority) {
        right->left_son = merge(left, right->left_son);
        right->correctSize();
        return right;
    }
    else {
        left->right_son = merge(left->right_son, right);
        left->correctSize();
        return left;
    }
}

template<typename T>
pair<Node<T>*, Node<T>*> Treap<T>::split(Node<T>* current_root, int position) {
    if(current_root == nullptr) {
        return make_pair(nullptr, nullptr);
    }

    int current_root_position = current_root->left_son == nullptr ? 0 : current_root->left_son->size;

    if(position <= current_root_position) {
        pair<Node<T>*, Node<T>*> result = split(current_root->left_son, position);
        current_root->left_son = result.second;
        current_root->correctSize();
        return make_pair(result.first, current_root);
    }
    else {
        pair<Node<T>*, Node<T>*> result = split(current_root->right_son, position - current_root_position - 1);
        current_root->right_son = result.first;
        current_root->correctSize();
        return make_pair(current_root, result.second);
    }
}


template<typename T>
void Treap<T>::InsertAt(int position, const T& value) {
    auto* new_node = new Node<T>(value);
    pair<Node<T>*, Node<T>*> result_of_split = split(root, position);
    root = merge(result_of_split.first, merge(new_node, result_of_split.second));


}

template<typename T>
void Treap<T>::DeleteAt(int l, int r) {

    pair<Node<T>*, Node<T>*> result_of_split_1 = split(root, r + 1);
    pair<Node<T>*, Node<T>*> result_of_split_2 = split(result_of_split_1.first, l);
    DeleteHelper(result_of_split_2.second);
    root = merge(result_of_split_2.first, result_of_split_1.second);
}

template<typename T>
T Treap<T>::GetAt(int position) {
    Node<T>* current_node = root;

    do {
        int current_node_position = current_node->left_son == nullptr ? 0 : current_node->left_son->size;
        if (current_node_position == position)
            break;
        if (position < current_node_position) {
            current_node = current_node->left_son;
        }
        else {
            position = position- current_node_position - 1;
            current_node = current_node->right_son;
        }
    } while (true);
    return current_node->data;

}




int main() {
    int k = 0;
    cin >> k;
    vector<std::string> answer;

    Treap<std::string> array;
    char command_type;
    for(int i  = 0; i < k; ++i) {
        cin >> command_type;
        int position = 0;
        switch(command_type) {
            case '+': {
                std::string data;
                cin >> position >> data;
                array.InsertAt(position, data);
                break;
            }
            case '-' : {
                int second_position = 0;
                cin >> position >> second_position;
                array.DeleteAt(position, second_position);
                break;
            }
            case '?' : {
                cin >> position;
                answer.push_back(array.GetAt(position));
                break;
            }
            default:
                continue;
        }
    }
    
    std::for_each(answer.begin(), answer.end(), [](const auto& x){ cout << x << std::endl;});
    return 0;
}


