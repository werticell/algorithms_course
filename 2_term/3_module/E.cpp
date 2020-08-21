// Алгоритм Форда-Фалкерсона
#include <iostream>
#include <vector>
#include <algorithm>
using std::vector;
using std::cout;
using std::cin;
using std::cout;

const int MAX_INT = 1u << 16u;

enum EdgeType {
    STRAIGHT,
    INVERSE
};

struct Edge {
    Edge(int from, int to, int capacity,
         int next, EdgeType x) : from(from), to(to), capacity(capacity), next(next), type(x) {}
    int from;
    int to;
    int capacity;
    int next;
    int type;
};



bool dfs(vector<Edge>& edgeList, const vector<int>& vertexEdge, vector<bool>& visited, vector<int>& ans, int s, int t) {
    if (s == t) {   // мы дошли
        ans.push_back(s + 1);
        return true;
    }
    visited[s] = true;

    for(int i = vertexEdge[s]; i != -1; i = edgeList[i].next) {
        Edge& new_edge = edgeList[i];
        if(!visited[new_edge.to] && new_edge.capacity == 0  && new_edge.type == STRAIGHT) {
            if(dfs(edgeList, vertexEdge, visited,ans, new_edge.to, t)) {
                new_edge.capacity = -1;
                ans.push_back(s + 1);
                return true;
            }

        }
    }
}


int findIncreasePath(vector<Edge>& edgeList, vector<bool>& visited, const vector<int>& vertexEdge, int from, int to, int flow) {
    if (from == to)
        return flow;
    visited[from] = true;
    for (int i = vertexEdge[from]; i != -1; i = edgeList[i].next) {
        Edge& new_edge = edgeList[i];
        if( new_edge.capacity > 0 && !visited[new_edge.to]) {
            int df = findIncreasePath(edgeList,visited, vertexEdge, new_edge.to, to, std::min(flow, new_edge.capacity));
            if( df > 0) {
                new_edge.capacity -= df;
                edgeList[(unsigned)i ^ 1u].capacity += df;  // в нашем списке ребер обратное и прямое хранятся рядом просто с разной четностью
                return df;
            }
        }
    }
    return 0;
}

int findMaxFlow(vector<Edge>& edgeList, const vector<int>& vertexEdge, int s, int t) {
    int flow = 0;
    vector<bool> visited(vertexEdge.size(), false);
    while(true) {
        visited.assign(vertexEdge.size(), false);
        int df = findIncreasePath(edgeList, visited, vertexEdge, s, t, MAX_INT);
        if (df == 0)
            return flow;
        flow += df;
    }
}

void printAns(const vector<int>& ans) {
    for(auto it = ans.rbegin(); it != ans.rend(); ++it) {
        if(*it != -1)
            cout << *it << " ";
    }
}

void findDisjointPaths(vector<Edge>& edgeList, const vector<int>& vertexEdges, int s, int t, int max_flow) {

    if(max_flow >= 2) {
        cout << "YES" << std::endl;
        for (int i = 1; i <= 2; ++i) {
            vector<bool> visited(vertexEdges.size(), false);
            vector<int> ans(vertexEdges.size(), -1);
            dfs(edgeList, vertexEdges, visited, ans, s - 1, t - 1);
            printAns(ans);
            cout << std::endl;
        }

    } else
        cout << "NO";
}


int main() {
    int n = 0, m = 0, s = 0, t = 0, from = 0, to = 0;
    cin >> n >> m >> s >> t;

    vector<Edge> edgeList;  // список ребер нашего графа
    vector<int> vertexEdges(n,-1); // хранит в себе индекс последнего добавленного ребра исходящего из данной вершины

    for(int i = 0; i < m; ++i) {
        cin >> from >> to;
        --from; --to;
        edgeList.emplace_back(from, to, 1, vertexEdges[from], STRAIGHT);
        vertexEdges[from] = edgeList.size() - 1;
        edgeList.emplace_back(to, from, 0,vertexEdges[to], INVERSE);
        vertexEdges[to] = edgeList.size() - 1;
    }

    int max_flow = findMaxFlow(edgeList, vertexEdges, s - 1, t - 1);
    findDisjointPaths(edgeList, vertexEdges, s, t, max_flow);

    return 0;
}


