#include <iostream>
#include <vector>
#include <algorithm>
using std::vector;
using std::cin;
using std::cout;
using std::pair;
using ull = unsigned long long;

class DSU {
public:

    explicit DSU(int size);

    int getDelegate(int vertex);
    void tryUnion(int first, int second);
private:
    vector<int> parent;
    vector<int> rank;
};

DSU::DSU (int size) {
    rank.resize(size, 1);
    for(int i = 0; i < size; ++i) {
        parent.push_back(i);
    }
}

int DSU::getDelegate(int vertex) {
    if(parent[vertex] == vertex)
        return vertex;
    else
        return parent[vertex] = getDelegate(parent[vertex]);
}

void DSU::tryUnion(int first, int second) {
    first = getDelegate(first);
    second = getDelegate(second);
    if(first != second) {
        if(rank[first] < rank[second])
            std::swap(first, second);
        parent[second] = first;
        rank[first] += rank[second];
    }

}


struct Edge {
    Edge(int from, int to, ull weight) : from(from), to(to), weight(weight) {}
    int from;
    int to;
    ull weight;
};


vector<Edge> kruscal(vector<Edge>& edgeList, int vertexCount) {
    std::sort(edgeList.begin(), edgeList.end(), [](const Edge& e1, const Edge& e2){return e1.weight < e2.weight;});
    DSU components(vertexCount);
    vector<Edge> resultMST;

    for (const auto& new_edge : edgeList) {
        if (components.getDelegate(new_edge.to) != components.getDelegate(new_edge.from)) {
            resultMST.push_back(new_edge);
            components.tryUnion(new_edge.from, new_edge.to);
        }
    }
    return resultMST;
}


ull findMSTWeight(const vector<Edge>& edgeList) {
    ull result = 0;
    for (const auto& new_edge : edgeList)
        result += new_edge.weight;
    return result;
}



int main() {
    int n = 0, m = 0;
    cin >> n >> m;
    int from = 0, to = 0, min_vertex_number = 0;
    ull weight = 0, min_weight = 1ull << 63ull;
    vector<Edge> edgeList;

    vector<ull> verticesCost;
    for (int i = 0; i < n; ++i) {
        cin >> weight;
        verticesCost.emplace_back(weight);
        if(weight < min_weight) {
            min_weight = weight;
            min_vertex_number = i;
        }
    }

    for (int i = 0; i < n; ++i) {
        if (i != min_vertex_number) {
            ull result_cost = min_weight + verticesCost[i];
            edgeList.emplace_back(min_vertex_number, i, result_cost);
        }

    }

    for (int i = 0; i < m; ++i) {
        cin >> from >> to >> weight;
        if (from != to)
            edgeList.emplace_back(from - 1, to - 1, weight);
    }

    cout << findMSTWeight(kruscal(edgeList, n));
    return 0;
}





