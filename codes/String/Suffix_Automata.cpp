/*
    Based on std_abs's SAM template.
    Haven't checked whether rebuild is correct.

    Tested : https://www.luogu.com.cn/record/288050837
             https://qoj.ac/submission/2658596
             https://cses.fi/paste/82cea009edcd771e1138ab2/
*/
// root node is 0
// node -> strings with the same endpos set,
//         length in range [len[fa] + 1, len]
// link -> longest suffix with different endpos set
// len  -> longest path length from root
// cnt  -> size of endpos set
// node's endpos set -> pos (1-based) in the subtree (link) of node (Some node have pos = 0 and should be ignored)
struct SAM {
  static constexpr int N = (int)(1e5 + 10), Z = 26;
  int ch[2 * N][Z], len[2 * N], link[2 * N], pos[2 * N], cnt[2 * N], sz;
  int newnode() {
    fill_n(ch[sz], Z, 0);
    len[sz] = link[sz] = pos[sz] = cnt[sz] = 0;
    return sz++;
  }
  void build(string s) {
    sz = 0, newnode(), link[0] = -1;
    int lst = 0;
    for (int i = 0; i < s.size(); ++i) {
      int c = s[i] - 'a'; // you may need to change to 'A'
      int cur = newnode();
      len[cur] = len[lst] + 1, pos[cur] = i + 1;
      int p = lst;
      while (~p && !ch[p][c])
        ch[p][c] = cur, p = link[p];
      if (p == -1) link[cur] = 0;
      else {
        int q = ch[p][c];
        if (len[p] + 1 == len[q]) {
          link[cur] = q;
        } else {
          int nxt = newnode();
          len[nxt] = len[p] + 1, link[nxt] = link[q];
          pos[nxt] = 0;
          for (int j = 0; j < Z; ++j)
            ch[nxt][j] = ch[q][j];
          while (~p && ch[p][c] == q)
            ch[p][c] = nxt, p = link[p];
          link[q] = link[cur] = nxt;
        }
      }
      cnt[cur]++, lst = cur;
    }
    vector <int> p(sz);
    iota(p.begin(), p.end(), 0);
    sort(p.begin(), p.end(), [&](int i, int j) {
      return len[i] > len[j];
    });
    for (int i = 0; i < sz; ++i) if (~link[p[i]])
      cnt[link[p[i]]] += cnt[p[i]];
  }
};
