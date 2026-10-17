// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/two_edge_connected_components
#include "common.hpp"
#include "../codes/Graph/EdgeBCC.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int n, m;
    cin >> n >> m;
    EdgeBCC g(n);
    for (int i = 0, u, v; i < m; i++) cin >> u >> v, g.add_edge(u, v);
    g.get_bcc();
    cout << g.sz << '\n';
    for (int i = 0; i < g.sz; i++) {
        cout << g.bcc[i].size();
        for (int v : g.bcc[i]) cout << ' ' << v;
        cout << '\n';
    }
}
