// ДО отложенные операции
#include <iostream>
#include <vector>
#include <cmath>
using std::cin;
using std::cout;
using std::vector;
using std::pair;

const unsigned int E = 1000;
const unsigned int INF = 1u << 30u;

class SegmentTree {
public:
    explicit SegmentTree(vector<int>& other);
    
    SegmentTree(const SegmentTree&) = delete;
    SegmentTree(SegmentTree&&) = delete;
    SegmentTree& operator=(const SegmentTree&) = delete;
    SegmentTree& operator=(SegmentTree&&) = delete;

    void increaseSegment(int query_l , int query_r, int add);
    int getMin( int query_l , int query_r);

private:
    /*
     * tl - левая граница отрезка контролируемая вершиной current_index (левая граница поддерева с корнем current_index)
     * tr - правая граница отрезка контролируемая вершиной current_index (правая граница поддерева с корнем current_index)
     * query_l - левая граница текущего запроса
     * query_x - правая граница текущего запроса
     * value - значения для присваивания на отрезке
     */
    void increaseSegmentHelp(int current_index, int tl, int tr, int query_l , int query_r, int value);
    int getMinHelp(int current_index, int tl, int tr, int query_l , int query_r);
    void propogate(int current_index, int tl, int tr);

    vector<int> tree;
    vector<int> changes;
};


void SegmentTree::propogate(int current_index, int tl, int tr) {
    if(tr == tl)
        return;
    if(changes[current_index] == E)
        return;
    for(int i = 1; i <=2; ++i) {
        changes[current_index * 2 + i] = changes[current_index];
        tree[current_index * 2 + i] = changes[current_index];
    }
    changes[current_index] = E;
}

SegmentTree::SegmentTree(vector<int>& other) {
    size_t new_size = 1ull << (size_t)ceil(std::log2((double)other.size()));
    changes.assign(new_size * 2 - 1, E);
    tree.assign(new_size * 2 - 1, INF);
    for(int i = 0; i < other.size(); ++i) {
        tree[new_size - 1 + i] = other[i];
    }

    for(int i = (int)new_size - 2;i >= 0; --i) {
        tree[i] = std::min(tree[i * 2 + 1], tree[i * 2 + 2]);
    }
}

int SegmentTree::getMin(int query_l, int query_r) {
    return getMinHelp(0, 0, tree.size() / 2, query_l, query_r);
}

void SegmentTree::increaseSegment(int query_l, int query_r, int add) {
    increaseSegmentHelp(0, 0, tree.size() / 2, query_l, query_r, add);
}


void SegmentTree::increaseSegmentHelp(int current_index, int tl, int tr, int query_l , int query_r, int value) {

    propogate(current_index, tl, tr);
    if (query_l > query_r || tl > tr)
        return;

    if(tl == query_l && tr == query_r) {
        changes[current_index] = value;
        tree[current_index] = value;
        return;
    }

    int tm = (tl + tr) / 2;
    increaseSegmentHelp(current_index * 2 + 1, tl, tm, query_l, std::min(query_r, tm), value);
    increaseSegmentHelp(current_index * 2 + 2,tm + 1, tr, std::max(tm + 1, query_l), query_r, value);

    tree[current_index] = std::min(tree[current_index * 2 + 1], tree[current_index * 2 + 2]);
}

int SegmentTree::getMinHelp(int current_index, int tl, int tr, int query_l , int query_r) {

    propogate(current_index, tl, tr);
    if (query_l > query_r || tl > tr)
        return INF;
    if(tl == query_l && tr == query_r) {
        return tree[current_index];
    }

    int tm = (tl + tr) / 2;
    int min_left = getMinHelp(current_index * 2 + 1, tl, tm, query_l, std::min(query_r, tm));
    int min_right = getMinHelp(current_index * 2 + 2,tm + 1, tr,
                               std::max(tm + 1, query_l), query_r);

    return std::min(min_left, min_right);
}


int main() {
    int n = 0;
    cin >> n;

    vector<int> colourSequence(n, 0);
    int r = 0, g = 0, b = 0;
    for(int i = 0; i < n; ++i) {
        cin >> r >> g >> b;
        colourSequence[i] = r + g + b;
    }

    int query_amount = 0;
    cin >> query_amount;
    SegmentTree myTree(colourSequence);

    int l_min = 0, r_min = 0, l_set = 0, r_set = 0;
    for(int i = 0; i < query_amount; ++i) {
        cin >> l_set >> r_set >> r >> g >> b >> l_min >> r_min;
        myTree.increaseSegment(l_set , r_set, r + g + b);
        cout << myTree.getMin(l_min, r_min) << " ";
    }

    return 0;
}


