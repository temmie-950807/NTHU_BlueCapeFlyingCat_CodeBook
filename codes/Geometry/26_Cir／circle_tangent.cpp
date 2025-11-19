// returns nothing when two circles are the same 
auto circle_tangent(Cir c2, int sign1) {
    // sign1 = 1 for outer tang, -1 for inter tang
    using ptld = point<ld>;
    vector<pair<ptld, ptld>> res;
    ld d_sq = abs2(o - c2.o);
    if (sign(d_sq) == 0) return res;
    ld d = sqrt(d_sq);
    ptld v = (c2.o - o) / d;
    ld c = (r - sign1 * c2.r) / d;
    if (c * c > 1) return res;
    ld h = sqrt(max((ld)0.0, 1.0 - c * c));
    for (int sign2 = 1; sign2 >= -1; sign2 -= 2) {
        ptld n(v.x * c - sign2 * h * v.y, v.y * c + sign2 * h * v.x);
        ptld p1 = o + n * r;
        ptld p2 = c2.o + n * (c2.r * sign1);
        if (sign(p1.x - p2.x) == 0 && sign(p1.y - p2.y) == 0)
            p2 = p1 + (c2.o - o).rotate({0, 1});
        res.push_back({p1, p2});
    }
    if (sign1 == -1 && sign(r + c2.r - d) == 0) res.pop_back();
    if (sign1 ==  1 && sign(abs(r - c2.r) - d) == 0) res.pop_back();
    return res;
} 
