#include <iostream>
#include <string>
using std::cin;
using std::cout;

template <typename T, class TLess = std::less<T>>
void merge(T* array, int left_start, int mid, int right_end, T* newArray, const TLess& isLess= TLess())
{
    int ind_left = 0, ind_right = 0;
    while (left_start + ind_left < mid && mid + ind_right < right_end) {
        
        if(isLess(array[left_start + ind_left], array[mid + ind_right])) {
            newArray[ind_left + ind_right] = array[left_start + ind_left];
            ++ind_left;
        }
        else {
            newArray[ind_left + ind_right] = array[mid + ind_right];
            ++ind_right;
        }
    }


    while (mid + ind_right < right_end) {
        newArray[ind_left + ind_right] = array[mid + ind_right];
        ++ind_right;
    }

    while (left_start + ind_left < mid) {
        newArray[ind_right + ind_left] = array[left_start + ind_left];
        ++ind_left;
    }

    for (int i = 0; i < ind_right + ind_left; ++i)
        array[left_start + i] = newArray[i];
}


template <typename T, class TLess = std::less<T>>
void mergeSort( T* array, int left, int right, T* arrayCopy, const TLess& isLess = TLess() ){
    if (left + 1 >= right)
        return;
    int mid = (left + right) / 2;
    mergeSort(array, left, mid, arrayCopy);
    mergeSort(array, mid, right, arrayCopy);
    merge(array, left, mid, right, arrayCopy);
}


int main()
{
    int n = 0, k = 0;
    cin >> n >> k;
    int* array = new int [k * 2];
    int* arrayCopy = new int[k * 2];

    for (int i = 0; i < std::min(k * 2, n); ++i)
        cin >> array[i];
    mergeSort(array, 0, k * 2, arrayCopy);
    n -= k * 2;
    
    
    while (n > 0){
        for (int j = k; j < std::min(k * 2, n + k); ++j)
            cin >> array[j];
        mergeSort(array, k, std::min(n + k, k * 2), arrayCopy);
        merge(array, 0, k, std::min(n + k, k * 2), arrayCopy);
        n -= k;
    }
    
    for (int i = 0; i < k; ++i)
        cout << array[i] << " ";
    
    delete[] array;
    delete[] arrayCopy;
    return 0;
}

