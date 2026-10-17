// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/dominatortree
#include "common.hpp"
#include "../codes/Graph/Dominator_Tree.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int n, m, s;
    cin >> n >> m >> s;
    DominatorTree DT(n, s);
    for (int i = 0, u, v; i < m; i++) cin >> u >> v, DT.add_edge(u, v);
    DT.build();
    for (int i = 0; i < n; i++) cout << DT[i] << " \n"[i == n - 1];
}
