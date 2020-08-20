#include <iostream>
#include <string>
using std::cin;
using std::cout;
using std::string;

template <typename T>
struct IsLess {
    bool operator() (const T& l, const T& r) const {
        return l < r;
    }
};

struct Note{
    int Parameter[10];
    string Name;

};

template <typename T, class TLess = IsLess<T>>
void insertSort(T* array, int size, const TLess& isLess = TLess())
{
    for (int i = 1; i < size; ++i) {
        T tmp_value = array[i];
        int tmp_value_ind = i;
        for (int j = i - 1; j >= 0 && isLess(tmp_value, array[j]); --j) {
            array[j + 1] = array[j];
            tmp_value_ind = j;
        }
        array[tmp_value_ind] = tmp_value;
    }
}

class CompareByPriority {
public:

    CompareByPriority(const int* priority, int size) {
        priorityArray = new int [size];
        for(int i = 0; i < size; ++i)
            priorityArray[i] = priority[i];
    }

    ~CompareByPriority() { delete[] priorityArray; }

    bool operator () (const Note& r, const Note& l) const {
        int i = 0;
        while (priorityArray[i] != 0) {
            int position = priorityArray[i] - 1;
            if(l.Parameter[position] < r.Parameter[position])
                return true;
            if(l.Parameter[position] > r.Parameter[position])
                return false;
            ++i;
        }
        return false;
    }
private:
    int* priorityArray;
};

int main()
{
    int n = 0, k = 0;
    cin >> n >> k;
    int* priority = new int [k + 1]; // занулим последний элемент
    priority[k] = 0;
    Note* array = new Note [n];  // массив записей с полями имя и вектором из значений

    for (int i = 0; i < k; ++i) // ввод приоритета для значений
        cin >> priority[i];

    for (int i = 0; i < n; ++i) { // ввод  имемен и значений
        cin >> array[i].Name;
        for(int j = 0; j < k; ++j) {
            cin >> array[i].Parameter[j];
        }
    }

    CompareByPriority compare(priority, k + 1);
    insertSort(array, n, compare);

    for (int i = 0; i < n; ++i)
        cout << array[i].Name << std::endl;

    delete[] priority;
    delete[] array;
    return 0;
}



