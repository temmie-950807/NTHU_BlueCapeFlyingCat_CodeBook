// competitive-verifier: STANDALONE
#include "common.hpp"
#include <cassert>
#include "../codes/String/Suffix_Automata.cpp"

SAM sam; // 約 41MB，必須是全域

// 同一個物件重複 build（長字串 -> 短字串），每次都對拍暴力：
// 子字串是否存在、cnt（出現次數）、fpos（第一次出現的結尾）、相異子字串數
signed main() {
    mt19937 rd(7122);
    for (int iter=0 ; iter<300 ; iter++){
        int n = (iter%2 ? rd()%8+1 : rd()%60+1), k = (iter%2 ? 2 : 26);
        string s;
        for (int i=0 ; i<n ; i++) s += 'a'+rd()%k;
        sam.build(s);
        set<string> all;
        for (int l=0 ; l<n ; l++){
            for (int r=l ; r<n ; r++){
                string p = s.substr(l, r-l+1);
                all.insert(p);
                int v = 0;
                for (char c : p) v = sam.ch[v][c-'a'], assert(v);
                int occ = 0;
                for (int i=0 ; i+p.size()<=n ; i++) occ += s.compare(i, p.size(), p)==0;
                assert(sam.cnt[v]==occ);
                assert(sam.fpos[v]-(int)p.size()==(int)s.find(p));
            }
        }
        for (int t=0 ; t<100 ; t++){ // 不存在的字串必須走不到
            string p;
            int m = rd()%6+1;
            for (int i=0 ; i<m ; i++) p += 'a'+rd()%26;
            int v = 0;
            for (char c : p) if (~v) v = sam.ch[v][c-'a'] ? sam.ch[v][c-'a'] : -1;
            assert((v!=-1)==(s.find(p)!=string::npos));
        }
        int distinct = 0;
        for (int v=1 ; v<sam.sz ; v++) distinct += sam.len[v]-sam.len[sam.link[v]];
        assert(distinct==all.size());
    }
    return 0;
}
