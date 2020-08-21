// поиск цикла отрицательного веса
#include <iostream>
#include <vector>
using std::vector;
using std::cin;
using std::cout;

const double INF = -1.00;

struct Changer {
    Changer() = default;
    Changer(int from, int to, double rate, double commission) : from(from) , to(to), rate(rate), commission(commission) {}
    size_t from;
    size_t to;
    double rate;
    double commission;
};



class ListGraph {
public:
    explicit ListGraph(size_t size);

    ListGraph(const ListGraph&) = delete;
    ListGraph(ListGraph&&) = delete;
    ListGraph& operator=(const ListGraph&) = delete;
    ListGraph& operator=(ListGraph&&) = delete;

    void addEdge(size_t from, size_t to, double rate, double commission);
    int getSize() const;
    vector<Changer> getPreviousVertices(size_t vertex) const;
private:
    vector<vector<Changer>> inAdjLists;


};


ListGraph::ListGraph(size_t size) : inAdjLists(size, vector<Changer>()) {}


int ListGraph::getSize() const {
    return inAdjLists.size();
}

vector<Changer> ListGraph::getPreviousVertices(size_t vertex) const {
    return inAdjLists[vertex];
}


void ListGraph::addEdge(size_t from, size_t to, double rate, double commission) {
    inAdjLists[to].emplace_back(from, to, rate, commission);
}




bool hasNegativeCycle(const ListGraph& graph, int start, double value) {
    vector<double> dist(graph.getSize(), INF);
    vector<Changer> vertices(graph.getSize());
    dist[start] = value;

    for (int i = 0; i < graph.getSize() - 1; ++i) {
        for (int v = 0; v < graph.getSize(); ++v) {
            vertices = graph.getPreviousVertices(v);

            for (const auto& u : vertices)
                if (dist[u.from] != INF  && (dist[u.from] - u.commission >= 0) && (dist[v] < (dist[u.from] - u.commission) * u.rate) )
                    dist[v] = (dist[u.from] - u.commission) * u.rate;
        }
    }

    for (int v = 0; v < graph.getSize(); ++v) {
        vertices = graph.getPreviousVertices(v);
        for (auto& u : vertices)
            if (dist[u.from] != INF && (dist[u.from] - u.commission >= 0) && (dist[v] < (dist[u.from] - u.commission) * u.rate) )
                return true;
    }

    /*
     * Если пришли сюда, значит в результате принудительного запуска еще одной итерации алгоритма Форда-Беллмана
     * ни одного уже насчитанное расстояние не изменилось
     */
    return false;
}


int main() {
    int n = 0, m = 0, s = 0;
    double v = 0;
    cin >> n >> m >> s >> v;
    ListGraph graph(n);
    int from = 0, to = 0;
    double rateA, commissionA, rateB, commissionB;
    for(int i = 0; i < m; ++i) {
        cin >> from >> to >> rateA >> commissionA >> rateB >> commissionB;
        graph.addEdge(from - 1, to - 1, rateA, commissionA);
        graph.addEdge(to - 1, from - 1, rateB, commissionB);
    }

    std::string result = hasNegativeCycle(graph, s - 1 , v) ? "YES" : "NO";
    cout << result;

    return 0;
}


