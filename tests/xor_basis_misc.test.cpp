// competitive-verifier: STANDALONE
#include "common.hpp"
#include "../codes/Misc/Xor_Basis.cpp"
#include "../codes/Misc/random_int.cpp"

// 用 recorder 取代 bitset，記下 solve<> 最後實際開的長度
int chosen = -1;
template <size_t L> struct recorder { recorder() { chosen = L; } };
#define bitset recorder
#include "../codes/Misc/Dynamic_Bitset.cpp"
#undef bitset

// Xor_Basis 對拍暴力張成空間（60-bit 值）、random_int 範圍與分布、Dynamic_Bitset 選到的長度
signed main() {
    mt19937_64 rd(7122);
    for (int it=0 ; it<3000 ; it++){
        basis.clear();
        int n = rd()%12+1, bits = it%3==0 ? 4 : (it%3==1 ? 10 : 60);
        set<int> span = {0};
        vector<int> vals;
        for (int i=0 ; i<n ; i++){
            int x = rd()>>(64-bits);
            if (rd()%3==0 && vals.size()) x = vals[rd()%vals.size()]^vals[rd()%vals.size()]; // 線性相依
            vals.push_back(x);
            add_vector(x);
            set<int> nxt = span;
            for (int v : span) nxt.insert(v^x);
            span = nxt;
            assert((int)span.size()==(1LL<<basis.size()));
            assert(get_max()==*span.rbegin());
        }
        for (int v : span) assert(check(v));
        for (int t=0 ; t<20 ; t++){ int x = rd()>>(64-bits); assert(check(x)==(bool)span.count(x)); }
    }

    seed.seed(7122);
    for (int t=0 ; t<1000 ; t++) assert(rng(5, 5)==5);
    map<int, int> cnt;
    for (int t=0 ; t<70000 ; t++){ int x = rng(-3, 3); assert(-3<=x && x<=3); cnt[x]++; }
    assert(cnt.size()==7);
    for (auto [k, c] : cnt) assert(abs(c-10000)<600);
    bool hi = false, lo = false;
    for (int t=0 ; t<200 ; t++){ int x = rng(LLONG_MIN, LLONG_MAX); hi |= x>(1LL<<62); lo |= x<-(1LL<<62); }
    assert(hi && lo);

    for (int n : {0LL, 1LL, 2LL, 3LL, 1000LL, 1024LL, 1025LL, 131072LL, 131073LL, 200000LL, MAXN}){
        solve<>(n);
        assert(chosen>=n && (chosen==1 || chosen==MAXN || chosen<2*max(n, 1LL)));
    }
    return 0;
}
