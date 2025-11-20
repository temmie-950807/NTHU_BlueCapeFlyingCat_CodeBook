// simple 為 true 的時候會回傳任意三點不共線的凸包
void make_convex_hull(int simple) {
    auto cmp = [&](point<T> &p, point<T> &q) {
        return (p.x == q.x) ? (p.y < q.y) : (p.x < q.x);
    };
    simple = (bool)simple;
    sort(v.begin(), v.end(), cmp);
    v.resize(unique(v.begin(), v.end()) - v.begin());
    if (v.size() <= 1) return;
    vector<point<T>> hull;
    for (int t = 0; t < 2; ++t){
        int sz = hull.size();
        for (auto &i:v) {
            while (hull.size() >= sz+2 && ori(hull[hull.
                  size()-2], hull.back(), i) < simple) {
                hull.pop_back();
            }
            hull.push_back(i);
        }
        hull.pop_back();
        reverse(v.begin(), v.end());
    }
    swap(hull, v);
} 
