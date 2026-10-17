/*Tested: https://judge.yosupo.jp/submission/316309
Write by: temmie*/
// 第 i 輪算「字母 'a'+i 或 '*'」的配對數；i=26 時 'a'+26='{' 不會出現，只剩兩邊都是 '*'
// 兩邊都是 '*' 會被 26 個字母各算一次，所以最後扣掉 25 次
const int B = 27;
vector<vector<double>> res(B);
for (int i=0 ; i<B ; i++){
    vector<double> ss, tt;
    for (auto x : s) ss.push_back(x=='a'+i || x=='*');
    for (auto x : t) tt.push_back(x=='a'+i || x=='*');
    reverse(tt.begin(), tt.end());
    res[i] = PolyMul(ss, tt);
}
for (int i=t.size()-1 ; i+t.size()-1<res[0].size() ; i++){
    int total = 0;
    for (int j=0 ; j<B-1 ; j++) total += (int)abs(round(res[j][i]));
    total -= 25*(int)abs(round(res[26][i]));
    cout << (total==t.size());
}