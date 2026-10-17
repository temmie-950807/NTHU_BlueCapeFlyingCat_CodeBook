// competitive-verifier: STANDALONE
#include "common.hpp"
// 檔案自帶 signed main() 且沒有 return，改名成 void 函式，透過 stringstream 餵輸入
#define signed void
#define main knapsack_main
#include "../codes/Dynamic-Programming/Knaspack_On_Tree.cpp"
#undef main
#undef signed

mt19937 rng(7122);
int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }

signed main() {
    for (int it = 0; it < 3000; it++) {
        int n = rnd(1, 12), Wc = rnd(0, 30);
        vector<int> P(n + 1), ww(n + 1), vv(n + 1);
        for (int i = 1; i <= n; i++) P[i] = rnd(0, i - 1), ww[i] = rnd(0, 10), vv[i] = rnd(0, 100);
        // 森林亂序：重新編號讓父節點編號不一定比較小
        vector<int> perm(n + 1);
        iota(perm.begin(), perm.end(), 0);
        shuffle(perm.begin() + 1, perm.end(), rng);
        vector<int> p2(n + 1), w2(n + 1), v2(n + 1);
        for (int i = 1; i <= n; i++) p2[perm[i]] = perm[P[i]], w2[perm[i]] = ww[i], v2[perm[i]] = vv[i];
        stringstream in, out;
        in << n << ' ' << Wc << '\n';
        for (int i = 1; i <= n; i++) in << p2[i] << ' ';
        for (int i = 1; i <= n; i++) in << w2[i] << ' ';
        for (int i = 1; i <= n; i++) in << v2[i] << ' ';
        adj.clear(), cur = 0; // 全域狀態：原程式只會跑一次
        auto ib = cin.rdbuf(in.rdbuf()), ob = cout.rdbuf(out.rdbuf());
        knapsack_main();
        cin.rdbuf(ib), cout.rdbuf(ob);
        int got; out >> got;
        // 暴力：選的點集合要對父節點封閉（父節點 0 是虛擬根）
        int best = 0;
        for (int s = 0; s < (1 << n); s++) {
            int sw = 0, sv = 0; bool ok = true;
            for (int i = 1; i <= n && ok; i++) if (s >> (i - 1) & 1) {
                if (p2[i] && !(s >> (p2[i] - 1) & 1)) ok = false;
                sw += w2[i], sv += v2[i];
            }
            if (ok && sw <= Wc) best = max(best, sv);
        }
        assert(got == best);
    }
    return 0;
}
