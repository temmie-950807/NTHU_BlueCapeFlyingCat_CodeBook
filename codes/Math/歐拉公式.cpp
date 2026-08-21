// phi(n) = 小於 n 並與 n 互質的正整數數量。
// O(sqrt(n))，回傳 phi(n)
int phi(int n){
    int ret = n;

    for (int i=2 ; i*i<=n ; i++){
        if (n%i==0){
            while (n%i==0) n /= i;
            ret = ret*(i-1)/i;
        }
    }
    if (n>1) ret = ret*(n-1)/n;

    return ret;
}

// O(n)，回傳 1~n 的 phi 值
vector<int> phi_1_to_n(int n) {
    vector<int> primes, lpf(n + 1), phi(n + 1);
    phi[0] = 0; phi[1] = 1;
    for (int i = 2; i <= n; ++i) {
        if (lpf[i] == 0) {
            primes.push_back(i);
            lpf[i] = i;
        }
        for (auto p : primes) {
            if (i * p > n) break;
            lpf[i * p] = p;
            if (i % p == 0) break;
        }
        bool b = i % (lpf[i] * lpf[i]);
        phi[i] = phi[i / lpf[i]] * (lpf[i] - b);
    }
    return phi;
}
