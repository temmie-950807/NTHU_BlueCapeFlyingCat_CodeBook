// competitive-verifier: STANDALONE
// pb_ds 不能在 #define int long long 之後 include，所以這裡不用 common.hpp
#include <bits/stdc++.h>
#include <cassert>
using namespace std;
#include "../codes/Misc/Custom_Set_PQ_Sort.cpp"
#include "../codes/Data-Structure/PBDS_Order_Set.cpp"

signed main() {
    // Custom_Set_PQ_Sort：cmp 是 a.val > b.val
    assert(ss.begin()->val == 3 && pq.top().val == 1 && mp.begin()->first.val == 3);
    set<my_struct> s1{{3, 0}, {3, 1}, {1, 2}};  // operator< 有 tie-breaker，不會去重
    assert(s1.size() == 3 && s1.begin()->val == 1);
    priority_queue<my_struct> q1; q1.push(1); q1.push(5); assert(q1.top().val == 5);
    priority_queue<my_struct, vector<my_struct>, my_cmp> q2; q2.push(1); q2.push(5);
    assert(q2.top().val == 1);

    // PBDS_Order_Set
    order_set os; os.insert(3); os.insert(1); assert(os.order_of_key(2) == 1 && *os.find_by_order(1) == 3);
    oset<int, greater<int>> b; b.insert(3); b.insert(1); assert(*b.find_by_order(0) == 3);
    oset<pair<int, int>> c; c.insert({7, 0}); c.insert({7, 1}); assert(c.size() == 2);
    c.erase(c.lower_bound({7, INT_MIN})); assert(c.size() == 1);
    oset<my_struct, my_cmp> e; e.insert(1); e.insert(5); assert(e.find_by_order(0)->val == 5);
    oset<my_struct, decltype(cmp)> f(cmp); f.insert(1); f.insert(5); assert(f.find_by_order(0)->val == 5);
    oset<Frac> s; s.insert({1, 2}); s.insert({2, 4}); s.insert({1, -3}); s.insert({2, 3});
    assert(s.size() == 3 && s.find_by_order(0)->p == -1 && s.order_of_key({3, 5}) == 2);
    long long B = 1e18;
    s.insert({B, B - 1}); s.insert({B - 1, B - 2});
    assert(s.order_of_key({B - 1, B - 2}) == 4);  // 交叉相乘超過 long long，要靠 __int128
    return 0;
}
