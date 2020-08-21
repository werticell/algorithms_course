// Транизитивное замыкание орграфа
#include <iostream>
#include <vector>
#include <string>
#include <bitset>
using std::vector;
using std::cin;
using std::cout;
using std::string;
using std::bitset;


void floyd(vector<bitset<1000>>& graph) {
    for (size_t k = 0 ; k < graph.size(); ++k)
        for (size_t i = 0 ; i < graph.size(); ++i){
            bitset<1000> tmp;
            if (graph[i][graph.size() - 1 - k]  == 1)
                tmp.set();
            else
                tmp.reset();
            graph[i] = graph[i] | (tmp & graph[k]);
        }
}


int main() {
    size_t n = 0;
    cin >> n;
    vector<bitset<1000>> graph(n);
    string input;
    for (int i = 0; i < n; ++i)
        cin >> graph[i];

    floyd(graph);

    for (size_t i  = 0; i < graph.size(); ++i) {
        for (size_t j = 0; j < graph.size(); ++j)
            cout << graph[i][n - 1 - j];
        cout << std::endl;
    }
    return 0;
}



