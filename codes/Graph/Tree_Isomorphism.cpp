// 無根樹同構：0-based，兩棵樹同構 <=> tree_id 相同，O(n log n)
// 要比較的樹必須共用同一個 mp；有根樹直接用 rooted_id(g, root, -1)
map<vector<int>, int> mp;
int rooted_id(vector<vector<int>> &g, int u, int p){
    vector<int> v;
    for (int x : g[u]) if (x != p) v.push_back(rooted_id(g, x, u));
    sort(v.begin(), v.end());
    return mp.emplace(v, mp.size()).first->second;
}

// 以重心（1 或 2 個）為根，取最小的 id
int tree_id(vector<vector<int>> &g){
    int n = g.size(), res = INF;
    vector<int> sz(n), cen;
    auto dfs = [&](auto self, int u, int p) -> void {
        sz[u] = 1;
        int mx = 0;
        for (int x : g[u]) if (x != p){
            self(self, x, u);
            sz[u] += sz[x], mx = max(mx, sz[x]);
        }
        if (max(mx, n - sz[u]) * 2 <= n) cen.push_back(u);
    };
    dfs(dfs, 0, -1);
    for (int c : cen) res = min(res, rooted_id(g, c, -1));
    return res;
}
