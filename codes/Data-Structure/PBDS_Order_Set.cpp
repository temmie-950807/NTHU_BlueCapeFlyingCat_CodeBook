// .find_by_order(k) 回傳第 k 小的值（based-0）
// .order_of_key(k) 回傳有多少元素比 k 小
// 不能在 #define int long long 後 #include 檔案
// multiset 不要用 less_equal（erase 會壞），改存 {val, id}
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
template<class T, class Cmp = less<T>>
using oset = tree<T, null_type, Cmp, rb_tree_tag,
                  tree_order_statistics_node_update>;
typedef oset<int> order_set;

// Cmp 不寫就是 less<T>（用 T::operator<）
// oset<int, greater<int>> b;        // 由大到小
// oset<pair<int,int>> c;            // multiset：存 {值, 唯一 id}
// oset<my_struct, my_cmp> e;        // 用 functor
// oset<my_struct, decltype(cmp)> f(cmp); // lambda 要傳 cmp

// 範例：分數，1/2 和 2/4 視為同一個元素
struct Frac {
    long long p, q; // p / q，保證 q > 0
    Frac(long long _p = 0, long long _q = 1) : p(_p), q(_q) {
        if (q < 0) p = -p, q = -q;
    }
    bool operator<(const Frac& o) const {
        return (__int128)p * o.q < (__int128)o.p * q;
    }
};
// oset<Frac> s; s.insert({1, 2}); s.order_of_key({3, 5});
