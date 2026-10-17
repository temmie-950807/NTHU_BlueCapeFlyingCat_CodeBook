// 根節點是 0，位置從 1 開始數
// 節點 -> 一群 endpos（所有結尾位置）相同的子字串，
//         長度剛好是 [len[link] + 1, len] 這一段
// link -> 最長的、endpos 跟自己不同的後綴所在的節點
// len  -> 節點裡最長字串的長度（也是從根走過來的最長路徑長）
// cnt  -> endpos 的大小，也就是出現次數（calc_cnt / build 之後才正確）
// pos  -> 這個節點代表的前綴結尾位置（等於 len），clone 是 0
//         節點的 endpos = 它在 link 樹的子樹裡所有不為 0 的 pos
// fpos -> 節點裡的字串第一次出現的結尾位置（endpos 的最小值）
//         P 第一次出現的起點是 fpos - |P| + 1
struct SAM {
    static constexpr int N = (int)(1e5 + 10), Z = 26;
    int ch[2 * N][Z], len[2 * N], link[2 * N], pos[2 * N], fpos[2 * N], cnt[2 * N], sz, last;

    void init(){ sz = 0, newnode(), link[0] = -1, last = 0; }
    SAM(){ init(); }

    int newnode(){
        fill_n(ch[sz], Z, 0);
        len[sz] = link[sz] = pos[sz] = fpos[sz] = cnt[sz] = 0;
        return sz++;
    }

    // 0f0e45
    void extend(int c){
        int cur = newnode();
        len[cur] = len[last] + 1, pos[cur] = fpos[cur] = len[cur];
        int p = last;
        while (~p && !ch[p][c]) ch[p][c] = cur, p = link[p];
        if (p == -1) link[cur] = 0;
        else {
            int q = ch[p][c];
            if (len[p] + 1 == len[q]) link[cur] = q;
            else {
                int nxt = newnode();
                len[nxt] = len[p] + 1, link[nxt] = link[q];
                pos[nxt] = 0, fpos[nxt] = fpos[q];
                for (int j = 0; j < Z; ++j) ch[nxt][j] = ch[q][j];
                while (~p && ch[p][c] == q) ch[p][c] = nxt, p = link[p];
                link[q] = link[cur] = nxt;
            }
        }
        cnt[cur]++, last = cur;
    }

    void calc_cnt(){ // call once, after all extend
        vector<int> p(sz);
        iota(p.begin(), p.end(), 0);
        sort(p.begin(), p.end(), [&](int i, int j){ return len[i] > len[j]; });
        for (int i = 0; i < sz; ++i)
            if (~link[p[i]]) cnt[link[p[i]]] += cnt[p[i]];
    }

    void build(const string &s){
        init();
        for (char ch : s) extend(ch - 'a'); // you may need to change to 'A'
        calc_cnt();
    }
};