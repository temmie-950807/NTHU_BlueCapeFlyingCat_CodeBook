int sign(long long x) {return (x >= 0) ? ((bool)x) : -1; }

template<typename T>
struct point {
    T x, y;
    point() {}
    point(const T &x0, const T &y0) : x(x0), y(y0) {}
    
    point operator-(point b) {return {x-b.x, y-b.y}; }
    bool operator==(point b) {return x==b.x && y==b.y; }
    T operator^(point b) {return x * b.y - y * b.x; }
    friend int ori(point a, point b, point c) {
        return sign((b-a)^(c-a));
    }
};

template<typename T>
struct polygon {
    vector<point<T>> v;
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
};
