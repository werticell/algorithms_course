#include <iostream>
using std::cin;
using std::cout;

int bin_search(const int* a, int n) {
    if (n == 2)
        return a[0] < a[1];

    if (a[0] > a[1])
        return 0;

    if (a[n-2] < a[n-1])
        return n-1;

    int ind = 1;
    while(a[ind] > a[ind - 1] && ind < n)
        ind *= 2;
    n = std::min(ind, n);

    int left = n / 2 - 1, right = n - 1;
    while(right - left > 1) {
        int middle = (right + left) / 2;
        if(a[middle + 1] < a[middle] && a[middle] > a[middle - 1])
            return middle;
        if(a[middle + 1] > a[middle])
            left = middle;
        else
            right = middle;
    }

    return (a[right + 1] < a[right] && a[right] > a[right - 1]) ? right : left;
}


int main() {
    int n = 0;
    cin >> n;

    int* a = new int [n];
    for(int i = 0; i < n; ++i)
        cin >> a[i];

    cout << bin_search(a, n);
    delete []a;
    return 0;
}

