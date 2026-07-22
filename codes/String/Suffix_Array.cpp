/*
Tested : https://judge.yosupo.jp/submission/387108
*/
// 注意，當 |s|=1 時，lcp 不會有值，務必測試 |s|=1 的 case
struct SuffixArray {
    string s;
    vector<int> sa, lcp;

    // 0eea96
    template<typename T>
    void sais(const T s, int n, int *sa, int *lar, int *p, int *t, int A) {
        int *rnk = p + n, *q = p + n / 2, *bkt = lar + A;
        int m = 0, i, j, x = t[n - 1] = 1, y = rnk[0] = -1, cnt = -1;

        for (i=n-2 ; ~i ; i--) t[i] = (s[i] == s[i + 1] ? t[i + 1] : s[i] < s[i + 1]);
        for (i=1 ; i<n ; i++) rnk[i] = t[i] && !t[i-1] ? (p[m]=i, m++) : -1;
        fill_n(lar, A, 0);
        for (i=0 ; i<n ; i++) ++lar[s[i]];
        for (i=1 ; i<A ; i++) lar[i] += lar[i-1];
        auto pushS = [&](int x) { sa[--bkt[s[x]]] = x; };
        auto pushL = [&](int x) { sa[bkt[s[x]]++] = x; };
        auto induce_sort = [&](int* v) {
            fill_n(sa, n, 0);
            copy_n(lar, A, bkt);
            for (i=m-1 ; ~i ; i--) pushS(v[i]);
            copy_n(lar, A - 1, bkt + 1);
            for (i=0 ; i<n ; i++) if (sa[i] && !t[sa[i] - 1]) pushL(sa[i] - 1);
            copy_n(lar, A, bkt);
            for (i=n-1 ; ~i ; i--) if (sa[i] && t[sa[i] - 1]) pushS(sa[i] - 1);
        };
        induce_sort(p);
        for (i=0 ; i<n ; i++){
            if (~(x=rnk[sa[i]])){
                j = y < 0 || memcmp(s + p[x], s + p[y], (p[x + 1] - p[x]) * sizeof(s[0]));
                q[y = x] = cnt += j;
            }
        }
        if (cnt+1<m) sais(q, m, sa, bkt, rnk, t + n, cnt + 1);
        else for (i=0 ; i<m ; i++) sa[q[i]] = i;
        for (i=0 ; i<m ; i++) q[i] = p[sa[i]];
        induce_sort(q);
    }

    // da9ddf, O(n + lim), lim = max{s[i]}
    template<typename T>
    SuffixArray(const T& _s) : s(_s.begin(), _s.end()) {
        s.push_back(-1);
        for (auto &i:s) i += 1;
        int n = s.size(), lim = *max_element(s.begin(), s.end()) + 5;
        sa.resize(n);
        lcp.resize(n);
        vector<int> bkt(n + lim * 2), p(n * 2), t(n * 2), rank(n);
        sais(&s[0], n, &sa[0], &bkt[0], &p[0], &t[0], lim);

        for (int i=1 ; i<n ; i++) rank[sa[i]] = i;
        for (int i=0, j, k=0 ; i<n-1 ; lcp[rank[i++]]=k){
            for (k && k--, j=sa[rank[i]-1] ; i+k<s.size() && j+k<s.size() && s[i+k]==s[j+k] ; k++);
        }

        sa.erase(sa.begin());
        lcp.erase(lcp.begin(), lcp.begin()+2);
        s.pop_back();
    }

    // f49583
    vector<int> pos; // pos[i] = i 這個值在 pos 的哪個地方
    SparseTable st;
    void init_lcp(){
        pos.resize(sa.size());
        for (int i=0 ; i<sa.size() ; i++){
            pos[sa[i]] = i;
        }
        if (lcp.size()){
            st.build(lcp);
        }
    }

    // 用之前記得 init
    // 查詢「sa 上的位置」的 x 跟 y 的 lcp
    int get_lcp(int x, int y){
        if (x==y) return s.size()-x;
        if (x>y) swap(x, y);
        return st.query(x, y);
    }

    // 回傳 [l1, r1] 跟 [l2, r2] 的 lcp，0-based
    int get_lcp(int l1, int r1, int l2, int r2){
        int pos_1 = pos[l1], len_1 = r1-l1+1;
        int pos_2 = pos[l2], len_2 = r2-l2+1;
        if (pos_1>pos_2){
            swap(pos_1, pos_2);
            swap(len_1, len_2);
        }

        if (l1==l2){
            return min(len_1, len_2);
        }else{
            return min({st.query(pos_1, pos_2), len_1, len_2});
        }
    }

    // 檢查 [l1, r1] 跟 [l2, r2] 的大小關係，0-based
    // 如果前者小於後者，就回傳 <0，相等就回傳 =0，否則回傳 >0
    // 5b8db0
    int substring_cmp(int l1, int r1, int l2, int r2){
        int len_1 = r1-l1+1;
        int len_2 = r2-l2+1;
        int res = get_lcp(l1, r1, l2, r2);

        if (res<len_1 && res<len_2){
            return s[l1+res]-s[l2+res];
        }else if (len_1==res && len_2==res){
            return 0;
        }else{
            return len_1==res ? -1 : 1;
        }
    }

    // 對於位置在 <=p 的後綴，找離他左邊/右邊最接近位置 >p 的後綴的 lcp，0-based
    // pre[i] = s[i] 離他左邊最接近位置 >p 的後綴的 lcp，0-based
    // suf[i] = s[i] 離他右邊最接近位置 >p 的後綴的 lcp，0-based
    // da12fa
    pair<vector<int>, vector<int>> get_left_and_right_lcp(int p){
        vector<int> pre(p+1);
        vector<int> suf(p+1);

        { // build pre
            int now = 0;
            for (int i=0 ; i<s.size() ; i++){
                if (sa[i]<=p){
                    pre[sa[i]] = now;
                    if (i<lcp.size()) now = min(now, lcp[i]);
                }else{
                    if (i<lcp.size()) now = lcp[i];
                }
            }
        }
        { // build suf
            int now = 0;
            for (int i=s.size()-1 ; i>=0 ; i--){
                if (sa[i]<=p){
                    suf[sa[i]] = now;
                    if (i-1>=0) now = min(now, lcp[i-1]);
                }else{
                    if (i-1>=0) now = lcp[i-1];
                }
            }
        }

        return {pre, suf};
    }
};
