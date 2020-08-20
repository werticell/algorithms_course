#include <iostream>
#include <string>
using std::cin;
using std::cout;
using std::string;

class Compare {
public:
    explicit Compare(int rank) : rank(rank) {};

    unsigned int operator()(unsigned int number) {
        return ((number >> rank * 8u) & 255u);
    }
private:
    int rank;
};

// структура для того чтобы удобно находить минимум и максимум ключей элементов в массиве
struct Extremum{
    int min;
    int max;
    Extremum(int min, int max) : min(min), max(max) {}
};

// функция возвращающая минимум и максимум ключей элементов на массиве
template <typename T, class TGetKey>
Extremum getExtremum(T* array, int size, TGetKey getKey)
{
    int max = getKey(array[0]);
    int min = getKey(array[0]);
    for (int i = 0; i < size; ++i) {
        int key = getKey(array[i]);
        if (key > max)
            max = key;
        if (key < min)
            min = key;
    }
    return {min, max};
}

template <typename T, class TGetKey>
void countingSort(T* array, int size, TGetKey getKey)  // где к количество групп элементов // int r,
{
    // значения ключей элементов в границах от 0  до bins.Max - bins.Min
    Extremum bins = getExtremum(array, size, getKey);
    int countBins = bins.max - bins.min + 1; // диапазон значений ключей сортируемых элементов

    int* ArrayOfKeys = new int [countBins]; // массив групп элементов по их ключам

    // занулили элементы массива которые овтечают за количество элементов с ключами данного значения
    for (int i = 0; i < countBins; ++i)
        ArrayOfKeys[i] = 0;

    for (int i = 0; i < size; ++i) // подсчитали количество элементов по равным ключам
        ++ArrayOfKeys[getKey( array[i]) - bins.min];

    for (int i = 1; i < countBins; ++i)
        ArrayOfKeys[i] += ArrayOfKeys[i - 1];   // посчитали концы групп


    T* answer = new T [size];  // массив для формирования ответа

    for (int i = size - 1; i >= 0; --i)
        answer[--ArrayOfKeys[getKey(array[i]) - bins.min]] = array[i];  // должны скастить ключ до его положения в массиве  - bins.Min

    for (int i = 0; i < size; ++i)
        array[i] = answer[i];

    delete[] ArrayOfKeys;
    delete[] answer;

}

template<typename T>
void LSDSort(T* array, const int size) {
    for (int rank = 0; rank < sizeof(T); ++rank)
        countingSort( array, size, Compare(rank));
}

int main()
{
    int n = 0;
    cin >> n;
    auto* array = new long long [n];
    for (int i = 0; i < n; ++i)
        cin >> array[i];

    LSDSort(array, n);

    for (int i = 0; i < n; ++i)
        cout << array[i] << " ";

    delete[] array;
    return 0;
}

