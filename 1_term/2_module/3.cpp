#include <iostream>
#include <string>
using std::cin;
using std::cout;


template<typename T, class TLess = std::less<T>>
int median(int first, int second, int third, T* array, const TLess& isLess)
{
    if (isLess(array[second], array[first])) {
        if (isLess(array[first], array[third]))
            return first;
        return (isLess(array[third], array[second])) ? second : third;
    }
    if (isLess(array[second], array[third]))
        return second;
    return (isLess(array[third],array[first])) ? first : third;
}


template < typename T, class TLess = std::less<T>>
T findOrderStatistic(T* array, int size, int k, const TLess& isLess = TLess())
{
    int left = 0, right = size - 1;
    while (right - left >= 1) {
        int pivot = median(right, left, (right + left) / 2, array, isLess);

        //partition start
        int i = left, j = left;  // указатели в начало массива
        std::swap(array[pivot], array[right]); // pivot на последнем месте поэтому дальше сравнение идет с последним на отрезке
        while (j != right) {
            if (isLess(array[j], array[right])){
                if (i != j)
                    std::swap(array[i], array[j]);
                ++i;
            }
            ++j;
        }
        std::swap(array[i], array[right]);    // теперь пивот на месте i
        pivot = i;
        // partition end

        if (pivot  == k)
            return array[pivot];
        else {
            if (k > pivot)
                left = pivot + 1;
            else
                right = pivot - 1;
        }

    }
    return array[right];

}


int main()
{
    int n = 0, k = 0;
    cin >> n >> k;
    int* array = new int [n];

    for (int i = 0; i < n; ++i)
        cin >> array[i];

    cout << findOrderStatistic(array, n, k);
    delete[] array;
    return 0;
}

