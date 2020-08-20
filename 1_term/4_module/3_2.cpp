#include <iostream>
using std::cin;
using std::cout;

/* p(n, k) = p(n, k - 1) + p(n - k, k - 1)
 * берутся из двух клеток таблицы array[i][j - 1] и array[i - j][j - 1]
*/
unsigned long long getAnswer(int n)
{
    auto** array = new unsigned long long* [n + 1];

    for (int i = 0; i <= n; ++i)
        array[i] = new unsigned long long[n + 1];

    /*
     двумерная динамика по ширине нижнего слоя и количеству используемых кубиков
     в строках с номером i лежит количество пирамид с шириной <= j
     в столбцах количество кубиков
    */
    for (int i = 0; i <= n; ++i)
        array[0][i] = 1;

    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            if (j - i >= 1) /* если ширина больше чем максимальное количество используемых кубиков
                 то количесвтов таких пирамид равно количеству пирамид с шириной не большей чем количество кубиков */
                array[i][j] = array[i][j - 1];
            else
                array[i][j] = array[i][j - 1] + array[i - j][j - 1];
    unsigned long long temp = array[n][n];

    for (int i = 0; i <= n; ++i)
        delete[] array[i];

    delete[] array;

    return temp;

}

int main() {
    int n = 0;
    cin >> n;
    cout << getAnswer(n);
    return 0;
}

