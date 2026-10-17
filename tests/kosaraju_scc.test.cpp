// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/scc
#include "common.hpp"
#include "../codes/Graph/Kosaraju_SCC.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int n, m;
    cin >> n >> m;
    SCC_compress g(n);
    for (int i = 0, u, v; i < m; i++) cin >> u >> v, g.add_edge(u, v);
    g.compress();
    // Kosaraju 的 SCC 編號就是拓撲序
    cout << g.sz << '\n';
    for (auto &c : g.SCC) {
        cout << c.size();
        for (int v : c) cout << ' ' << v;
        cout << '\n';
    }
}
