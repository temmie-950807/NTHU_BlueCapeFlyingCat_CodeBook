/* Tested: https://judge.yosupo.jp/submission/319715
Write by: ChatGPT */
// ---- sqrt for formal power series over MOD=998244353 ----
// b17ddb
int mod_sqrt(int a) { // Tonelli–Shanks; return -1 if no solution
    a %= MOD; if (a < 0) a += MOD;
    if (a == 0) return 0;
    if (qp(a, (MOD - 1) / 2, MOD) == MOD - 1) return -1; // non-residue
    if (MOD % 4 == 3) return qp(a, (MOD + 1) / 4, MOD);
    int q = MOD - 1, s = 0;
    while ((q & 1) == 0) q >>= 1, ++s;
    int z = 2;
    while (qp(z, (MOD - 1) / 2, MOD) != MOD - 1) ++z; // find non-residue
    long long c = qp(z, q, MOD);
    long long x = qp(a, (q + 1) / 2, MOD);
    long long t = qp(a, q, MOD);
    int m = s;
    while (t != 1) {
        int i = 1;
        long long tt = t * t % MOD;
        while (tt != 1) {
            tt = tt * tt % MOD;
            ++i;
            if (i == m) return -1; // shouldn't happen
        }
        long long b = qp(c, 1LL << (m - i - 1), MOD);
        x = x * b % MOD;
        long long b2 = b * b % MOD;
        t = t * b2 % MOD;
        c = b2;
        m = i;
    }
    return (int)x;
}

// 4a0a48
vector<int> polySqrt(vector<int> a) {
    int n = (int)a.size();

    // all zero -> zero
    bool all0 = true;
    for (int x : a) if (x % MOD != 0) { all0 = false; break; }
    if (all0) return vector<int>(n, 0);

    // first nonzero position
    int k = 0;
    while (k < n && a[k] == 0) ++k;
    if (k & 1) return {};           // odd -> impossible
    int shift = k / 2;

    // strip x^k
    vector<int> f;
    f.reserve(n - k);
    for (int i = k; i < n; ++i) f.push_back(a[i]);

    // constant term sqrt, choose canonical sign
    int c0 = (f.empty() ? 0 : f[0] % MOD);
    int s0 = mod_sqrt(c0);
    if (s0 == -1) return {};
    if (s0 > MOD - s0) s0 = MOD - s0;   // choose the smaller nonnegative root

    // We must produce N terms after placing the shift => need = N - shift
    int need = n - shift;
    if ((int)f.size() < need) f.resize(need, 0);

    // Newton: g_{new} = (g + f/g) / 2  (mod x^m)
    const int INV2 = (MOD + 1) / 2;
    vector<int> g(1, s0);

    for (int m = 1; m < need; m <<= 1) {
        int lim = min(need, m << 1);

        vector<int> g_cut = g; g_cut.resize(lim);
        vector<int> inv_g = polyInv(g_cut);       // inv up to lim
        vector<int> f_cut(f.begin(), f.begin() + lim);

        vector<int> q = polyMul(inv_g, f_cut);
        if ((int)q.size() > lim) q.resize(lim);

        g.resize(lim);
        for (int i = 0; i < lim; ++i) {
            long long gi = (i < (int)g_cut.size() ? g_cut[i] : 0);
            long long qi = (i < (int)q.size() ? q[i] : 0);
            g[i] = (gi + qi) % MOD * 1LL * INV2 % MOD;
        }
    }
    g.resize(need);

    // place back the x^{k/2} shift
    vector<int> res(n, 0);
    for (int i = 0; i < need && i + shift < n; ++i) res[i + shift] = g[i];
    return res;
}