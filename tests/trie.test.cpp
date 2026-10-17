// competitive-verifier: STANDALONE
#include "common.hpp"
#include <cassert> // GCC 16 的 bits/stdc++.h 不含 assert
const int N = 62;
#include "../codes/Data-Structure/Trie.cpp"

// 62 位元最大 XOR 與暴力比對；同一個 tr 重複 init
signed main() {
    mt19937_64 rng(7122);
    for (int tc = 0; tc < 200; tc++) {
        int m = rng() % 50 + 1;
        tr.init(m * (N + 1) + 1);
        vector<int> v;
        for (int i = 0; i < m; i++) {
            int x = rng() >> 1;
            if (tc % 2) x &= 7 | (1LL << 62); // 共用前綴、只差高位
            tr.insert(x), v.push_back(x);
            int y = rng() >> 1, best = 0;
            for (int z : v) best = max(best, y ^ z);
            assert(tr.query(y) == best);
        }
    }
    puts("OK");
}
