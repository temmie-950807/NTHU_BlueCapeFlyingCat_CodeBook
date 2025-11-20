template<typename T>
struct line {
    point<T> p1, p2;
    // ax + by + c = 0
    T a, b, c; // |a|, |b| ≤ 2C, |c| ≤ 8C²
    line() {}
    line(const point<T> &x,const point<T> &y) : p1(x), p2(y){
        build();
    }
    void build() {
        a = p1.y - p2.y;
        b = p2.x - p1.x;
        c = (-a*p1.x)-b*p1.y;
    }
};
