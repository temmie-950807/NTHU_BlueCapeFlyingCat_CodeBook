//  可以在有 n 個點的凸包內，用 O(log n) 判斷一條直線：
//  {1 : 穿過凸包, 0 : 剛好切過凸包, -1 : 沒碰到凸包}
int line_cut_convex(line<T> L) {
    L.build();
    point<T> p(L.a, L.b);
    auto gt = [&](int neg) {
        auto f = [&](int x, int y) {
            return sign((v[x] - v[y]) * p) == neg;
        };
        return -(v[cycle_search(f)] * p);
    };
    T x = gt(1), y = gt(-1);
    if (L.c < x || y < L.c) return -1;
    return not (L.c == x || L.c == y);
} 
