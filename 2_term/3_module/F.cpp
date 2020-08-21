// Поиск MinCut'a за O(n^2) с помощью Форда-Фалкерсона
#include <iostream>
#include <vector>
#include <stack>
using std::stack;
using std::vector;
using std::cout;
using std::cin;
using std::cout;

const int MAX_INT = 1u << 16u;

void dfs(vector<vector<int>>& graph, vector<bool>& visited, int s) {
    stack<int> traversal;
    visited[s] = true;
    traversal.push(s);

    while(!traversal.empty()) {
        int vertex = traversal.top();
        visited[vertex] = true;
        traversal.pop();

        for(int i = 0; i < graph[vertex].size(); ++i)
            if(!visited[i] && graph[vertex][i] > 0)
                traversal.push(i);

    }
}


int findIncreasePath(vector<vector<int>>& graph, vector<bool>& visited, int u, int t, int flow) {
    if (u == t)
        return flow;
    visited[u] = true;
    for (int v = 0; v < visited.size(); ++v)
        if (!visited[v] && graph[u][v] > 0) {
            int df = findIncreasePath(graph, visited, v, t, std::min(flow, graph[u][v]));
            if (df > 0) {
                graph[u][v] -= df;
                graph[v][u] += df;
                return df;
            }
        }
    return 0;
}
void findReachableVertices(vector<vector<int>>& graph, int s) {
    vector<bool> answer(graph.size(),false);
    dfs(graph, answer, s);
    for(int i = 0; i < graph.size(); ++i) {
        if(answer[i])
            cout << i + 1 << " ";
    }
    cout << std::endl;
    for(int i = 0; i < graph.size(); ++i) {
        if(!answer[i])
            cout << i + 1 << " ";
    }
}

int findMaxFlow(vector<vector<int>> graph, int s, int t, int type = 0) { // type с возможностью получить minCut для данных s и t
    int flow = 0;
    vector<bool> visited(graph.size(), false);
    while(true) {
        visited.assign(graph.size(), false);
        int df = findIncreasePath(graph, visited, s, t, MAX_INT);
        if (df == 0) {
            if(type == 1) { //  то решаем задачу
                findReachableVertices(graph, s);
            }
            return flow;
        }
        flow += df;
    }
}

void findMinCut(vector<vector<int>>& graph) {
    vector<bool> visited(graph.size(), false);
    vector<int> ans(graph.size(), -1);
    int overall_max_flow = MAX_INT, desired_s = 0, desired_t = 0;

    for(int s = 0; s < graph.size(); ++s) {
        for(int t = s + 1; t < graph.size(); ++t) {
            int current_max_flow = findMaxFlow(graph, s, t);
            if(current_max_flow < overall_max_flow) {
                overall_max_flow = current_max_flow;
                desired_s = s;
                desired_t = t;
            }
        }
    }

    findMaxFlow(graph, desired_s, desired_t, 1);

}


int main() {
    int n = 0;
    cin >> n;
    vector<vector<int>> graph(n, vector<int> (n, 0));
    std::string input;
    for(int i = 0; i < n; ++i) {
        cin >> input;
        for(int j = 0; j < n; ++j) {
            graph[i][j] = input[j] - '0';
            graph[j][i] = input[j] - '0';
        }
    }

    findMinCut(graph);

    return 0;
}


