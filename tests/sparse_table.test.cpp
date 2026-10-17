// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/staticrmq
#include "common.hpp"
#include "../codes/Data-Structure/Sparse_Table.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto &x : a) cin >> x;
    SparseTable st;
    st.build(vector<int>(n + 7, -1)); // 先用別的陣列 build 一次，確認重複 build 不會殘留舊資料
    st.build(a);
    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << st.query(l, r) << '\n';
    }
}
