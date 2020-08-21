#include <iostream>
#include <vector>
#include <deque>
using std::vector;
using std::cin;
using std::cout;
using std::pair;
using std::deque;
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
    size_t getSize() const;

    void getNextVertices(size_t vertex, vector<pair<size_t, T>>& vertices) const;

private:
    vector<vector<pair<size_t, T>>> adjLists;

};

template<typename T>
ListGraph<T>::ListGraph(size_t size) : adjLists(size, vector<pair<size_t, T>>()) {}

template<typename T>
size_t ListGraph<T>::getSize() const {
    return adjLists.size();
}

template<typename T>
void ListGraph<T>::getNextVertices(size_t vertex, vector<pair<size_t, T>>& vertices) const {
    vertices.clear();
    for (const pair<size_t, T>& temp : adjLists[vertex])
        vertices.push_back(temp);
}

template<typename T>
void ListGraph<T>::addEdge(size_t from, size_t to, const T& weight) {
    adjLists[from].push_back({to, weight});
}


template<typename T>
void BFS_0_1(const ListGraph<T>& graph, vector<T>& dist, int start, int cost1, int cost2) {
    deque<int> my_deque;
    my_deque.push_back(start);
    dist[start] = 0;

    while (!my_deque.empty()) {
        int vertex = my_deque.front();
        my_deque.pop_front();

        vector<pair<size_t, T>> adjList;
        graph.getNextVertices(vertex, adjList);

        for(const pair<size_t, T>& x :adjList) {
            if (dist[x.first] > dist[vertex] + x.second) {
                dist[x.first] = dist[vertex] + x.second;
                if (x.second == std::min(cost1, cost2))
                    my_deque.push_front(x.first);
                else
                    my_deque.push_back(x.first);
            }
        }
    }
}


template<typename T>
T getShortestPath(const vector<T>& dist, int to) {
    return dist[to] == INF ? -1 : dist[to];
}


int main() {
    int cost1 = 0, cost2 = 0, n = 0, s = 0, f = 0;
    cin >> cost1 >> cost2 >> n >> s >> f;
    ListGraph<long long> graph(n);
    for(size_t i = 0; i < n; ++i) {
        size_t target1 = (i + 1) % n;
        size_t target2 = ((i % n) * (i % n)+ 1 % n) % n;
        if(target1 == target2)
            graph.addEdge(i, target1, std::min(cost1, cost2));

        if(cost1 < cost2) {
            graph.addEdge(i, target1, cost1);
            graph.addEdge(i, target2, cost2);
        }
        if(cost2 < cost1) {
            graph.addEdge(i, target2, cost2);
            graph.addEdge(i, target1, cost1);
        }
    }

    vector<long long> dist(graph.getSize(), INF);

    BFS_0_1(graph, dist, s, cost1, cost2);
    cout << getShortestPath(dist, f);
    return 0;
}





