// Sparse Table
#include <iostream>
#include <vector>
#include <cmath>
using std::cin;
using std::cout;
using std::vector;

const unsigned int INF = 1u << 30u;

struct Minimum {
    Minimum(int min_value, int min_position) : value(min_value), position(min_position) {}
    int value;
    int position;
};

class SparseTable {
public:
    explicit SparseTable(const vector<int>& array);
    
    SparseTable(const SparseTable&) = delete;
    SparseTable(SparseTable&&) = delete;
    SparseTable& operator=(const SparseTable&) = delete;
    SparseTable& operator=(SparseTable&&) = delete;

    int getSecondMin(int query_l, int query_r) const;
    
private:
    vector<vector<Minimum>> sparseTable;
    vector<unsigned int> logarithms;
    Minimum getMinPair(Minimum segment_1, Minimum segment_2) const;
    int getFirstMin(int query_l , int query_r) const;
};

SparseTable::SparseTable (const vector<int>& array) :
        sparseTable(ceil(log2((double)array.size())), vector<Minimum> (array.size(), Minimum(0, 0))) {


    for(int i = 0; i < array.size() + 1; ++i)
        logarithms.emplace_back((unsigned)ceil(log2((double)i)));

    for(int i = 0; i < array.size(); ++i)
        sparseTable[0][i] = Minimum(array[i], i);

    for(size_t k = 1; k < ceil(log2((double)array.size())) ; ++k) {
        for(size_t i = 0; i < array.size() - (1u << k) + 1; ++i) {
            sparseTable[k][i] = getMinPair(sparseTable[k - 1][i], sparseTable[k - 1][i + (1u << (k - 1))]);
        }
    }
}

Minimum SparseTable::getMinPair(Minimum segment_1, Minimum segment_2) const {
    return segment_1.value <= segment_2.value ? segment_1 : segment_2;
}


int SparseTable::getSecondMin(int query_l, int query_r) const {
    size_t k = logarithms[query_r - query_l + 1] - 1;

    int first_min_pos = getMinPair(sparseTable[k][query_l], sparseTable[k][query_r - (1u << k) + 1]).position;
    return std::min(getFirstMin(query_l, first_min_pos - 1), getFirstMin(first_min_pos + 1, query_r));
}

int SparseTable::getFirstMin(int query_l, int query_r) const {
    if(query_l == query_r)
        return sparseTable[0][query_l].value;
    if (query_l > query_r )
        return INF;
    size_t k = logarithms[query_r - query_l + 1] - 1;
    return getMinPair(sparseTable[k][query_l], sparseTable[k][query_r - (1u << k) + 1]).value;
}


int main() {
    int n = 0, m = 0;
    cin >> n >> m;

    vector<int> array(n);
    for(int i = 0; i < n; ++i)
        cin >>  array[i];

    SparseTable myTable(array);

    int query_l = 0, query_r = 0;
    for (int i = 0; i < m; i++) {
        cin >> query_l >> query_r;
        cout << myTable.getSecondMin(query_l - 1, query_r - 1) << std::endl;
    }

    return 0;
}


