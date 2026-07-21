/*
Tested : https://cses.fi/paste/92fb6be2b651cc87112ee10/
         https://cses.fi/paste/cc510df5d0b84d99112ee2e/
*/
// 回傳所有最長（無法往兩側擴展）迴文的區間 [l, r], 0-based
vector<array<int, 2>> Manacher(string str) {
    string tmp = "$#";
    for(char i : str) {
        tmp += i;
        tmp += '#';
    }

    int m = tmp.size(), mx = 0, id = 0;
    vector<int> p(m);
    vector<array<int, 2>> res;
    for(int i=1 ; i<m ; i++) {
        p[i] = mx > i ? min(p[id*2-i], mx-i) : 1;
        while(tmp[i+p[i]] == tmp[i-p[i]]) p[i]++;
        if (mx < i + p[i]) mx = i + p[i], id = i;

        if (p[i] > 1) {
            int r = (i / 2) + (p[i] / 2) - 2 + (i & 1);
            res.push_back({r - p[i] + 2, r});
        }
    }
    return res;
}
