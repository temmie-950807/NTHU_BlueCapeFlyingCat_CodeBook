/// 警告：以下所有凸包專用的函式都只接受逆時針排序且任三點不共線的凸包 ///
//  可以在有 n 個點的凸包內，用 O(log n) 判斷一個點：
//  {1 : 在凸包內, 0 : 在凸包邊上, -1 : 在凸包外}
int in_convex(point<T> p) {
    int n = v.size();
    int a = ori(v[0], v[1], p), b = ori(v[0], v[n-1], p);
    if (a < 0 || b > 0) return -1;
    if (btw(v[0], v[1], p)) return 0;
    if (btw(v[0], v[n - 1], p)) return 0;
    int l = 1, r = n - 1, mid;
    while (l + 1 < r) {
        mid = (l + r) >> 1;
        if (ori(v[0], v[mid], p) >= 0) l = mid;
        else r = mid;
    }
    int k = ori(v[l], v[r], p);
    if (k <= 0) return k;
    return 1;
} 
