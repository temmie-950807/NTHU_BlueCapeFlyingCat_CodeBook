// 判斷點和有向直線的關係：{1:左邊,0:在線上,-1:右邊}
int ori(point<T> &p) {
    return sign((p2-p1) ^ (p-p1));
}
// 判斷直線斜率是否相同
bool parallel(line &l) {
    return ((p1-p2) ^ (l.p1-l.p2)) == 0;
}
// 兩直線交點
point<ld> line_intersection(line &l) {
    using P = point<ld>;
    point<T> u = p2-p1, v = l.p2-l.p1, s = l.p1-p1;
    return P(p1) + P(u) * ((ld(s^v)) / (u^v));
}
