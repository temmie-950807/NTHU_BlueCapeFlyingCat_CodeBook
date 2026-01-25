/*
Tested: https://cses.fi/problemset/hack/1134/entry/16044047/
Write by: Niter
*/
// K_n 有 n ^ (n - 2) 種生成樹
vector<pair<int, int>> prufer_reverse(vector<int> &v) {
    int n = v.size() + 2, mn = 1, mx = n; // 1-based
    vector<int> deg(mx + 1);
    for (auto &i:v) deg[i] += 1;
    int ind = mn;
    while (deg[ind] != 0) ind += 1;
    int leaf = ind;
    vector<pair<int, int>> res;
    for (auto &i:v) {
        res.emplace_back(leaf, i);
        deg[i] -= 1;
        if (deg[i] == 0 && i < ind) leaf = i;
        else {
            do ind += 1; while (deg[ind] != 0);
            leaf = ind;
        }
    }
    res.emplace_back(leaf, mx);
    return res;
}
