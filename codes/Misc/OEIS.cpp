/*Tested: https://judge.yosupo.jp/submission/318106 (mod version)
Write by: kactl*/
// 若一個線性遞迴有 k 項，給他恰好 2*k 個項可以求出線性遞迴
// s = 1 1 3 7 17, 則它會回傳 1 2，代表 s[i] = 1*s[i-2] + 2*s[i-1]（跟 OEIS.py 順序相反）
vector<int> BerlekampMassey(vector<int> s) {
    if (s.empty()) return vector<int>();
    int n = s.size(), L = 0, m = 0;
    vector<int> C(n), B(n), T;
    C[0] = B[0] = 1;

    int b = 1;
    for (int i=0 ; i<n ; i++) {
        ++m; int d = s[i]%MOD;
        for (int j=1 ; j<L+1 ; j++) d = (d+C[j]*s[i-j])%MOD;
        if (d<0) d += MOD;
        if (!d) continue;
        T = C; int coef = d*qp(b, MOD-2, MOD)%MOD;
        for (int j=m ; j<n ; j++) C[j] = (C[j]-coef*B[j-m])%MOD;
        if (2*L>i) continue;
        L = i+1-L; B=T; b=d; m=0;
    }
    C.resize(L+1); C.erase(C.begin());
    for (int &x : C) x = (MOD-x)%MOD;
    reverse(C.begin(), C.end());
    return C;
}