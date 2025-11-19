// 判斷線段 ab, cd 是否相交
friend bool banana(point a, point b, point c, point d) {
    if (btw(a, b, c) || btw(a, b, d)
        || btw(c, d, a) || btw(c, d, b)) return true;
    int u = ori(a, b, c) * ori(a, b, d);
    int v = ori(c, d, a) * ori(c, d, b);
    return u < 0 && v < 0;
} 
