#include <iostream>
using std::cin;
using std::cout;

std::pair<int, int> find_max_pair(const int* a, const int* b, int n) {
    int max_value = a[n - 1] + b[n - 1];
    int max_value_b = n - 1;
    int max_value_ind_a = 0, max_value_ind_b = 0;

    for(int i = n - 1; i >= 0; --i) {
        if(b[i] >= b[max_value_b])
            max_value_b = i;
        if(a[i] + b[max_value_b] >= max_value) {
            max_value_ind_a = i;
            max_value_ind_b = max_value_b;
            max_value = a[i] + b[max_value_b];
        }
    }
    return {max_value_ind_a, max_value_ind_b};
}
int main() {
    int n = 0;
    cin >> n;
    int* a = new int [n];
    int* b = new int [n];

    for(int i = 0; i < n; ++i)
        cin >> a[i];
    for(int i = 0; i < n; ++i)
        cin >> b[i];

    auto p = find_max_pair(a, b, n);

    delete []a;
    delete []b;
    cout << p.first << " " << p.second;
    return 0;
}

