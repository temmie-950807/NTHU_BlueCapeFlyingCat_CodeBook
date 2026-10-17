// competitive-verifier: STANDALONE
#include <cassert>
#include <sys/wait.h>
#include <unistd.h>
#include "common.hpp"
#include "../codes/Misc/Fast_Input.cpp"

// readchar 的 static buffer 只能從頭讀一次，所以每組測資 fork 一個子行程，stdin 指到暫存檔
// 長度刻意壓在 BUFSIZ 的邊界上（0、1、BUFSIZ-1、BUFSIZ、BUFSIZ+1、倍數）
signed main() {
    mt19937 rng(7122);
    vector<int> lens = {0, 1, 2, BUFSIZ - 1, BUFSIZ, BUFSIZ + 1, 2 * BUFSIZ, 3 * BUFSIZ - 1, 3 * BUFSIZ, 3 * BUFSIZ + 1};
    char path[] = "/tmp/fast_input_XXXXXX";
    int fd = mkstemp(path);
    assert(fd != -1);
    close(fd);
    for (int len : lens) for (int tail = 0; tail < 2; tail++) { // tail=1：結尾補空白，tail=0：結尾停在數字上
        string s;
        while ((int)s.size() < len) {
            if (rng() % 2) s += '-';
            s += to_string(rng() % 1000000000);
            s += " \n\t"[rng() % 3];
        }
        s.resize(len);
        if (len && tail) s.back() = '\n';
        if (len && s.back() == '-') s.back() = '7';
        vector<int> want;
        istringstream in(s);
        for (long long x; in >> x;) want.push_back(x);

        FILE *f = fopen(path, "wb");
        fwrite(s.data(), 1, s.size(), f);
        fclose(f);
        pid_t pid = fork();
        if (pid == 0) {
            alarm(2); // EOF 沒處理好會卡住或讀爛記憶體
            if (!freopen(path, "rb", stdin)) _exit(3);
            for (int x : want) if (nextint() != x) _exit(1);
            for (int i = 0; i < 5; i++) if (nextint() != 0 || readchar() != EOF) _exit(2); // 重複讀到 EOF
            _exit(0);
        }
        signed st;
        waitpid(pid, &st, 0);
        if (st) { // 正常結束且回傳 0 時 st 才是 0（WIFEXITED 等巨集會被 #define int 弄壞）
            fprintf(stderr, "len=%lld tail=%lld status=%d\n", len, tail, st);
            assert(false);
        }
    }
    remove(path);
    return 0;
}
