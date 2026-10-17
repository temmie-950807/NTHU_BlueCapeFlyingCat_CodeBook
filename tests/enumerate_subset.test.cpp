// competitive-verifier: STANDALONE
#include <cassert>
#include "common.hpp"

// 片段只是一段迴圈、迴圈本體沒有掛勾，所以暫時把 int 換成會記錄的型別：
// s 由 mask 複製建構（開始枚舉新的 mask），之後 s=(s-1)&mask 走 operator=
vector<vector<long long>> seen;
struct Rec {
    long long v;
    Rec(long long v = 0) : v(v) {}
    Rec(const Rec &o) : v(o.v) { seen.push_back({v}); }
    Rec &operator=(const Rec &o) { v = o.v; seen.back().push_back(v); assert(seen.back().size() <= 1024); return *this; }
    Rec operator++(signed) { return Rec(v++); }
    friend Rec operator-(const Rec &a, const Rec &b) { return Rec(a.v - b.v); }
    friend Rec operator&(const Rec &a, const Rec &b) { return Rec(a.v & b.v); }
    friend bool operator<(const Rec &a, const Rec &b) { return a.v < b.v; }
    friend bool operator>=(const Rec &a, const Rec &b) { return a.v >= b.v; }
    friend bool operator==(const Rec &a, const Rec &b) { return a.v == b.v; }
};

void run(long long n) {
#undef int
#define int Rec
#include "../codes/Misc/Enumerate_Subset.cpp"
#undef int
#define int long long
}

signed main() {
    for (int n = 0; n <= 10; n++) {
        seen.clear();
        run(n);
        // 每個 mask 要依序（遞減）恰好枚舉到它所有的子集，最後是 0
        int cnt = 0;
        for (auto &v : seen) {
            assert(cnt < (1 << n));
            int mask = cnt++;
            vector<int> want;
            for (int s = mask; s >= 0; s--) if ((s & mask) == s) want.push_back(s);
            assert((vector<int>(v.begin(), v.end()) == want));
        }
        assert(cnt == (1 << n));
    }
    return 0;
}
