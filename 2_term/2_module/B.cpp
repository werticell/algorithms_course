#include <iostream>
#include <vector>
#include <set>
using std::vector;
using std::cin;
using std::cout;
using std::set;
using std::pair;

const unsigned int INF = 1u << 31u;

template<typename T>
class ListGraph {
public:
    explicit ListGraph(size_t size);
    
    ListGraph(const ListGraph&) = delete;
    ListGraph(ListGraph&&) = delete;
    ListGraph& operator=(const ListGraph&) = delete;
    ListGraph& operator=(ListGraph&&) = delete;

    void addEdge(size_t from, size_t to, const T& weight);
    int getSize() const;
    vector<pair<int, T>> getNextVertices(size_t vertex) const;

private:
    vector<vector<pair<int,T>>> adjLists;
};

template<typename T>
ListGraph<T>::ListGraph(size_t size) : adjLists(size, vector<pair<int, T>>()) {}

template<typename T>
int ListGraph<T>::getSize() const {
    return adjLists.size();
}

template<typename T>
vector<pair<int, T>> ListGraph<T>::getNextVertices(size_t vertex) const {
    return adjLists[vertex];
}

template<typename T>
void ListGraph<T>::addEdge(size_t from, size_t to, const T& weight) {
    adjLists[from].push_back({to, weight});
}



template<typename T>
void fordBellman(const ListGraph<T>& graph, vector<vector<T>>& dist, int k, int start) {
    dist[0][start] = 0;
    for(int i = 0; i < k; ++i) {
        for(int j = 0; j < graph.getSize(); ++j) {
            
            if(dist[i][j] != INF) {
                vector<pair<int, int>> adjList;
                adjList = graph.getNextVertices(j);
                for (const pair<int, int>& x : adjList)
                    if (dist[i][j] + x.second < dist[i + 1][x.first])
                        dist[i + 1][x.first] = dist[i][j] + x.second;
            }

        }
        dist[i + 1][start] = 0;
    }
}


template<typename T>
T getShortestPath(const vector<vector<T>>& dist, int k, int to) {
    return dist[k][to] == INF ? -1 : dist[k][to];
}



int main() {
    int n = 0, m = 0, s = 0, f = 0, k = 0;
    cin >> n >> m >> k >> s >> f;
    ListGraph<int> graph(n);
    int from = 0, to = 0;
    int weight = 0;
    for(int i = 0; i < m; ++i) {
        cin >> from >> to >> weight;
        graph.addEdge(from - 1, to - 1, weight);
    }

    /*
     * Таблица расстояний:
     * по горизонтали - вершина для которой считаем значение динамики,
     * по вертикали - слой динамики каждый k-ый слой отвечает за длину кратчайшего пути до вершины в столбце
     * состоящего из не более чем k ребер
     */
    vector<vector<int>> dist(k + 1, vector<int>(graph.getSize(), INF));

    fordBellman(graph, dist, k,s - 1);
    cout << getShortestPath(dist, k,f - 1);

    return 0;
}

