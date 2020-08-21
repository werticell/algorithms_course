#include <iostream>
#include <vector>
#include <set>
#include <iomanip>
using std::vector;
using std::cin;
using std::cout;
using std::set;
using std::pair;

const double INF = 2;

template<typename T>
class ListGraph {
public:
    explicit ListGraph(size_t size);
    ListGraph(const ListGraph&) = default;
    ListGraph(ListGraph&&) = delete;
    ListGraph& operator=(const ListGraph&) = delete;
    ListGraph& operator=(ListGraph&&) = delete;


    void addEdge(size_t from, size_t to, const T& weight );
    size_t getSize() const;
    vector<pair<T, size_t>> getNextVertices(size_t vertex) const;

private:
    vector<vector<pair<T,size_t>>> adjLists;

};


template<typename T>
ListGraph<T>::ListGraph(size_t size) : adjLists(size, vector<pair<T, size_t>>()) {}


template<typename T>
size_t ListGraph<T>::getSize() const {
    return adjLists.size();
}


template<typename T>
vector<pair<T, size_t>> ListGraph<T>::getNextVertices(size_t vertex) const {
    return adjLists[vertex];
}


template<typename T>
void ListGraph<T>::addEdge(size_t from, size_t to, const T& weight) {
    adjLists[from].push_back({weight, to});
}



template<typename T>
void dijkstra(const ListGraph<T>& graph, vector<T>& dist, int start) {
    /*
     * Массив dist хранит насчитанные кратчайшие расстояния от исходной вершины до всех остальных
     * Массив used говорит прорелаксировали ли все ребра исходящие из данной вершины
     * В set'e current_dist хранятся пары <насчитанный "кратчайший" путь, вершина>
     */
    vector<int> used(graph.getSize(),0);
    set<pair<T, int>> current_dist;
    current_dist.emplace(0, start);
    dist[start] = 0;
    vector<pair<T, size_t >> adjList;
    while (!current_dist.empty()) {

        int vertex = current_dist.begin()->second;
        current_dist.erase(current_dist.begin());
        used[vertex] = 1;

        adjList = graph.getNextVertices(vertex);
        for (const auto& x :adjList) {
            double tmp_dist = dist[vertex] * x.first + (1 - dist[vertex]) * x.first + dist[vertex] * (1 - x.first);
            if (tmp_dist < dist[x.second]) {
                if (dist[x.second] != INF)
                    current_dist.erase({dist[x.second], x.second});

                dist[x.second] = tmp_dist;
                current_dist.emplace(dist[x.second], x.second);

            }
        }

    }
}


template<typename T>
T getShortestPath (const vector<T>& dist, int to) {
    return dist[to];
}



int main() {
    int n = 0, m = 0, s = 0, f = 0;
    cin >> n >> m >> s >> f;
    ListGraph<double> graph(n);
    int from = 0, to = 0;
    double weight = 0;
    for (int i = 0; i < m; ++i) {
        cin >> from >> to >> weight;
        weight /= 100;
        graph.addEdge(from - 1, to - 1, weight);
        graph.addEdge(to - 1, from - 1, weight);
    }

    vector<double> dist(graph.getSize(), INF);

    dijkstra(graph, dist, s - 1);
    cout << std::setprecision(6) << getShortestPath(dist, f - 1);
    return 0;
}

