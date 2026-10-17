// competitive-verifier: STANDALONE
#include "common.hpp"
#include "../codes/Graph/prufer_reverse.cpp"

mt19937 rng(7122);

// O(n^2) 暴力解碼：每次拿目前編號最小的葉子接到序列的下一個數
vector<pair<int, int>> brute(vector<int> v) {
    int n = v.size() + 2;
    vector<int> deg(n + 1, 1);
    for (int x : v) deg[x]++;
    vector<pair<int, int>> res;
    for (int x : v) {
        int l = 1;
        while (deg[l] != 1) l++;
        res.emplace_back(l, x), deg[l]--, deg[x]--;
    }
    int a = 0, b = 0;
    for (int i = 1; i <= n; i++) if (deg[i] == 1) (a ? b : a) = i;
    res.emplace_back(a, b);
    return res;
}

// 編碼：每次拔掉編號最小的葉子，記下它的鄰居
vector<int> encode(int n, vector<pair<int, int>> E) {
    vector<set<int>> g(n + 1);
    for (auto [a, b] : E) g[a].insert(b), g[b].insert(a);
    vector<int> code;
    for (int k = 0; k < n - 2; k++) {
        int l = 1;
        while (g[l].size() != 1) l++;
        int p = *g[l].begin();
        code.push_back(p), g[p].erase(l), g[l].clear();
    }
    return code;
}

void check(vector<int> v) {
    int n = v.size() + 2;
    auto res = prufer_reverse(v);
    assert(res == brute(v));
    assert((int)res.size() == n - 1);
    vector<int> d(n + 1); iota(d.begin(), d.end(), 0); // DSU 檢查是樹
    function<int(int)> f = [&](int x) { return d[x] == x ? x : d[x] = f(d[x]); };
    for (auto [a, b] : res) assert(1 <= a && a <= n && 1 <= b && b <= n && f(a) != f(b)), d[f(a)] = f(b);
    assert(encode(n, res) == v);
}

signed main() {
    check({});
    for (int n = 3; n <= 7; n++) { // 窮舉所有序列，n^(n-2) 棵樹
        vector<int> v(n - 2, 1);
        while (true) {
            check(v);
            int i = 0;
            while (i < n - 2 && v[i] == n) v[i++] = 1;
            if (i == n - 2) break;
            v[i]++;
        }
    }
    for (int it = 0; it < 3000; it++) {
        int n = rng() % 60 + 2;
        vector<int> v(n - 2);
        for (auto &x : v) x = rng() % (it % 2 ? n : 3) + 1;
        check(v);
    }
    return 0;
}
