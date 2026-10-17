const int MAX_N = (1<<20); // 必須是 2 的冪次
vector<int> BIT(MAX_N);

void add(int x, int v){ // based-1
    for ( ; x<MAX_N ; x+=x&-x) BIT[x] += v;
}

int query(int x){ // 回傳 [1, x] 的和
    int res = 0;
    for ( ; x ; x-=x&-x) res += BIT[x];
    return res;
}

int k_th(int k){ // 回傳 BIT 中第 k 小的元素（based-1）
    int res = 0;
    for (int i=MAX_N>>1 ; i>=1 ; i>>=1)
        if (BIT[res+i]<k)
            k -= BIT[res+=i];
    return res+1;
}