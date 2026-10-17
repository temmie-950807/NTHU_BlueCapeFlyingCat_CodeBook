// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/two_sat
#include "common.hpp"
#include "../codes/Graph/2-SAT.cpp"

signed main() {
    ios::sync_with_stdio(0), cin.tie(0);
    string p, cnf;
    int n, m;
    cin >> p >> cnf >> n >> m;
    TWO_SAT ts;
    ts.init(n);
    for (int i = 0, a, b, z; i < m; i++) {
        cin >> a >> b >> z;
        ts.add_disjunction(abs(a) - 1, a < 0, abs(b) - 1, b < 0);
    }
    if (!ts.solve()) return cout << "s UNSATISFIABLE\n", 0;
    vector<int> res;
    ts.get_result(res);
    cout << "s SATISFIABLE\nv";
    for (int i = 0; i < n; i++) cout << ' ' << (res[i] ? i + 1 : -(i + 1));
    cout << " 0\n";
}
