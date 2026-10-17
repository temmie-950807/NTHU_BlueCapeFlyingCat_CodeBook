// competitive-verifier: STANDALONE
#include "common.hpp"
#include "../codes/Graph/Tree_Isomorphism.cpp"

mt19937 rng(7122);
vector<vector<int>> random_tree(int n) {
    vector<vector<int>> g(n);
    for (int i = 1; i < n; i++) {
        int p = rng() % i;
        g[i].push_back(p), g[p].push_back(i);
    }
    return g;
}
vector<vector<int>> relabel(const vector<vector<int>> &g) {
    int n = g.size();
    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 0);
    shuffle(perm.begin(), perm.end(), rng);
    vector<vector<int>> h(n);
    for (int u = 0; u < n; u++) for (int v : g[u]) h[perm[u]].push_back(perm[v]);
    for (auto &a : h) shuffle(a.begin(), a.end(), rng);
    return h;
}
bool brute_iso(const vector<vector<int>> &a, const vector<vector<int>> &b) {
    int n = a.size();
    set<pair<int, int>> eb;
    for (int u = 0; u < n; u++) for (int v : b[u]) eb.insert({u, v});
    vector<int> p(n);
    iota(p.begin(), p.end(), 0);
    do {
        bool ok = true;
        for (int u = 0; u < n && ok; u++) for (int v : a[u]) if (!eb.count({p[u], p[v]})) { ok = false; break; }
        if (ok) return true;
    } while (next_permutation(p.begin(), p.end()));
    return false;
}

signed main() {
    for (int it = 0; it < 3000; it++) {
        int n = rng() % 8 + 1;
        auto a = random_tree(n);
        auto b = it % 2 ? relabel(a) : random_tree(n);
        assert((tree_id(a) == tree_id(b)) == brute_iso(a, b));
    }
    // 兩個重心的情況：長度 6 的鏈，從不同端點編號
    vector<vector<int>> c1(6), c2(6);
    for (int i = 0; i + 1 < 6; i++) c1[i].push_back(i + 1), c1[i + 1].push_back(i);
    c2 = relabel(c1);
    assert(tree_id(c1) == tree_id(c2));
    // 大一點的樹：relabel 後要相同，接一個葉子後要不同
    auto big = random_tree(50000);
    auto big2 = relabel(big);
    assert(tree_id(big) == tree_id(big2));
    big2.push_back({0}), big2[0].push_back(50000);
    auto big3 = big;
    big3.push_back({1}), big3[1].push_back(50000);
    // 兩棵 50001 點的樹可能同構，只檢查和原本 50000 點的不同
    assert(tree_id(big3) != tree_id(big));
    return 0;
}
