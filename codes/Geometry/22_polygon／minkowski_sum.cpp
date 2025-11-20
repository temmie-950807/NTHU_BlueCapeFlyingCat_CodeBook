void minkowski(vector<point<T>> a, vector<point<T>> b) {
    // a, b: convex polygon's inside vector, CCW order
    auto cmp = [&](auto &a, auto &b) {
        return tie(a.y, a.x) < tie(b.y, b.x);
    };
    auto reorder = [&](auto &u) {
        auto it = min_element(u.begin(), u.end(), cmp);
        rotate(u.begin(), it, u.end());
    };
    reorder(a); reorder(b);
    a.push_back(a[0]); a.push_back(a[1]);
    b.push_back(b[0]); b.push_back(b[1]);
    v.clear();
    for (int i = 0, j = 0; i+2<a.size()||j+2<b.size();) {
        v.push_back(a[i] + b[j]);
        auto val = (a[i + 1] - a[i]) ^ (b[j + 1] - b[j]);
        if (val >= 0) i++;
        if (val <= 0) j++;
    }
} 
