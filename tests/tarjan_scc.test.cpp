// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/scc
#include "common.hpp"
#include "../codes/Graph/Tarjan_SCC.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int n, m;
    cin >> n >> m;
    tarjan_SCC g(n);
    for (int i = 0, u, v; i < m; i++) cin >> u >> v, g.add(u, v);
    g.build();
    vector<vector<int>> comp(g.now_SCCs);
    for (int i = 0; i < n; i++) comp[g.SCC[i]].push_back(i);
    // Tarjan 的 SCC 編號是反拓撲序
    cout << comp.size() << '\n';
    for (int i = comp.size() - 1; i >= 0; i--) {
        cout << comp[i].size();
        for (int v : comp[i]) cout << ' ' << v;
        cout << '\n';
    }
}
