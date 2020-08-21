// Топологическая сортировка
#include <iostream>
#include <vector>
#include <stack>
using std::vector;
using std::cin;
using std::cout;
using std::stack;

class ListGraph {
public:
    explicit ListGraph(int count) : adjLists(count) {}
    ListGraph(const ListGraph&) = delete;
    ListGraph(ListGraph&&) = delete;
    ListGraph& operator=(const ListGraph&) = delete;
    ListGraph& operator=(ListGraph&&) = delete;


    void addEdge(size_t from, size_t to);
    size_t verticesCount() const { return adjLists.size();}
    vector<int> getNextVertices(size_t vertex) const;

private:
    vector<vector<int>> adjLists;
};


vector<int> ListGraph::getNextVertices(size_t vertex) const
{
    return adjLists[vertex];
}

void ListGraph::addEdge(size_t from, size_t to)
{
    adjLists[from].push_back(to);
}


enum VertexStatus {
    NOT_VISITED,
    IN_WORK,
    VISITED
};

bool DFS(const ListGraph& graph, int vertex, vector<VertexStatus>& vertexStatus, vector<int>& answer) {

    stack <std::pair<int, int>> graphStack; // стек в котором хранятся пары (элемент, текущий просматриваемый сосед)
    graphStack.push( {vertex,0});
    vertexStatus[vertex] = IN_WORK;
    vector <int> vertices;

    while(!graphStack.empty()) {
        int currentVertex = graphStack.top().first;
        int currentIndex = graphStack.top().second;
        vertices = graph.getNextVertices(currentVertex);

        if(currentIndex >= vertices.size()) {
            // если индекс вершины превысил количество смежных с ней вершин, то необходимо вытащить ее из стека
            graphStack.pop();
            vertexStatus[currentVertex] = VISITED;
            answer.push_back(currentVertex);
        }
        else {
            ++graphStack.top().second; // еще одно ребро просмотрено

            if (vertexStatus[vertices[currentIndex]] == IN_WORK) // нашли цикл
                return false;

            if (vertexStatus[vertices[currentIndex]] == NOT_VISITED) {
                graphStack.push({vertices[currentIndex], 0}); // кладем смемжную вершину по индексу, если она еще не посещена
                vertexStatus[vertices[currentIndex]] = IN_WORK;
            }

        }
    }
    return true;
}


bool hasCycle(const ListGraph& graph, vector<int>& answer) {
    vector <VertexStatus> vertexStatus(graph.verticesCount(), NOT_VISITED);

    for (int i = 0; i < graph.verticesCount(); ++i)
        if(vertexStatus[i] == NOT_VISITED)
            if(!DFS(graph, i, vertexStatus, answer))
                return true;
    return false;
}


int main() {
    int n = 0, m = 0;
    cin >> n >> m;
    ListGraph graph(n);

    int from = 0, to = 0;
    for(int i = 0; i < m; ++i) {
        cin >> from >> to;
        if(from == to) {
            cout << "NO";
            return 0;
        }
        graph.addEdge(from, to);
    }


    vector<int> answer;
    if(hasCycle(graph, answer))
        cout << "NO";
    else {
        cout << "YES" << std::endl;
        for (int i = graph.verticesCount() - 1; i >= 0; --i) {
            cout << answer[i] << " ";
        }
    }
    return 0;
}

