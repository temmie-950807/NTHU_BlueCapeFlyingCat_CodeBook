//  可以在有 n 個點的簡單多邊形內，用 O(n) 判斷一個點：
//  {1 : 在多邊形內, 0 : 在多邊形上, -1 : 在多邊形外}
int in_polygon(point<T> a){
    const T MAX_POS = 1e9 + 5; // [記得修改] 座標的最大值
    point<T> pre = v.back(), b(MAX_POS, a.y + 1);
    int cnt = 0;

    for (auto &i:v) {
        if (btw(pre, i, a)) return 0;
        if (banana(a, b, pre, i)) cnt++;
        pre = i;
    }

    return cnt%2 ? 1 : -1;
} 
