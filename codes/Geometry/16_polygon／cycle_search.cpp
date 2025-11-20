//  凸包專用的環狀二分搜，回傳 0-based index
int cycle_search(auto &f) {
    int n = v.size(), l = 0, r = n;
    if (n == 1) return 0;
    bool rv = f(1, 0);
    while (r - l > 1) {
        int m = (l + r) / 2;
        if (f(0, m) ? rv: f(m, (m + 1) % n)) r = m;
        else l = m;
    }
    return f(l, r % n) ? l : r % n;
} 
