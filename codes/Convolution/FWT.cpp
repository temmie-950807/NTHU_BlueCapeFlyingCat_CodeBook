// 回傳 c[x] = sum_{i^j=x} a[i]*b[j]，|a| = |b| = 2^k，不取模
vector<int> xor_convolution(vector<int> a, vector<int> b, int k) {
    if (k==0) return vector<int>{a[0]*b[0]};
    vector<int> aa(1 << (k - 1)), bb(1 << (k - 1));
    for (int i=0 ; i<(1<<(k-1)) ; i++){
        aa[i] = a[i] + a[i + (1 << (k - 1))];
        bb[i] = b[i] + b[i + (1 << (k - 1))];
    }
    vector<int> X = xor_convolution(aa, bb, k - 1);
    for (int i=0 ; i<(1<<(k-1)) ; i++){
        aa[i] = a[i] - a[i + (1 << (k - 1))];
        bb[i] = b[i] - b[i + (1 << (k - 1))];
    }
    vector<int> Y = xor_convolution(aa, bb, k - 1);
    vector<int> c(1 << k);
    for (int i=0 ; i<(1<<(k-1)) ; i++){
        c[i                 ] = (X[i] + Y[i]) / 2;
        c[i + (1 << (k - 1))] = (X[i] - Y[i]) / 2;
    }
    return c;
};