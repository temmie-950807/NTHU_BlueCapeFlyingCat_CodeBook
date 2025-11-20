//  可以在有 n 個點的凸包內，用 O(log n) 判斷一個線段：
//  {1 : 線段上存在某一點位於凸包內部（邊上不算）,
//   0 : 線段上存在某一點碰到凸包的邊但線段上任一點均不在凸包內部,
//  -1 : 線段完全在凸包外面}
int segment_pass_convex_interior(line<T> L) {
    if (in_convex(L.p1) == 1 || in_convex(L.p2) == 1) return 1;
    L.build();
    auto res = convex_line_intersect(L);
    if (res.empty()) return -1;
    bool cntp = 0, cntn = 0;
    for (auto &[i, j] : res) if (banana(v[i], v[j], L.p1, L.p2)) {
        int now = L.ori(v[(i + 1) % v.size()]);
        cntp |= (now == 1);
        cntn |= (now == -1);
    }
    return (-1 + cntp + cntn);
} 
