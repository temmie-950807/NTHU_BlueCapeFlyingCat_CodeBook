// Returns sum_{i=0}^{n-1} floor((a*i + b) / m)
// Requires a, b >= 0, m >= 1
// log( max(a, m) )
int floor_sum(int n, int m, int a, int b) {
    int ans = 0;
    while (true) {
        if (a >= m) {
            ans += (n - 1) * n * (a / m) / 2;
            a %= m;
        }
        if (b >= m) {
            ans += n * (b / m);
            b %= m;
        }
        int y_max = a * n + b;
        if (y_max < m) break;
        n = y_max / m;
        b = y_max % m;
        swap(m, a);
    }
    return ans;
}