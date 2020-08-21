#include <iostream>
#include <vector>
#include <stack>
using std::vector;
using std::cin;
using std::cout;
using std::stack;

class ListGraph {
public:
    explicit ListGraph(size_t count) : adjLists(count) {}

    ListGraph(const ListGraph&) = delete;
    ListGraph(ListGraph&&) = delete;
    ListGraph& operator=(const ListGraph&) = delete;
    ListGraph& operator=(ListGraph&&) = delete;

    void addEdge(size_t from, size_t to);

    size_t verticesCount() const;
    void getNextVertices(size_t vertex, vector<int>& vertices) const;
    int vertexAdjCount(size_t vertex) const;

private:
    vector<vector<int>> adjLists;
};

int ListGraph::vertexAdjCount(size_t vertex) const
{
    return adjLists[vertex].size();
}

size_t ListGraph::verticesCount() const
{
    return adjLists.size();
}

void ListGraph::getNextVertices(size_t vertex, vector<int> &vertices) const
{
    vertices = adjLists[vertex];
}

void ListGraph::addEdge(size_t from, size_t to)
{
    adjLists[from].push_back(to);
}



void DFS (const ListGraph& graph, int vertex, vector<bool>& vertexStatus, vector<int>& A,
          void(*visit)(int, vector<int>&, int), int currentColour = 0 )
{
    stack <std::pair<int, int>> graphStack; // стек в котором хранятся пары (элемент, текущий просматриваемый сосед)
    graphStack.push({vertex,0});
    vertexStatus[vertex] = true;
    vector <int> vertices;

    while (!graphStack.empty())
    {
        int current_vertex = graphStack.top().first;
        int current_index = graphStack.top().second;
        graph.getNextVertices(current_vertex, vertices);
        if(current_index >= graph.vertexAdjCount(current_vertex)) {
            graphStack.pop();
            vertexStatus[current_vertex] = true;
            visit(current_vertex, A, currentColour);
        }
        else {
            ++graphStack.top().second;
            if (!vertexStatus[vertices[current_index]]) {
                graphStack.push({vertices[current_index], 0});
                vertexStatus[vertices[current_index]] = true;
            }
        }
    }
}

std::pair<vector<int>, int> Kosaraju(const ListGraph& graph, const ListGraph& inverseGraph)
{
    int vertexCount = graph.verticesCount();
    vector <bool> vertexStatus(vertexCount, false); // 0 - not used, 1 - used
    vector <int> timeOut;

    for (int i = 0; i < vertexCount; ++i) { // создание массива овтета по возрастанию времени выхода DFS
        if (!vertexStatus[i])
            DFS(graph, i, vertexStatus , timeOut,
                 []( int currentVertex, vector<int>& timeOut, int x = 0){ timeOut.push_back(currentVertex);});
    }

    vertexStatus.clear();
    vertexStatus.resize(vertexCount, false);
    vector <int> componentsColour(vertexCount); // покраска вершин в зависимости от компоненты связности
    int currentColour = 0;

    for (int i = vertexCount - 1; i >= 0; --i) {
        if (!vertexStatus[timeOut[i]]) {
            componentsColour[timeOut[i]] = currentColour;
            DFS( inverseGraph, timeOut[i], vertexStatus, componentsColour,
                 []( int currentVertex, vector<int>& componentsColour, int currentColour){ componentsColour[currentVertex] = currentColour;}, currentColour );//
            ++currentColour;
        }
    }
    return {componentsColour, currentColour};
}

int findStrongConnectivity(const ListGraph& graph, const ListGraph& inverseGraph) {
    auto resultOfKosaraju = Kosaraju(graph, inverseGraph);
    int currentColour = resultOfKosaraju.second;
    vector<int> componentsColour = resultOfKosaraju.first;

    vector<bool> condIn(currentColour, false); // входит ребро или нет
    vector<bool> condOut(currentColour, false); // выходит ребро или нет
    vector<int> vertices;

    for (int i = 0; i < graph.verticesCount(); ++i) {
        graph.getNextVertices(i, vertices);
        for (const auto& to : vertices) {
            if (componentsColour[i] != componentsColour[to]) {
                condOut[componentsColour[i]] = true;
                condIn[componentsColour[to]] = true;
            }
        }
    }

    int inCount = 0, outCount = 0;
    for(int i = 0; i < currentColour; ++i) {
        if(!condIn[i])
            ++inCount;
        if(!condOut[i])
            ++outCount;
    }
    return (currentColour == 1 ? 0 : std::max(inCount, outCount));
}


int main() {
    int n = 0, m = 0;
    cin >> n >> m;
    ListGraph graph(n);
    ListGraph inverseGraph(n);
    int from = 0, to = 0;

    for(int i = 0; i < m; ++i) {
        cin >> from >> to;
        graph.addEdge(from - 1, to - 1);
        inverseGraph.addEdge(to - 1, from - 1);
    }
    cout << findStrongConnectivity(graph, inverseGraph);
    return 0;
}

