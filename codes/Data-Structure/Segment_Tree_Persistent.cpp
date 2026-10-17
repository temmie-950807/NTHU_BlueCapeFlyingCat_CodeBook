// 全部都是 0-based
// Persistent_Segment_Tree st; st.init(n, q); // q = clone + update 的總次數
// st.build(v, 0);
// 函式：
// update_version(pos, val, ver)：對版本 ver 的 pos 位置改成 val（會直接改到 ver，要保留舊版請先 clone_version）
// query_version(ql, qr, ver)：對版本 ver 查詢 [ql, qr) 的區間和
// clone_version(ver)：複製版本 ver 到最新的版本
struct Persistent_Segment_Tree{
    int n, node_cnt = 0;
    struct Node{
        int lc = -1;
        int rc = -1;
        int val = 0;
    };
    vector<Node> arr;
    vector<int> version;
    
    void init(int _n, int q){ // 節點數 2n + q(lg n + 2)
        n = _n, node_cnt = 0;
        arr.assign(2*n+q*(__lg(n)+2), Node());
        version.assign(1, node_cnt++);
    }

    void pull(Node &c, Node a, Node b){
        c.val = a.val+b.val;
        return;
    }

    // 33aebd
    void build(vector<int> &v, int idx, int ll = 0, int rr = -1){
        if (rr==-1) rr = n;
        auto &now = arr[idx];

        if (rr-ll==1){
            now.val = v[ll];
            return;
        }

        int mid = (ll+rr)/2;
        now.lc = node_cnt++;
        now.rc = node_cnt++;
        build(v, now.lc, ll, mid);
        build(v, now.rc, mid, rr);
        pull(now, arr[now.lc], arr[now.rc]);
        return;
    }

    // 50136b
    void update(int pos, int val, int idx, int ll = 0, int rr = -1){
        if (rr==-1) rr = n;
        auto &now = arr[idx];

        if (rr-ll==1){
            now.val = val;
            return;
        }

        int mid = (ll+rr)/2;
        if (pos<mid){
            arr[node_cnt] = arr[now.lc];
            now.lc = node_cnt;
            node_cnt++;
            update(pos, val, now.lc, ll, mid);
        }else{
            arr[node_cnt] = arr[now.rc];
            now.rc = node_cnt;
            node_cnt++;
            update(pos, val, now.rc, mid, rr);
        }
        pull(now, arr[now.lc], arr[now.rc]);
        return;
    }

    void update_version(int pos, int val, int ver){
        update(pos, val, version[ver]);
    }

    Node query(int ql, int qr, int idx, int ll = 0, int rr = -1){
        if (rr==-1) rr = n;
        auto &now = arr[idx];

        if (ql<=ll && rr<=qr) return now;
        if (rr<=ql || qr<=ll) return Node();

        int mid = (ll+rr)/2;

        Node ret;
        pull(ret, query(ql, qr, now.lc, ll, mid), query(ql, qr, now.rc, mid, rr));
        return ret;
    }

    Node query_version(int ql, int qr, int ver){
        return query(ql, qr, version[ver]);
    }

    void clone_version(int ver){
        version.push_back(node_cnt);
        arr[node_cnt] = arr[version[ver]];
        node_cnt++;
    }
};