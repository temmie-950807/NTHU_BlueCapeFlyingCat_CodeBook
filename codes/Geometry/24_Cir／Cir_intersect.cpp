vector<point<ld>> Cir_intersect(Cir c) {
    ld d2 = abs2(o - c.o), d = sqrt(d2);
    if (d < max(r, c.r) - min(r, c.r) || d > r + c.r) return {};
    auto sqdf = [&](ld x, ld y) { return x*x - y*y; };
    point<ld> u = (o + c.o) / 2 + (o - c.o) * (sqdf(c.r, r) / (2 * d2));
    ld A = sqrt(sqdf(r + d, c.r) * sqdf(c.r, d - r));
    point<ld> v = (c.o - o).rotate({0,1}) * A / (2 * d2);
    if (sign(v.x) == 0 && sign(v.y) == 0) return {u};
    return {u - v, u + v};
} 
