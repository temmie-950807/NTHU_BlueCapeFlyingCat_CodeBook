/*
Tested: https://cses.fi/problemset/hack/3227/entry/17905886/
        https://judge.yosupo.jp/submission/385180
*/
// 全部都是 0-based
// 宣告：LC_Segment_Tree st(n);  // n = 值域大小，x 的範圍為 [0, n)
// 函式：
// st.update_segment({a, b}, ql, qr)：在 [ql, qr) 插入一條 y=ax+b 的線段
// st.query(x)：查詢所有線段在位置 x 的最小值

struct LC_Segment_Tree {
    struct Line { // y = ax + b
        int a = 0, b = INF;
        int y(int x) const { return a * x + b; }
    };

    int N;
    vector<Line> arr;
    LC_Segment_Tree(int n) : N(n), arr(4 * n + 5) {}

    void insert_line(Line val, int idx, int l, int r) {
        if (arr[idx].a > val.a) swap(arr[idx], val);
        if (arr[idx].a == val.a) {
            arr[idx].b = min(arr[idx].b, val.b);
            return;
        }
        if (val.y(l) >= arr[idx].y(l)) return;
        if (val.y(r - 1) < arr[idx].y(r - 1)) {
            arr[idx] = val;
            return;
        }
        int mid = (l + r) / 2;
        if (val.y(mid) < arr[idx].y(mid)) {
            swap(arr[idx], val);
            insert_line(val, 2 * idx + 2, mid, r);
        }
        else insert_line(val, 2 * idx + 1, l, mid);
    }

    void dfs(Line v, int ql, int qr, int idx, int l, int r) {
        if (l >= r || r <= ql || l >= qr) return;
        if (ql <= l && r <= qr) {
            insert_line(v, idx, l, r);
            return;
        }
        int mid = (l + r) / 2;
        dfs(v, ql, qr, 2 * idx + 1, l, mid);
        dfs(v, ql, qr, 2 * idx + 2, mid, r);
    }

    int q(int x, int idx, int l, int r) {
        if (l >= r) return INF;
        int res = arr[idx].y(x);
        if (l + 1 == r) return res;
        int mid = (l + r) / 2;
        if (x < mid) return min(res, q(x, 2*idx+1, l, mid));
        else return min(res, q(x, 2*idx+2, mid, r));
    }

    void update_segment(Line val, int ql, int qr) {
        dfs(val, ql, qr, 0, 0, N);
    }
    int query(int x) { return q(x, 0, 0, N); }
};
