mt19937 seed(chrono::steady_clock::now().time_since_epoch().count());
int rng(int l, int r){
    return uniform_int_distribution<int>(l, r)(seed);
}
const int B = (1LL<<61)-1;
int A = rng(1e5, B-1);

// 1903d7
struct RollingHash{
    vector<int> Pow, Pre;
    RollingHash(string s = ""){
        Pow.resize(s.size());
        Pre.resize(s.size());

        for (int i=0 ; i<s.size() ; i++){
            if (i==0){
                Pow[i] = 1;
                Pre[i] = s[i];
            }else{
                Pow[i] = (__int128)Pow[i-1]*A%B;
                Pre[i] = ((__int128)Pre[i-1]*A+s[i])%B;
            }
        }

        return;
    }

    int get(int l, int r){ // 取得 [l, r] 的數值
        if (l==0) return Pre[r];
        int res = Pre[r]-(int)((__int128)Pre[l-1]*Pow[r-l+1]%B);
        if (res<0) res += B;
        return res;
    }
};
