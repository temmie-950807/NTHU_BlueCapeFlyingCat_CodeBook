// 判斷 "射線 ab" 與 "線段 cd" 是否相交
friend bool rayHitSeg(point a,point b,point c,point d) {
    if (a == b) return btw(c, d, a); // Special case
    if (((a - b) ^  (c - d)) == 0) {
        return btw(a, c, b) || btw(a, d, b) || banana(a, b, c, d);
    }
    point u = b - a, v = d - c, s = c - a;
    return sign(s ^ v) * sign(u ^ v) >= 0 && sign(s ^ u)
         * sign(u ^ v) >= 0 && abs(s ^ u) <= abs(u ^ v);
} 
