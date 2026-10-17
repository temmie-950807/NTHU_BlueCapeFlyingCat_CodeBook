// competitive-verifier: STANDALONE
#include "common.hpp"
#include "../codes/Data-Structure/Sparse_Table.cpp"
#include "../codes/String/Suffix_Array.cpp"
#include "../codes/String/Enumerate_Runs.cpp"

// enumerate_run 對拍暴力：所有 (最小週期 t, l, r)，[l, r) 為極大重複且 r-l >= 2t，輸出依 (t, l) 排序
vector<array<int, 3>> brute(const string &s){
    int n = s.size();
    vector<array<int, 3>> res;
    for (int l=0 ; l<n ; l++) for (int r=l+1 ; r<=n ; r++){
        int p = 1;
        while (p<r-l){ bool ok = true; for (int i=l ; i+p<r ; i++) if (s[i]!=s[i+p]){ ok = false; break; } if (ok) break; p++; }
        if (r-l<2*p) continue;
        if (l>0 && s[l-1]==s[l-1+p]) continue;
        if (r<n && s[r]==s[r-p]) continue;
        res.push_back({p, l, r});
    }
    sort(res.begin(), res.end());
    return res;
}

signed main() {
    mt19937 rd(7122);
    vector<string> tests = {"a", "aa", "ab", "aaaaaaa", "abababa", "abaababaabaab", "mississippi"};
    string f0 = "a", f1 = "ab";
    while (f1.size() < 80) { string f2 = f1 + f0; f0 = f1; f1 = f2; tests.push_back(f1); }
    for (int it=0 ; it<3000 ; it++){
        int n = rd()%30+1, k = rd()%3+1;
        string s;
        if (it%4==0){ string b; for (int i=0, m=rd()%4+1 ; i<m ; i++) b += 'a'+rd()%k; while ((int)s.size()<n) s += b; s += 'a'+rd()%k; }
        else for (int i=0 ; i<n ; i++) s += 'a'+rd()%k;
        tests.push_back(s);
    }
    for (auto &s : tests){
        auto got = enumerate_run(s), exp = brute(s);
        assert(got==exp);
    }
    return 0;
}
