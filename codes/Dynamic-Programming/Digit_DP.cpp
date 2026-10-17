#include <bits/stdc++.h>
using namespace std;
 
long long l, r;
long long dp[20][10][2][2]; // dp[pos][pre][limit][lead] = 後 pos 位，pos 前一位是 pre，（是/否）有上界，（是/否）有前綴零的答案數量
 
// 1763f3
long long memorize_search(string &s, int pos, int pre, bool limit, bool lead){
 
    // 已經被找過了，直接回傳值
    if (dp[pos][pre][limit][lead]!=-1) return dp[pos][pre][limit][lead];
 
    // 已經搜尋完畢，紀錄答案並回傳
    if (pos==(int)s.size()){
        return dp[pos][pre][limit][lead] = 1;
    }
 
    // 枚舉目前的位數數字是多少
    long long ans = 0;
    for (int now=0 ; now<=(limit ? s[pos]-'0' : 9) ; now++){
        if (now==pre){
 
            // 1~9 絕對不能連續出現
            if (pre!=0) continue;
 
            // 如果已經不在前綴零的範圍內，0 不能連續出現
            if (lead==false) continue;
        }
 
        ans += memorize_search(s, pos+1, now, limit&(now==(s[pos]-'0')), lead&(now==0));
    }
 
    // 已經搜尋完畢，紀錄答案並回傳
    return dp[pos][pre][limit][lead] = ans;
}
 
// 回傳 [0, n] 有多少數字符合條件
long long find_answer(long long n){
    memset(dp, -1, sizeof(dp));
    string tmp = to_string(n);
 
    return memorize_search(tmp, 0, 0, true, true);
}
 
int main(){
 
    // input
    cin >> l >> r;
 
    // output - 計算 [l, r] 有多少數字相鄰兩個位數都不相同
    cout << find_answer(r)-find_answer(l-1) << "\n";
 
    return 0;
}