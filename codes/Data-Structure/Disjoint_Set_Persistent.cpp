// 依賴 Persistent_Segment_Tree，0-based
// init(n, q)：q = unite 次數，版本 0 為初始狀態，第 i 次 unite 後為版本 i
// find(a, ver)：查詢版本 ver 中 a 的根
struct Persistent_Disjoint_Set{
    Persistent_Segment_Tree arr, sz;

    void init(int n, int q){
        vector<int> v1, v2(n, 1);
        for (int i=0 ; i<n ; i++){
            v1.push_back(i);
        }
        arr.init(n, q);
        arr.build(v1, 0);
        sz.init(n, q);
        sz.build(v2, 0);
    }

    int find(int a, int ver){
        int res = arr.query_version(a, a+1, ver).val;
        if (res==a) return a;
        return find(res, ver);
    }

    bool unite(int a, int b){ // 以最新版本為基礎產生新版本
        int ver = arr.version.size();
        arr.clone_version(ver-1);
        sz.clone_version(ver-1);
        a = find(a, ver);
        b = find(b, ver);
        if (a==b) return false;

        int sz1 = sz.query_version(a, a+1, ver).val;
        int sz2 = sz.query_version(b, b+1, ver).val;
        if (sz1<sz2) swap(a, b);
        arr.update_version(b, a, ver);
        sz.update_version(a, sz1+sz2, ver);
        return true;
    }
};