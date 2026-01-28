/*
Tested : https://cses.fi/paste/09f2afa2bfd9b6a5e92eb8/
         https://codeforces.com/gym/102835/submission/360263301
         https://codeforces.com/contest/1986/submission/36026541
Author : std_abs
*/
struct BCC_AP { // 0-based, remember to build, 不支援重邊
    int n, nbcc; // 注意孤點不會有任何方點連接
    vector<vector<int>> E, F; // id >= n: 方點
    vector<int> pa, dep, low, stk, paf, depf;
    void dfs(int v, int p) {
        dep[v] = low[v] = ~p ? dep[p] + 1 : 0;
        stk.push_back(v), pa[v] = p;
        for (auto& u : E[v]) if (u != p) {
            if (low[u] == -1) {
                dfs(u, v), low[v] = min(low[v], low[u]);
                if (low[u] >= dep[v]) {
                    int id = nbcc++, x;
                    do {
                        x = stk.back(), stk.pop_back();
                        F[id + n].push_back(x), F[x].push_back(id + n);
                    } while (x != u);
                    F[id + n].push_back(v), F[v].push_back(id + n);
                }
            } else low[v] = min(low[v], dep[u]);
        }
    }
    bool is_bridge(int u, int v) {
        return (F[bcc_id(u, v) + n].size() == 2); }
    // 判斷原圖上的一個點是不是割點
    bool is_cut(int x) { return F[x].size() != 1; }
    // 回傳第 id 個 bcc 的每個點 (0-based)
    vector<int> bcc(int id) { return F[id + n]; }
    // 對於邊 (u, v) 回傳包含它的 bcc id（每條邊都恰好被一個 bcc 包含）
    int bcc_id(int u, int v) { // starts from 0
        return paf[depf[u] < depf[v] ? v : u] - n; }
    // 計算在新圖上的 depf, paf 陣列，呼叫 bcc_id() 的時候會用到
    void dfs2(int v, int p) {
        depf[v] = ~p ? depf[p] + 1 : 0, paf[v] = p;
        for (int u : F[v]) if (u != p) dfs2(u, v);
    }
    void build() {
        low.assign(n, -1);
        for (int i = 0; i < n; ++i) if (low[i] == -1)
            dfs(i, -1), dfs2(i, -1);
    }
    void add_edge(int u, int v) {
        E[u].push_back(v), E[v].push_back(u);
    }
    BCC_AP (int _n) : n(_n), nbcc(0), E(n), F(2 * n), pa(n), dep(n), low(n), stk(), paf(n * 2), depf(n * 2) {}
};
