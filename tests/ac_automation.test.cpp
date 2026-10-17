// competitive-verifier: STANDALONE
#include "common.hpp"
#include "../codes/String/AC_automation.cpp"

// AC 自動機：多模式（含重複、互為後綴）出現次數對拍暴力，每組測資重建一個新的自動機
signed main() {
    mt19937 rd(7122);
    for (int it=0 ; it<2000 ; it++){
        int k = rd()%3+1, m = rd()%8+1, n = rd()%40+1;
        string text;
        for (int i=0 ; i<n ; i++) text += 'a'+rd()%k;
        if (it%5==0) text = string(n, 'a');
        vector<string> pats;
        for (int i=0 ; i<m ; i++){
            string p;
            int r = rd()%4;
            if (r==0 && pats.size()) p = pats[rd()%pats.size()];                       // 重複
            else if (r==1 && pats.size()){ p = pats[rd()%pats.size()]; p = p.substr(rd()%p.size()); } // 後綴
            else for (int j=0, L=rd()%5+1 ; j<L ; j++) p += 'a'+rd()%k;
            pats.push_back(p);
        }
        int tot = 1;
        for (auto &p : pats) tot += p.size();
        ACAutomation ac(tot);
        for (auto &p : pats) ac.add(p);
        ac.build();
        ac.buildMatch(text);
        for (int i=0 ; i<m ; i++){
            int cnt = 0;
            for (int j=0 ; j+(int)pats[i].size()<=n ; j++) cnt += text.compare(j, pats[i].size(), pats[i])==0;
            assert(ac.match[ac.pos[i]]==cnt);
        }
    }
    return 0;
}
