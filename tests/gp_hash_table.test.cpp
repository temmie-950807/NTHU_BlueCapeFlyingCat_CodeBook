// competitive-verifier: STANDALONE
// pb_ds 不能在 #define int long long 之後 include，所以這裡不用 common.hpp
#include <bits/stdc++.h>
#include <cassert>
using namespace std;
#include "../codes/Data-Structure/PBDS_GP_Hash_Table.cpp"

// gp_hash_table 基本操作與 map 比對（含負數 key）；order_set 順便測
signed main() {
    mt19937_64 rng(7122);
    map<int, int> ref;
    for (int op = 0; op < 200000; op++) {
        int k = (int)(rng() % 2000) - 1000, t = rng() % 4;
        if (op % 3 == 0) k = (int)rng(); // 大範圍 key
        if (t == 0) ss[k] += op, ref[k] += op;
        else if (t == 1) assert((ss.find(k) != ss.end()) == ref.count(k));
        else if (t == 2) ss.erase(k), ref.erase(k);
        else if (ref.count(k)) assert(ss[k] == ref[k]);
        if (op % 10000 == 0) {
            assert(ss.size() == ref.size());
            size_t cnt = 0;
            for (auto [key, val] : ss) assert(ref.at(key) == val), cnt++;
            assert(cnt == ref.size());
        }
    }
    order_set os;
    os.insert(5), os.insert(-3), os.insert(5);
    assert(os.size() == 2 && *os.find_by_order(0) == -3 && os.order_of_key(5) == 1);
    puts("OK");
    return 0;
}
