#include<iostream>
#include<vector>
#include<stack>
using std::cin;
using std::cout;
using std::vector;
using std::stack;


struct Node {
    int count = 0; // количество вершин в дереве с корнем в данной вершине
    int prev_number; // номер родительской вершины
    int destination;
    Node() : prev_number(0), destination(0) {}
};

enum colour {
    WHITE,
    GREY,
    BLACK
};

int DFS(int u, vector<vector<int>>& graph, vector<Node>& nodes) {
    stack<int> myStack;
    myStack.push(u);
    vector<colour> status(nodes.size(), WHITE);

    while(!myStack.empty()) {
        int currentVertex = myStack.top();
        if(graph[currentVertex].empty()) { //оказались в вершине у которой нет детей
            ++nodes[currentVertex].count;
            status[currentVertex] = GREY;
            myStack.pop();
        }
        else {
            for (const auto& v : graph[currentVertex]) {

                if(status[v] == WHITE) {
                    myStack.push(v);
                    break;
                }

                if( status[v] == GREY ) {
                    nodes[currentVertex].destination += nodes[v].destination + nodes[v].count;
                    nodes[currentVertex].count += nodes[v].count;
                    status[v] = BLACK;
                }

                if(status[v] == BLACK && v == graph[currentVertex].back()) {
                    ++nodes[currentVertex].count;
                    status[myStack.top()] = GREY;
                    myStack.pop();
                    break;
                }
            }
        }
    }
    return nodes[0].destination;

}

int main(){
    int n = 0;
    cin >> n;
    int x = 0, y = 0;

    vector<vector<int>> graph(n);
    vector<Node> nodes(n);

    for (int i = 0; i < n - 1; ++i) {
        cin >> x >> y;
        if(x > y)
            std::swap(x,y);
        graph[x].push_back(y); // если индекс вершины меньше, то она родительская
        nodes[y].prev_number = x;
    }

    nodes[0].destination = DFS(0,graph, nodes);

    for (int i = 0; i < n; ++i) {
        if(i != 0)
            nodes[i].destination = nodes[nodes[i].prev_number].destination + n - 2 * nodes[i].count;
        cout << nodes[i].destination << std::endl;
    }

    return 0;
}

