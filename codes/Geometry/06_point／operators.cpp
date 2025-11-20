// 逆時針極角排序
bool side() const{return (y == 0) ? (x > 0) : (y < 0); }
bool operator<(const point &b) const {
    return side() == b.side() ?
        (x*b.y > b.x*y) : side() < b.side();
}
friend ostream& operator<<(ostream& os, point p) {
    return os << "(" << p.x << ", " << p.y << ")";
}
// 判斷 ab 到 ac 的方向：{1:逆時鐘,0:重疊,-1:順時鐘}
friend int ori(point a, point b, point c) {
    return sign((b-a)^(c-a));
}
friend bool btw(point a, point b, point c) {
    return ori(a, b, c) == 0 && sign((a-c)*(b-c)) <= 0;
}
