// competitive-verifier: STANDALONE
#include "common.hpp"
#include "../codes/String/KMP.cpp"
#include "../codes/String/Min_Rotation.cpp"

// KMP 失敗函數與 p+'#'+t 匹配、minRotation 對拍暴力（含全同、週期、Fibonacci 字串）
signed main() {
    mt19937 rd(7122);
    vector<string> tests = {"", "a", "aaaaaa", "abababab", "abaababaab", "abcabcabcab"};
    string f0 = "a", f1 = "ab";
    while (f1.size() < 60) { string f2 = f1 + f0; f0 = f1; f1 = f2; tests.push_back(f1); }
    for (int it=0 ; it<3000 ; it++){
        int n = rd()%30+1, k = rd()%3+1;
        string s;
        if (it%4==0){ string b; for (int i=0, m=rd()%4+1 ; i<m ; i++) b += 'a'+rd()%k; while ((int)s.size()<n) s += b; }
        else for (int i=0 ; i<n ; i++) s += 'a'+rd()%k;
        tests.push_back(s);
    }
    for (auto &s : tests){
        int n = s.size();
        vector<int> f = KMP(s);
        assert((int)f.size()==n);
        for (int i=0 ; i<n ; i++){
            int b = 0;
            for (int L=1 ; L<=i ; L++) if (s.substr(0, L)==s.substr(i-L+1, L)) b = L;
            assert(f[i]==b);
        }
        // 匹配：模式 p 為 s 的隨機子字串或隨機字串
        for (int t=0 ; t<3 && n ; t++){
            string p;
            if (t==0){ int l = rd()%n, len = rd()%min(4LL, n-l)+1; p = s.substr(l, len); }
            else for (int i=0, m=rd()%3+1 ; i<m ; i++) p += 'a'+rd()%2;
            string c = p + '#' + s;
            vector<int> g = KMP(c);
            vector<int> got, exp;
            for (int i=0 ; i<(int)c.size() ; i++) if (g[i]==(int)p.size()) got.push_back(i-2*(int)p.size());
            for (int i=0 ; i+(int)p.size()<=n ; i++) if (s.compare(i, p.size(), p)==0) exp.push_back(i);
            assert(got==exp);
        }
        // minRotation：回傳最小旋轉的起點，且是最小的那個起點
        int a = minRotation(s);
        string best = s; int bi = 0;
        for (int i=0 ; i<n ; i++){ string r = s.substr(i)+s.substr(0, i); if (r<best) best = r, bi = i; }
        assert(0<=a && (a<n || n==0));
        assert(n==0 || s.substr(a)+s.substr(0, a)==best); assert(a==bi);
    }
    return 0;
}
