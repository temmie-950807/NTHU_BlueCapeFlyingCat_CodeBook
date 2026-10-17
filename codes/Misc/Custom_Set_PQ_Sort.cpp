// 所有自訂的結構體，務必檢查相等的 case，給所有元素一個排序的依據
// set/map/pbds 用 !cmp(a,b) && !cmp(b,a) 判斷相等
// 只比部分欄位會被去重；比較必須嚴格（不能寫 <=），否則 UB
// pq 的 cmp(a,b) = true 代表 a 優先度低
// 所以 less 是 max-heap，> 是 min-heap
struct my_struct{
    int val, id;
    my_struct(int _val, int _id = 0) : val(_val), id(_id) {}
    // 方法 1：operator<，可直接 set<my_struct>
    bool operator<(const my_struct& o) const {
        return val != o.val ? val < o.val : id < o.id;
    }
};

// 方法 2：functor，set<my_struct, my_cmp>
struct my_cmp{
    bool operator()(const my_struct& a, const my_struct& b) const {
        return a.val > b.val;
    }
};

// 方法 3：lambda，C++17 以下必須把 cmp 傳進建構子，C++20 可省略
auto cmp = [](my_struct a, my_struct b) {
    return a.val > b.val;
};

set<my_struct, decltype(cmp)> ss({1, 2, 3}, cmp);
priority_queue<my_struct, vector<my_struct>, decltype(cmp)> pq(
    cmp, {1, 2, 3});
map<my_struct, my_struct, decltype(cmp)> mp(
    {{1, 4}, {2, 5}, {3, 6}}, cmp);
