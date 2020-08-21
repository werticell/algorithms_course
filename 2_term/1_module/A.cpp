#include <iostream>
#include <vector>
#include <queue>
using std::vector;
using std::cin;
using std::cout;
using std::queue;

class ListGraph {
public:
    explicit ListGraph(int count) : adjLists(count) {}
    ListGraph(const ListGraph&) = delete;
    ListGraph(ListGraph&&) = delete;
    ListGraph& operator=(const ListGraph&) = delete;
    ListGraph& operator=(ListGraph&&) = delete;


    void addEdge(size_t from, size_t to);

    int verticesCount() const { return adjLists.size(); }
    void getNextVertices(size_t vertex, vector<int>& vertices) const;
    void getPrevVertices(size_t vertex, vector<int>& vertices) const;

private:
    vector<vector<int>> adjLists;

};

void ListGraph::getNextVertices(size_t vertex, vector<int>& vertices) const
{
    vertices = adjLists[vertex];
}

void ListGraph::getPrevVertices(size_t vertex, vector<int>& vertices) const
{
    vertices.clear();
    for(int from = 0; from < adjLists.size(); ++from) {
        for(int i = 0; i < adjLists[from].size(); ++i)
            if(adjLists[from][i] == vertex)
                vertices.push_back(from);

    }
}
void ListGraph::addEdge(size_t from, size_t to)
{
    adjLists[from].push_back(to);
}


template <typename Visit>
void BFS(const ListGraph& graph, int vertex, vector<int>& Distance, Visit visit = [](auto& v){ cout << v;})
{
    queue<int> graphQueue;
    vector<bool> visited(graph.verticesCount(), false);
    graphQueue.push(vertex);
    visited[vertex] = true;

    while (!graphQueue.empty()) {
        int currentVertex = graphQueue.front();
        graphQueue.pop();

        vector <int> vertices;
        graph.getNextVertices(currentVertex, vertices);
        for (const auto& to : vertices) {
            if (!visited[to]) {
                visit(to, currentVertex, Distance);
                graphQueue.push(to);
                visited[to] = true;
            }
        }
    }
}

int main() {
    int n = 0, m = 0, leon = 0, matilda = 0, milk = 0;
    cin >> n >> m >> leon >> matilda >> milk;
    --leon; --matilda; --milk;
    ListGraph graph(n);
    int from = 0, to = 0;
    for (int i = 0; i < m; ++i) {
        cin >> from >> to;
        --from; --to;
        graph.addEdge(from, to);
        graph.addEdge(to,from);
    }

    vector<int> leonDistance(n, 0);
    leonDistance[leon] = 0;
    BFS(graph, leon, leonDistance, [](int vertex, int ancestor, vector<int>& Distance) { Distance[vertex] = Distance[ancestor] + 1; });

    vector<int> matildaDistance(n, 0);
    matildaDistance[matilda] = 0;
    BFS(graph, matilda, matildaDistance, [](int vertex, int ancestor, vector<int>& Distance) { Distance[vertex] = Distance[ancestor] + 1; });

    vector<int> milkDistance(n, 0);
    milkDistance[milk] = 0;
    BFS(graph, milk, milkDistance, [](int vertex, int ancestor, vector<int>& Distance) { Distance[vertex] = Distance[ancestor] + 1; });

    int min = milkDistance[0] + leonDistance[0] + matildaDistance[0];
    for (int i = 0; i < n; ++i) {
        if(milkDistance[i] + leonDistance[i] + matildaDistance[i] < min)
            min = milkDistance[i] + leonDistance[i] + matildaDistance[i];
    }
    cout << min;
    return 0;
}



