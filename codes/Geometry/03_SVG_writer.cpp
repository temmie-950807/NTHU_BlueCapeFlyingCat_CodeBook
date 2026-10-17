/*
Author : std_abs
*/
class SVG {
    void p(string_view s) { o << s; }
    void p(string_view s, auto v, auto... vs) {
        auto i = s.find('$');
        o << s.substr(0, i) << v, p(s.substr(i + 1), vs...);
    }
    ofstream o;
    string c = "red";
public:
    // cc67e3
    SVG(auto f, auto x1, auto y1, auto x2, auto y2) : o(f) {
        p("<svg xmlns='http://www.w3.org/2000/svg' "
        "viewBox='$ $ $ $'>\n"
        "<style>*{stroke-width:0.5%;}</style>\n",
        x1, -y2, x2 - x1, y2 - y1);
    }
    ~SVG() { p("</svg>\n"); }
    void color(string nc) { c = nc; }
    // a08d95
    void line(auto x1, auto y1, auto x2, auto y2) {
        p("<line x1='$' y1='$' x2='$' y2='$' stroke='$'/>\n",
        x1, -y1, x2, -y2, c);
    }
    // 73c400
    void circle(auto x, auto y, auto r) {
        p("<circle cx='$' cy='$' r='$' stroke='$' "
        "fill='$'/>\n", x, -y, r, c, "#none");
    }
    // a23944
    void text(auto x, auto y, string s, int w = 12) {
        p("<text x='$' y='$' font-size='$px'>$</text>\n",
        x, -y, w, s);
    }
};
// declare : SVG svg("test.svg", -99, -99, 100, 100);
