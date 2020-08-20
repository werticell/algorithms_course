#include<iostream>
using std::cin;
using std::cout;


/* Куча на максимум
 * public: add, pop , push , push, top, size;
 * private: siftDown, siftUp, buildHeap
*/
class Heap{
public:
    Heap () : array(new int[128]), array_size(0), max_size(8) {}
    ~Heap() { delete [] array; }

    Heap(const Heap&) = delete;
    Heap(Heap&&) = delete;
    Heap& operator=(const Heap&) = delete;
    Heap& operator=(Heap&&) = delete;

    void add(int value); // добавление элементов в конец реально массива для быстрого построения кучи
    void pop();
    bool empty() { return array_size == 0; }
    void push(int value);
    int top() { return array[0]; }
    int size() { return array_size; }

    void buildHeap();
private:
    int* array;
    int max_size;
    int array_size;

    void enlarge();
    void siftDown(int ind);
    void siftUp(int ind);
};

void Heap::add(int value) {
    if( array_size == max_size )
        enlarge();
    array[array_size] = value;
    ++array_size;
}

void Heap::pop() {
    std::swap(array[0], array[array_size - 1]);
    --array_size;
    siftDown(0); // меняю местами наибольший элемент и последний  и вызываю просеивание от нового верхнего
}

void Heap::enlarge() {
    int* new_array = new int[max_size * 2];
    for(int i = 0; i < max_size; ++i)
        new_array[i] = array[i];

    max_size *= 2;
    delete[] array;
    array = new_array;
}

void Heap::push(int value) {
    if(array_size == max_size)
        enlarge();

    array[array_size] = value;
    siftUp( array_size);
    ++array_size;
}

void Heap::buildHeap() {
    for(int i = array_size / 2; i >= 0; --i)
        siftDown(i);
}


void Heap::siftDown(int ind) {
    int left_child = 0, right_child = 0, temp_ind = 0;
    while (2 * ind + 1 < array_size) {
        left_child = 2 * ind + 1;
        right_child = 2 * ind + 2;
        temp_ind = left_child;
        if (right_child < array_size && array[right_child] > array[left_child])
            temp_ind = right_child;
        if (array[ind] >= array[temp_ind])
            break;
        else
            std::swap( array[ind], array[temp_ind] );
        ind = temp_ind;
    }
}

void Heap::siftUp(int ind) {
    while (array[ind] > array[(ind - 1) / 2]) {
        std::swap(array[ind], array[(ind - 1) / 2]);
        ind = (ind - 1) / 2;
    }
}




class Vector {
public:
    Vector() : array(new int[8]), array_size(0), max_size(8) {}
    ~Vector() { delete []array; }

    Vector(const Vector&) = delete;
    Vector(Vector&&) = delete;
    Vector& operator=(const Vector&) = delete;
    Vector& operator=(Vector&&) = delete;

    void push(int value);
    int size() { return array_size; }
    void pop();
    int operator[](int ind) { return array[ind]; }

private:
    int* array;
    int max_size;
    int array_size;
    void enlarge();
    void reduce();
};

void Vector::push(int value) {
    if(array_size == max_size)
        enlarge();
    array[array_size] = value;
    ++array_size;
}

void Vector::pop() {
    --array_size;
    if(array_size < max_size / 4)
        reduce();
}


void Vector::enlarge() {
    int* new_array = new int[max_size * 2];
    for(int i =0; i < max_size; ++i)
        new_array[i] = array[i];

    max_size *= 2;
    delete[] array;
    array = new_array;
}

void Vector::reduce() {
    int* new_array = new int[max_size / 2];
    for(int i =0; i < max_size / 4; ++i)
        new_array[i] = array[i];

    max_size /= 2;
    delete[] array;
    array = new_array;
}


int get_answer(Heap& heap, int k) {
    int steps = 0;
    Vector vector;
    while(heap.size() != 0) {
        int weight_of_step = 0;
        while (weight_of_step <= k && heap.size() != 0)
        {
            int max_weight = heap.top();
            if( weight_of_step + max_weight > k )
                break;
            if( max_weight != 1)
                vector.push(max_weight);
            heap.pop();
            weight_of_step += max_weight;
        }
        for (int i = vector.size() - 1 ; i >= 0; --i) {
            heap.push( vector[i] / 2 );
            vector.pop();
        }
        ++steps;
    }
    return steps;
}



int main() {
    int n = 0;
    cin >> n;
    Heap heap;
    int value = 0;
    for (int i = 0; i < n; ++i) { // вводим элементы кучи в массив не строя кучу
        cin >> value;
        heap.add(value);
    }

    int k = 0;
    cin >> k;
    heap.buildHeap();

    cout << get_answer(heap, k);
    return 0;

}
