// 邊 u → v 表示選 u 就要選 v (0-based)
// 保證回傳值非負
// 構造：呼叫 construct，選那些 vis 是 true 的點。
// 一般圖：O(n²m) / 二分圖：O(m√n)
template<typename U>
U maximum_closure(vector<U> w, vector<pair<int,int>> EV) {
    int n = w.size(), S = n + 1, T = n + 2;
    Flow G(T + 5); // Graph/Dinic.cpp
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
    return sum - cut;
}
