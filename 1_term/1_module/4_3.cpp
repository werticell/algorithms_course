#include <iostream>
#include <cassert>
using std::cin;
using std::cout;

class Stack{
public:

    Stack () : array(new int[1]), array_size(0), max_size(1) {}
    ~Stack() { delete []array; }

    Stack(const Stack&) = delete;
    Stack(Stack&&) = delete;
    Stack& operator=(const Stack&) = delete;
    Stack& operator=(Stack&&) = delete;

    void push(int value);
    void pop();
    bool empty();
    int top();

private:
    int* array;
    int max_size;
    int array_size;

    void enlarge();
    void reduce();
};

void Stack::push(int value) {
    if(array_size == max_size)
        enlarge();
    array[array_size] = value;
    ++array_size;
}

void Stack::pop() {
    assert(array_size >= 1);
    --array_size;
    if(array_size < max_size / 4)
        reduce();
}

bool Stack::empty() {
    return array_size == 0;
}

int Stack::top() {
    return array[array_size - 1];
}

void Stack::enlarge() {
    int* new_array = new int[max_size * 2];
    for(int i = 0; i < max_size; ++i)
        new_array[i] = array[i];

    max_size *= 2;
    delete[] array;
    array = new_array;
}

void Stack::reduce() {
    int* new_array = new int[max_size / 2];
    for(int i = 0; i < max_size / 4; ++i)
        new_array[i] = array[i];

    max_size /= 2;
    delete[] array;
    array = new_array;
}





class Queue{
public:
    Queue () : queue_size(0) {}
    Queue(const Queue&) = delete;
    Queue(Queue&&) = delete;
    Queue& operator=(const Queue&) = delete;
    Queue& operator=(Queue&&) = delete;

    void push_back(int value);
    int pop_front();

private:
    Stack stack_in;
    Stack stack_out;
    int queue_size;
};

void Queue::push_back(int value) {
    stack_in.push(value);
    ++queue_size;
}

int Queue::pop_front() {
    if(queue_size == 0)
        return -1;
    else {
        if(stack_out.empty())
            while(!stack_in.empty()) {
                stack_out.push(stack_in.top());
                stack_in.pop();
            }
        int tmp = stack_out.top();
        stack_out.pop();
        --queue_size;
        return tmp;
    }
}
void solve_request(int type, int value, bool& answer, Queue& queue) {
    if(type == 3)
        queue.push_back(value);
    else
    if(queue.pop_front() != value)
        answer = false;
}

int main() {
    Queue queue;
    int n = 1;
    cin >> n;
    int type = 0, value = 0;
    bool answer = true;
    for(int i = 0; i < n; ++i) {
        cin >> type >> value;
        solve_request(type, value, answer, queue);
    }
    cout << (answer ? "YES" : "NO");

    return 0;
}

