// Алгоритм Прима
#include <iostream>
#include <vector>
#include <set>
using std::vector;
using std::cin;
using std::cout;
using std::set;
using std::pair;

const unsigned int INF = 1u << 30u;

class ListGraph {
public:
    explicit ListGraph(size_t size);

    ListGraph(const ListGraph&) = delete;
    ListGraph(ListGraph&&) = delete;
    ListGraph& operator=(const ListGraph&) = delete;
    ListGraph& operator=(ListGraph&&) = delete;

    void addEdge(size_t from, size_t to, int weight);
    size_t getSize() const;

    void getNextVertices(size_t vertex, vector<pair<int, size_t>>& vertices) const;

private:

    vector<vector<pair<int, size_t>>> adjLists;

};

ListGraph::ListGraph(size_t size) : adjLists(size, vector<pair<int, size_t>>()) {}

size_t ListGraph::getSize() const {
    return adjLists.size();
}


void ListGraph::getNextVertices(size_t vertex, vector<pair<int, size_t>>& vertices) const {
    vertices.clear();
    for (const auto& temp : adjLists[vertex])
        vertices.push_back(temp);
}

void ListGraph::addEdge(size_t from, size_t to, int weight) {
    adjLists[from].push_back({weight, to});
}



long long mst_prim(const ListGraph& graph, size_t start) {

    vector<int> min_weight(graph.getSize(), INF);
    vector<int> used(graph.getSize(),0);
    set<pair<int, size_t>> not_in_mst_vertices;
    not_in_mst_vertices.emplace(0, start);
    min_weight[start] = 0;
    long long result = 0;
    while(!not_in_mst_vertices.empty()) {

        int vertex = not_in_mst_vertices.begin()->second;
        result += not_in_mst_vertices.begin()->first;
        not_in_mst_vertices.erase(not_in_mst_vertices.begin());
        used[vertex] = 1;

        vector<pair<int, size_t>> adjList;
        graph.getNextVertices(vertex, adjList);
        for (const auto& x :adjList) {
            if (x.first < min_weight[x.second] && used[x.second] != 1) {
                if (min_weight[x.second] != INF)
                    not_in_mst_vertices.erase({min_weight[x.second], x.second});

                min_weight[x.second] = x.first;
                not_in_mst_vertices.emplace(min_weight[x.second], x.second);

            }
        }
    }
    return result;
}


int main() {
    int n = 0, m = 0;
    cin >> n >> m;
    ListGraph graph(n);
    int from = 0, to = 0, weight = 0;
    for (int i = 0; i < m; ++i) {
        cin >> from >> to >> weight;
        graph.addEdge(from - 1, to - 1, weight);
        graph.addEdge(to - 1, from - 1, weight);
    }

    cout << mst_prim(graph, 0);
    return 0;
}
