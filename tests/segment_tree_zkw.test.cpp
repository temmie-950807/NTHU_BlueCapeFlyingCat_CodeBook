// competitive-verifier: STANDALONE
#include "common.hpp"
// 檔案需要外部的 mod 與 hmul_pow[len] = h^len
const int mod = 1e9 + 7, H = 131;
vector<int> hmul_pow;
#include "../codes/Data-Structure/Segment_Tree_Zkw.cpp"

// 區間乘、區間 hash（sum v_j * H^(j-l)），1-based，與暴力比對；含非 2 的冪的 n
signed main() {
    mt19937_64 rng(7122);
    auto rnd = [&](int l, int r) { return (int)(rng() % (r - l + 1)) + l; };
    for (int it = 0; it < 400; it++) {
        int n = it < 20 ? it + 1 : rnd(1, it % 4 ? 70 : 3000);
        hmul_pow.assign(n + 1, 1);
        for (int i = 1; i <= n; i++) hmul_pow[i] = hmul_pow[i - 1] * H % mod;
        vector<int> v(n + 1);
        vector<SegmentTreeZkw::T> init(n + 1);
        for (int i = 1; i <= n; i++) v[i] = rnd(0, mod - 1), init[i] = {1, v[i]};
        SegmentTreeZkw st(n);
        st.build(init.data());
        for (int op = 0; op < 200; op++) {
            int l = rnd(1, n), r = rnd(l, n);
            if (op % 13 == 0) l = 1, r = n;
            if (rng() % 2) {
                int val = op % 5 ? rnd(0, mod - 1) : rnd(0, 2);
                st.range_update(l, r, val);
                for (int i = l; i <= r; i++) v[i] = v[i] * val % mod;
            } else {
                int h = 0;
                for (int i = r; i >= l; i--) h = (h * H + v[i]) % mod;
                auto res = st.query(l, r);
                assert(res[0] == r - l + 1 && res[1] == h);
            }
        }
    }
    puts("OK");
    return 0;
}
