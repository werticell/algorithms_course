#include <iostream>
using std::cin;
using std::cout;

struct Dot {   // type = 1 - начало отрезка, 2 - конец отрезка
    int x;
    int type;
};

bool compareByX(const Dot& r, const Dot& l) {
    return r.x <= l.x;
}

template <typename T, class TLess = std::less<T>>
void heapSort(T* array, int size, const TLess& isLess = TLess())
{
    while (size != 0) {
        std::swap(array[0], array[--size]);
        siftDown(array, 0, size, compareByX);
    }
}

template <typename T>
void buildHeap(T* array, int size) {
    for(int i = size / 2; i >= 0; --i)
        siftDown(array, i, size,compareByX);

}

template <typename T, class TLess = std::less<T>>
void siftDown(T* array, int ind ,int size, const TLess& isLess = TLess())
{
    int left_child_ind = 0, right_child_ind = 0, temp_ind = 0;
    while (2 * ind + 1 < size) {
        left_child_ind = 2 * ind + 1;
        right_child_ind = 2 * ind + 2;
        temp_ind = left_child_ind; // выясняем кто из детей больше
        if (right_child_ind < size && isLess(array[left_child_ind], array[right_child_ind]))
            temp_ind = right_child_ind;
        if (isLess(array[temp_ind],array[ind]))
            break;
        else
            std::swap(array[ind], array[temp_ind]);
        ind = temp_ind;
    }

}
template <typename T>
int countLength(T* array, int size)
{
    int length = 0, colours = 0;
    for (int i = 0; i < size; ++i) {
        if (colours == 1)
            length += array[i].x - array[i - 1].x;

        (array[i].type == 1) ? ++colours : --colours;
    }
    return length;
}


int main()
{
    int n = 0;
    cin >> n;
    Dot* array = new Dot [n * 2];  // массив точек с типами

    for (int i = 0; i < n; ++i) {
        cin >> array[i * 2].x >> array[i * 2 + 1].x;
        array[i * 2].type = 1;
        array[i * 2 + 1].type = 2;
    }

    n *= 2; // точек в два раза больше
    buildHeap(array, n);
    heapSort(array, n, compareByX);
    cout << countLength(array, n);
    delete[] array;
    return 0;
}

