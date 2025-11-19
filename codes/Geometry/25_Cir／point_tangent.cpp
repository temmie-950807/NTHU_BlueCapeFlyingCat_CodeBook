auto point_tangent(point<ld> p) {
    vector<point<ld>> res;
    ld d_sq = abs2(p - o);
    if (sign(d_sq - r * r) <= 0) {
        res.pb(p + (p - o).rotate({0, 1}));
    } else if (d_sq > r * r) {
        ld s = d_sq - r * r;
        point<ld> v = p + (o - p) * s / d_sq;
        point<ld> u = (o - p).rotate({0, 1}) * sqrt(s) * r / d_sq;
        res.pb(v + u);
        res.pb(v - u);
    }
    return res;
} 
