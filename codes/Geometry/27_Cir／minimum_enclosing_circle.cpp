/*
Tested : https://judge.yosupo.jp/submission/328873
Author : std_abs
*/
point<ld> circenter(point<ld> p0, point<ld> p1, point<ld> p2) {
    // radius = abs(center)
    p1 = p1 - p0, p2 = p2 - p0;
    ld x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
    ld m = 2. * (x1 * y2 - y1 * x2);
    point<ld> center(0, 0);
    center.x = (x1 * x1 * y2 - x2 * x2 * y1 + y1 * y2 * (y1 - y2)) / m;
    center.y = (x1 * x2 * (x2 - x1) - y1 * y1 * x2 + x1 * y2 * y2) / m;
    return center + p0;
}
void min_enclosing(vector<point<ld>> p) {
    shuffle(p.begin(), p.end(), rngf);
    r = 0.0;
    point<ld> cent = p[0];
    for (int i = 1; i < p.size(); ++i) {
        if (abs2(cent - p[i]) <= r) continue;
        cent = p[i], r = 0.0;
        for (int j = 0; j < i; ++j) {
            if (abs2(cent - p[j]) <= r) continue;
            cent = (p[i] + p[j]) / 2, r = abs2(p[j] - cent);
            for (int k = 0; k < j; ++k) {
                if (abs2(cent - p[k]) <= r) continue;
                cent = circenter(p[i], p[j], p[k]);
                r = abs2(p[k] - cent);
            }
        }
    }
    o = cent;
    r = sqrt(r);
}
