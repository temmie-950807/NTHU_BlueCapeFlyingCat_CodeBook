// competitive-verifier: STANDALONE
#include "common.hpp"
#include <cassert>
#include "../codes/Data-Structure/Sparse_Table.cpp"
#include "../codes/String/Suffix_Array.cpp"

// get_lcp 全部 pair（含 x==y）、區間 lcp、substring_cmp 對拍暴力
signed main() {
    mt19937 rd(7122);
    for (int iter=0 ; iter<300 ; iter++){
        int n = rd()%40+1;
        string s;
        for (int i=0 ; i<n ; i++) s += 'a'+rd()%(iter%3+1);
        SuffixArray SA(s);
        SA.init_lcp();
        auto lcp = [&](int i, int j){ int k = 0; while (i+k<n && j+k<n && s[i+k]==s[j+k]) k++; return k; };
        for (int x=0 ; x<n ; x++) for (int y=0 ; y<n ; y++)
            assert(SA.get_lcp(x, y)==lcp(SA.sa[x], SA.sa[y]));
        for (int t=0 ; t<200 ; t++){
            int l1 = rd()%n, r1 = l1+rd()%(n-l1), l2 = rd()%n, r2 = l2+rd()%(n-l2);
            int len1 = r1-l1+1, len2 = r2-l2+1;
            assert(SA.get_lcp(l1, r1, l2, r2)==min({lcp(l1, l2), len1, len2}));
            int c = s.substr(l1, len1).compare(s.substr(l2, len2)), d = SA.substring_cmp(l1, r1, l2, r2);
            assert((c<0)==(d<0) && (c==0)==(d==0));
        }
    }
    return 0;
}
