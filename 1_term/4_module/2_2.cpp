#include <iostream>
#include <algorithm>
#include <vector>
using std::cin;
using std::cout;
using std::vector;

struct Time
{
    int timeIn;
    int timeOut;
    Time(int timeIn, int timeOut) : timeIn(timeIn), timeOut(timeOut) {};
    Time() = default;
};


template <typename T, class TLess = std::less<T>>
void mergeSort(vector<T>& array, int left, int right, const TLess& isLess = TLess()) {
    if (left + 1 >= right)
        return;
    int mid = (left + right) / 2;
    mergeSort(array, left, mid, isLess);
    mergeSort(array, mid, right, isLess);
    merge(array, left, mid, right, isLess);
}

template <typename T, class TLess = std::less<T>>
void merge(vector<T>&  array, int left_start, int mid, int right_end, const TLess& isLess= TLess())
{
    T* new_array = new T[right_end - left_start];
    int ind_left = 0, ind_right = 0;

    while(left_start + ind_left < mid && mid + ind_right < right_end)
        if(isLess(array[left_start + ind_left], array[mid + ind_right])) {
            new_array[ind_left + ind_right] = array[left_start + ind_left];
            ind_left ++;
        }
        else {
            new_array[ind_left + ind_right] = array[mid + ind_right];
            ind_right++;
        }

    while (mid + ind_right < right_end) {
        new_array[ind_left + ind_right] = array[mid + ind_right];
        ++ind_right;
    }

    while (left_start + ind_left < mid) {
        new_array[ind_right + ind_left] = array[left_start + ind_left];
        ++ind_left;
    }

    for (int i = 0; i < ind_right + ind_left; ++i)
        array[left_start + i] = new_array[i];

    delete[] new_array;
}

int main() {
    vector<Time> timeTable;
    Time temp;

    while(cin >> temp.timeIn >> temp.timeOut)
        timeTable.push_back(temp);

    mergeSort<Time>(timeTable, 0, timeTable.size(), [](const Time& l, const Time& r){ return l.timeOut < r.timeOut; });

    int answer = 1;
    int lastTime = timeTable[0].timeOut;
    for (const auto& another : timeTable) {
        if (another.timeIn >= lastTime) {
            ++answer;
            lastTime = another.timeOut;
        }
    }

    cout << answer;
    return 0;
}

