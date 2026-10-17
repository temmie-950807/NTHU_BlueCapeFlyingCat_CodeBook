/*Tested: https://judge.yosupo.jp/submission/319687
Write by: temmie*/
const int MOD = (119 << 23) + 1, ROOT = 62; // = 998244353
// For p < 2^30 there is also e.g. 5 << 25, 7 << 26, 479 << 21
// and 483 << 21 (same root). The last two are > 10^9.

// 76e5d5
vector<int> rt(2, 1);
void NTT_init(int n){
    static int lastInit = 0;
    if (lastInit<n){
        lastInit = n;
        for (int k=2, s=2 ; k<n ; k*=2, s++){
            rt.resize(n);
            int z[] = {1, qp(ROOT, MOD>>s, MOD)};
            for (int i=k ; i<2*k ; i++) rt[i] = rt[i/2]*z[i&1]%MOD;
        }
    }
}

// fec4b8
void NTT(vector<int> &a, int isInverse = false) {
    int n = a.size();
    int L = 31-__builtin_clz(n);
    NTT_init(n);

    vector<int> rev(n);
    for (int i=0 ; i<n ; i++) rev[i] = (rev[i/2]|(i&1)<<L)/2;
    for (int i=0 ; i<n ; i++){
        if (i<rev[i]) swap(a[i], a[rev[i]]);
    }

    for (int k=1 ; k<n ; k*=2){
        for (int i=0 ; i<n ; i+=2*k){
            for (int j=0 ; j<k ; j++){
                int z = rt[j+k]*a[i+j+k]%MOD, &ai = a[i+j];
                a[i+j+k] = ai-z+(z>ai ? MOD : 0);
                ai += (ai+z>=MOD ? z-MOD : z);
            }
        }
    }

    if (isInverse) {
        reverse(a.begin()+1, a.end());
        int n_inv = qp(n, MOD-2, MOD);
        for (int i=0 ; i<n ; i++) a[i] = a[i]*n_inv%MOD;
    }
}

// 771576
vector<int> polyMul(vector<int> a, vector<int> b){
    if (a.empty() || b.empty()) return {};
    int s = a.size()+b.size()-1, B = 32-__builtin_clz(s), n = 1<<B;
    vector<int> L(a), R(b), out(n);
    L.resize(n), R.resize(n);
    NTT(L), NTT(R);
    for (int i=0 ; i<n ; i++) out[i] = L[i]*R[i]%MOD;
    NTT(out, true);
    out.resize(s);
    return out;
}