// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/lca
#include "common.hpp"
#include "../codes/Graph/LCA.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int n, q;
    cin >> n >> q;
    Tree t(n); // codebook 是 1-based，0 是超級源點：題目的點 i 對應 i+1
    t.add_edge(0, 1);
    for (int i = 1, p; i < n; i++) cin >> p, t.add_edge(p + 1, i + 1);
    t.build_LCA();
    while (q--) {
        int u, v;
        cin >> u >> v;
        cout << t.get_LCA(u + 1, v + 1) - 1 << '\n';
    }
}
