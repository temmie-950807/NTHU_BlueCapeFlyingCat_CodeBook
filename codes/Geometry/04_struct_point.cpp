template<typename T>
struct point {
    T x, y;
    point() {}
    point(const T &x0, const T &y0) : x(x0), y(y0) {}
    explicit operator point<ld>() {return point<ld>(x, y); }
};
