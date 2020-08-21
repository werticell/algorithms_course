// ДО (прибавление на отрезке)
#include <iostream>
#include <vector>
#include <cmath>
using std::cin;
using std::cout;
using std::vector;
using std::pair;

class SegmentTree {
public:
    explicit SegmentTree(vector<int>& array);

    SegmentTree(const SegmentTree&) = delete;
    SegmentTree(SegmentTree&&) = delete;
    SegmentTree& operator=(const SegmentTree&) = delete;
    SegmentTree& operator=(SegmentTree&&) = delete;

    void increaseSegment(int query_l , int query_r, int add);
    int getMax(int query_l , int query_r) const;

private:
    /*
     * tl - левая граница отрезка контролируемая вершиной current_index (левая граница поддерева с корнем current_index)
     * tr - правая граница отрезка контролируемая вершиной current_index (правая граница поддерева с корнем current_index)
     * query_l - левая граница текущего запроса
     * query_x - правая граница текущего запроса
     * add - величина для прибавления на отрезке
     */
    void increaseSegmentHelp(int current_index, int tl, int tr, int query_l, int query_r, int add);
    int getMaxHelp(int current_index, int tl, int tr, int query_l, int query_r) const;

    vector<pair<int, int>> tree;
};

SegmentTree::SegmentTree(vector<int>& array) {
    size_t new_size = 1ull << (size_t)ceil(std::log2((double)array.size()));
    tree.assign(new_size * 2 - 1, pair<int, int>(0, 0));
    for(int i = 0; i < array.size(); ++i) {
        tree[new_size - 1 + i].first = array[i];
    }

    for(int i = (int)new_size - 2;i >= 0; --i) {
        tree[i].first = std::max(tree[i * 2 + 1].first, tree[i * 2 + 2].first);
    }
}

int SegmentTree::getMax(int query_l, int query_r) const {
    return getMaxHelp(0, 0, tree.size() / 2, query_l, query_r);
}

void SegmentTree::increaseSegment(int query_l, int query_r, int add) {
    increaseSegmentHelp(0, 0, tree.size() / 2, query_l, query_r, add);
}


void SegmentTree::increaseSegmentHelp(int current_index, int tl, int tr, int query_l , int query_r, int add) {

    if (query_l > query_r || tl > tr)
        return;

    if(tl == query_l && tr == query_r) {
        tree[current_index].second += add;
        return;
    }

    int tm = (tl + tr) / 2;
    increaseSegmentHelp(current_index * 2 + 1, tl, tm, query_l, std::min(query_r, tm), add);
    increaseSegmentHelp(current_index * 2 + 2,tm + 1, tr, std::max(tm + 1, query_l), query_r, add);

    tree[current_index].first = std::max(tree[current_index * 2 + 1].first + tree[current_index * 2 + 1].second,
                                         tree[current_index * 2 + 2].first + tree[current_index * 2 + 2].second);
}

int SegmentTree::getMaxHelp(int current_index, int tl, int tr, int query_l , int query_r) const {

    if (query_l > query_r || tl > tr)
        return 0 ;
    if(tl == query_l && tr == query_r) {
        return tree[current_index].first + tree[current_index].second;
    }

    int tm = (tl + tr) / 2;
    int max_left = getMaxHelp(current_index * 2 + 1, tl, tm, query_l, std::min(query_r, tm));
    int max_right = getMaxHelp(current_index * 2 + 2,tm + 1, tr,
                               std::max(tm + 1, query_l), query_r);

    return std::max(max_left, max_right) + tree[current_index].second;
}


int main() {
    int n = 0, train_size = 0;
    cin >> n;

    vector<int> ticketsSold(n - 1, 0);
    for(int i = 0; i < n - 1; ++i)
        cin >> ticketsSold[i];

    cin >> train_size;
    int query_amount = 0;
    cin >> query_amount;
    SegmentTree myTree(ticketsSold);

    int l = 0, r = 0, tickets_count = 0;
    for(int i = 0; i < query_amount; ++i) {
        cin >> l >> r >> tickets_count;

        if(myTree.getMax(l, r - 1) + tickets_count <= train_size)
            myTree.increaseSegment(l , r - 1, tickets_count);
        else
            cout << i << " ";
    }

    return 0;
}


