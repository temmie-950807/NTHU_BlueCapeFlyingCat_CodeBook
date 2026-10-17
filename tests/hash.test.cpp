// competitive-verifier: STANDALONE
#include "common.hpp"
#include <cassert>
#include "../codes/String/Hash.cpp"

// get(l, r) 對拍直接用 __int128 算的 sum s[i]*A^(r-i) mod B，並檢查相等子字串的 hash 相等
signed main() {
    mt19937 rd(7122);
    for (int iter=0 ; iter<200 ; iter++){
        A = (iter%4==0 ? B-1 : iter%4==1 ? B-2 : 100000+rd()%1000000007); // 也測 A 靠近 2^61 的情況
        int n = rd()%50+1;
        string s;
        for (int i=0 ; i<n ; i++) s += (iter%2 ? 'a'+rd()%2 : 32+rd()%95);
        RollingHash H(s);
        for (int l=0 ; l<n ; l++){
            __int128 v = 0;
            for (int r=l ; r<n ; r++){
                v = (v*A+s[r])%B;
                assert(H.get(l, r)==(int)v);
            }
        }
        for (int t=0 ; t<300 && iter%4>=2 ; t++){ // A=B-1 本來就容易碰撞，只測隨機 A
            int len = rd()%n+1, l1 = rd()%(n-len+1), l2 = rd()%(n-len+1);
            bool eq = s.compare(l1, len, s, l2, len)==0;
            assert(eq==(H.get(l1, l1+len-1)==H.get(l2, l2+len-1)));
        }
    }
    return 0;
}
