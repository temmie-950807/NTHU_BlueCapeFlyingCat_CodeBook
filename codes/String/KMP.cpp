// KMP[i] = s[0...i] 的最長共同前後綴長度，KMP[KMP[i]-1] 可以跳 fail link
vector<int> KMP(string &s){
    vector<int> ret(s.size());
    for (int i=1 ; i<s.size() ; i++){
        int j = ret[i-1];
        while (j && s[i]!=s[j]) j = ret[j-1];
        ret[i] = j + (s[i]==s[j]);
    }
    return ret;
}
