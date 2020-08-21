// Алгоритм Куна
#include <iostream>
#include <vector>
using std::vector;
using std::cout;
using std::cin;

class ListGraph {
public:
    explicit ListGraph(size_t size);

    ListGraph(const ListGraph&) = delete;
    ListGraph(ListGraph&&) = delete;
    ListGraph& operator=(const ListGraph&) = delete;
    ListGraph& operator=(ListGraph&&) = delete;

    void addEdge(size_t from, size_t to);
    vector<size_t> getNextVertices(size_t vertex) const;

private:
    vector<vector<size_t>> adjLists;

};

ListGraph::ListGraph(size_t size) : adjLists(size, vector<size_t>()) {}


vector<size_t> ListGraph::getNextVertices(size_t vertex) const {
    return adjLists[vertex];
}


void ListGraph::addEdge(size_t from, size_t to) {
    adjLists[from].push_back(to);
}


bool kuhnHelp(const ListGraph& graph, vector<bool>& used, vector<int>& match, int vertex) {
    if(used[vertex])
        return false;
    used[vertex] = true;

    vector<size_t> adjList = graph.getNextVertices(vertex);
    for(const auto& to : adjList) {
        if(match[to] == -1){
            match[to] = vertex;
            return true;
        }
        if(kuhnHelp(graph, used, match, match[to])) {
            match[to] = vertex;
            return true;
        }
    }
    return false;
}

long long kuhn(const ListGraph& graph, int black_count, int white_count) {
    vector<int> match(white_count, -1);
    vector<bool> used(black_count, false);
    long long max_matching = 0;

    for(int i = 0; i < black_count; ++i) {
        if(kuhnHelp(graph, used, match, i)) {
            used.assign(black_count, false);
            max_matching++;
        }
    }

    return max_matching;
}


void getBipartiteGraph(const vector<vector<int>>& input, ListGraph& graph, int n, int m) {

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if(input[i][j] != -1) {
                if((i + j) % 2 == 1) { // если клетка черная то нужно добавить ее соседей
                    if(i - 1 >= 0 && input[i - 1][j] != -1)
                        graph.addEdge(input[i][j], input[i - 1][j]);

                    if(j - 1 >= 0 && input[i][j - 1] != -1)
                        graph.addEdge(input[i][j], input[i][j - 1]);

                    if(j + 1 < m && input[i][j + 1] != -1)
                        graph.addEdge(input[i][j], input[i][j + 1]);

                    if(i + 1 < n && input[i + 1][j] != -1)
                        graph.addEdge(input[i][j], input[i + 1][j]);


                }
            }
        }
    }
}

int main() {
    int n = 0, m = 0, a = 0, b = 0;
    cin >> n >> m >> a >> b;
    vector<vector<int>> input(n, vector<int> (m, -1));
    int white_count = 0, black_count = 0;
    std::string new_string;
    for (int i = 0; i < n; ++i) {
        cin >> new_string;
        for (int j = 0; j < m; ++j) {
            if (new_string[j] == '*') {
                if((i + j) % 2 == 0) {
                    input[i][j] = white_count++;
                }
                else {
                    input[i][j] = black_count++;
                }
            }
        }
    }

    if(2 * b <= a) {
        cout << b * (black_count + white_count);
        return 0;
    }

    ListGraph graph(black_count);
    getBipartiteGraph(input, graph, n, m);

    long long count_of_big_blocks = kuhn(graph, black_count, white_count);
    cout << count_of_big_blocks * a + (black_count + white_count - count_of_big_blocks * 2) * b;
    return 0;
}


