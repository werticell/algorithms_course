// LCA методом двоичных подъемов
#include <iostream>
#include <vector>
#include <cmath>
#include <stack>
using std::stack;
using std::vector;
using std::cin;
using std::cout;
using std::pair;

class LCA{
public:
    explicit LCA(const vector<vector<int>>& graph);
    
    LCA(const LCA&) = delete;
    LCA(LCA&&) = delete;
    LCA& operator=(const LCA&) = delete;
    LCA& operator=(LCA&&) = delete;
    int findLCA(int query_l, int query_r);

private:

    void DFS(int vertex, int parent);
    bool isAncestor(int a, int b);

    int timer;
    int sparse_table_size;
    vector<vector<int>> graph;
    vector<pair<int, int>> time; // время: первое - вход, второе - выход
    vector<vector<int>> sparse_table;
};


LCA::LCA(const vector<vector<int>>& graph) : graph(graph), sparse_table_size(ceil(log2((double)graph.size()))), time(graph.size()),
                                             sparse_table(graph.size(), vector<int>(sparse_table_size + 1)), timer(0)
{
    DFS(0, 0);
}

struct dfsVertex {
    dfsVertex(int vertex, int parent, int neighbour) : vertex(vertex), parent(parent), neighbour(neighbour) {}
    int vertex;
    int parent;
    int neighbour;
};
void LCA::DFS(int vertex, int parent)
{
    stack<dfsVertex> graphStack;
    graphStack.push(dfsVertex(vertex, parent, 0));

    while(!graphStack.empty()) {
        dfsVertex current_vertex = graphStack.top();
        if(current_vertex.neighbour == 0) {
            time[current_vertex.vertex].first = ++timer;
            sparse_table[current_vertex.vertex][0] = current_vertex.parent;
            for (int i = 1; i <= sparse_table_size; ++i)
                sparse_table[current_vertex.vertex][i] = sparse_table[sparse_table[current_vertex.vertex][i - 1]][i - 1];
        }

        if(current_vertex.neighbour >= graph[current_vertex.vertex].size()) {
            // если номер соседа вершины превысил количество ее соседей то необходимо вытащить ее из стэка
            graphStack.pop();
            time[current_vertex.vertex].second = ++timer;
        }
        else {
            ++graphStack.top().neighbour; // еще одно ребро просмотрено
            graphStack.push(dfsVertex(graph[current_vertex.vertex][current_vertex.neighbour], current_vertex.vertex, 0));
            // кладем смемжную вершину по индексу если она еще не посещена

        }
    }

}

bool LCA::isAncestor(int a, int b) {
    return time[b].second <= time[a].second&& time[a].first <= time[b].first;
}

int LCA::findLCA(int query_l, int query_r) {
    if (isAncestor(query_l, query_r))
        return query_l;
    if (isAncestor(query_r, query_l))
        return query_r;

    for (int i = sparse_table_size; i >= 0; --i)
        if (!isAncestor(sparse_table[query_l][i], query_r))
            query_l = sparse_table[query_l][i];
    return sparse_table[query_l][0];
}




int main() {
    int n = 0, m = 0;
    cin >> n >> m;

    int vertex = 0;
    vector<vector<int>> graph(n);
    for(int i = 1; i < n ; ++i) {
        cin >> vertex;
        graph[vertex].emplace_back(i);
    }

    long long a0 = 0, a1 = 0, x = 0 ,y = 0, z = 0;
    cin >> a0 >> a1;
    cin >> x >> y >> z;

    LCA my_lca(graph);

    long long answer = 0, result = 0, query_r = a1, query_l = a0;
    for (int i = 0; i < m; ++i) {
        result = my_lca.findLCA((int)query_l, (int)query_r);
        answer += result;
        a0 = (a0 * x + a1 * y + z) % n;
        a1 = (a1 * x + a0 * y + z) % n;
        query_l = (a0 + result) % n;
        query_r = a1;
    }

    cout << answer;
    return 0;
}

