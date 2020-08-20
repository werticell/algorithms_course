#include <iostream>
using std::cin;
using std::cout;

int main() {
    int n = 0;
    cin >> n;
    int last_square = 1;
    for(int i = 1; i <= n; ++i) {
        last_square = ((i == 1) ? 1 : last_square + i + i - 1);
        cout << last_square << " ";
    }
    return 0;
}
