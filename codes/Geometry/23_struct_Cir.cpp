struct Cir {
    point<ld> o; ld r;
    friend ostream& operator<<(ostream& os, Cir c) {
        return os << "(x" << "+-"[c.o.x >= 0] << abs(c.o.x) << ")^2 + (y" << "+-"[c.o.y >= 0] << abs(c.o.y) << ")^2 = " << c.r * c.r;
    }
    bool covers(Cir b) {
        return sqrt((ld)abs2(o - b.o)) + b.r <= r;
    }
};
