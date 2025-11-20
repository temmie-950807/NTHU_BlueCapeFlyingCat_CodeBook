// return edge endpoint index
// but if (first == second) => intersect at a vertex
vector<pair<int,int>> convex_line_intersect(line<T> L) {
    auto dis = [&](int p){
        return (L.p2 - L.p1) ^ (v[p] - L.p1);
    };
    auto gao = [&](int s) {
        auto f = [&](int i, int j) {
            return sign(dis(i) - dis(j)) == s;
        };
        return cycle_search(f);
    };
    int x = gao(1), y = gao(-1), n = v.size();
    if (sign(dis(x)) < 0 || sign(dis(y)) > 0) return {};
    if (sign(dis(x)) == 0 || sign(dis(y)) == 0) {
        int j = ((sign(dis(x)) == 0 ? x : y) + n - 1) % n;
        vector<pair<int,int>> ret;
        for (int i = 0; i < 3; ++i, j = (j + 1) % n)
            if (sign(dis(j)) == 0) ret.emplace_back(j, j);
        return ret;
    }
    auto g = [&](int l, int r, int s) -> pair<int,int> {
        while ((l + 1) % n != r) {
            int m = ((l + r + (l < r ? 0 : n)) / 2) % n;
            (sign(dis(m)) == s ? l : r) = m;
        }
        return {sign(dis(r)) ? l : r, r};
    };
    return {g(x, y, 1), g(y, x, -1)};
} 
