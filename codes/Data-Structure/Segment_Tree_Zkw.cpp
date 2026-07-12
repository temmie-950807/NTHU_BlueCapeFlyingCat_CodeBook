/*
Tested : Random stress test
         https://qoj.ac/submission/2610837
Author : niter
*/
struct SegmentTreeZkw { // 1-based index
    using T = array<int, 2>;
    using U = int;
    int n, m;
    vector<T> seg;
    vector<U> lazy;
    static constexpr T empty_node = {0, 0};
    static constexpr U empty_lazy = 1;

    SegmentTreeZkw(int sz) : n(sz), m(sz - 1) {
        seg.assign(2 * n, empty_node);
        lazy.assign(2 * n, empty_lazy);
    }
    void build(T* a) {
        for (int i = 1; i <= n; ++i) {
            seg[m + i] = a[i];
        }
        for (int i = m; i >= 1; --i) {
            seg[i] = merge(seg[i << 1], seg[i << 1 | 1]);
        }
    }
    void range_update(int l, int r, U val) {
        l += m; r += m;
        int l0 = l, r0 = r;
        push(l0), push(r0);
        while (l <= r) {
            if (l & 1) modify(l++, val);
            if (!(r & 1)) modify(r--, val);
            l >>= 1; r >>= 1;
        }
        push(l0), push(r0);
        pull(l0), pull(r0);
    }
    void modify(int i, U val) {
        seg[i][1] = seg[i][1] * val % mod;
        lazy[i] = lazy[i] * val % mod;
    }
    void push(int i) {
        for (int h = __lg(i); h >= 1; h--) {
            int idx = i >> h;
            if (lazy[idx] == empty_lazy) continue;
            modify(idx << 1, lazy[idx]);
            modify(idx << 1 | 1, lazy[idx]);
            lazy[idx] = empty_lazy;
        }
    }
    void pull(int i) {
        while (i >>= 1) {
            seg[i] = merge(seg[i << 1], seg[i << 1 | 1]);
        }
    }
    T merge(T x, T y) {
        return {x[0] + y[0], (x[1] + hmul_pow[x[0]] * y[1]) % mod};
    }
    T query(int l, int r) { // [l, r]
        T resl = empty_node, resr = empty_node;
        l += m; r += m;
        push(l), push(r);
        while (l <= r) {
            if (l & 1) resl = merge(resl, seg[l++]);
            if (!(r & 1)) resr = merge(seg[r--], resr);
            l >>= 1; r >>= 1;
        }
        return merge(resl, resr);
    }
};
