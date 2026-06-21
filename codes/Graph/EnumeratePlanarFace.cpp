/*
Tested : https://codeforces.com/gym/104059/submission/379661333
         (compared with https://codeforces.com/gym/104059/submission/317578324)
         https://codeforces.com/gym/105112/submission/379661757
Author : niter
*/
struct PlanarGraph { // 0-based index
    using T = int;
    int n, m, faces;
    vector<point<T>> v;
    vector<vector<pair<int, int>>> D, G;
    vector<vector<int>> F;
/*  D : 對偶圖，平面圖的邊的編號是 [0, m - 1],
    outer face 的編號 = m, face 的編號是 [m + 1, m + faces]
    F : 存每個 inner face 的 point index */
    vector<int> conv, nxt, vis;

    PlanarGraph(int n, vector<point<T>> _v) :
        n(n), m(0), faces(0), v(_v), G(n) {}

    void add_edge(int x, int y) {
        G[x].emplace_back(y, 2 * m);
        G[y].emplace_back(x, 2 * m + 1);
        conv.push_back(x);
        conv.push_back(y);
        m++;
    }

    vector<T> enumerate_face() {
        D.resize(m + 1);
        F.resize(m + 1);
        nxt.resize(2 * m);
        vis.resize(2 * m);
        for (int i = 0; i < n; i++) {
            sort(G[i].begin(), G[i].end(), [&](auto& a,
                                               auto& b) {
                return (v[a.first]-v[i]) < (v[b.first]-v[i]);
            }); // 極角排序用的 '<' operator
            int sz = G[i].size(), pre = sz - 1;
            for (int j = 0; j < sz; pre = j, j++) {
                nxt[G[i][pre].second] = G[i][j].second ^ 1;
            }
        }

        vector<T> ret;
        // 這個 for 迴圈的 hash value 是 fa2180
        for (int i = 0; i < 2 * m; i++) if (vis[i] == false){
            vector<int> pt, outdeg;
            for (int now = i; !vis[now]; now = nxt[now]) {
                vis[now] = true;
                pt.push_back(conv[now]);
                outdeg.push_back(now / 2);
            }

            T area = -(v[pt.back()] ^ v[pt.front()]);
            for (int j = 0; j + 1 < pt.size(); j++) {
                area -= (v[pt[j]] ^ v[pt[j + 1]]);
            }

            if (area > 0) { // inner face
                F.push_back(move(pt));
                ret.push_back(area);
                faces += 1;
                D.emplace_back();
                for (auto &u:outdeg) {
                    D[m + faces].emplace_back(u, 0);
                    D[u].emplace_back(m + faces, 1);
                }
            }
            else { // outer face or a tree
                for (auto &u:outdeg) {
                    D[m].emplace_back(u, 0);
                }
            }
        }
        return ret;
    }
};
