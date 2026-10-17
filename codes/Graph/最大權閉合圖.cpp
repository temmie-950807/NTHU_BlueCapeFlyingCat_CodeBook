// 邊 u → v 表示選 u 就要選 v (0-based)
// 回傳 {最大權（保證非負）, 選的點}
// 一般圖：O(n²m) / 二分圖且 |w_i| = 1：O(m√n)
template<typename U>
pair<U, vector<int>> maximum_closure(vector<U> w, vector<pair<int,int>> EV) {
    int n = w.size(), S = n + 1, T = n + 2;
    Dinic G(T + 5); // Graph/Dinic.cpp
    U sum = 0;
    for (int i = 0; i < n; ++i) {
        if (w[i] > 0) {
            G.add(S, i, w[i]);
            sum += w[i];
        }
        else if (w[i] < 0) {
            G.add(i, T, abs(w[i]));
        }
    }
    for (auto &[u, v] : EV) { // 請務必確保 INF > Σ|w_i|
        G.add(u, v, INF);
    }
    U cut = G.flow(S, T);
    vector<bool> vis(T + 5);
    G.dfs_mincut(S, vis);
    vector<int> ret;
    for (int i = 0; i < n; ++i) if (vis[i]) ret.push_back(i);
    return {sum - cut, ret};
}
