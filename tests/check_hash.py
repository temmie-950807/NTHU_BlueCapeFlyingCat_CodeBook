# 檢查 codes/ 裡每個段落 hash 是否為最新：python3 tests/check_hash.py [--fix]
# 規則：// xxxxxx 涵蓋下一行開始到該區塊結束的 }，算法同 codes/Misc/4_Hash_Command.py
import hashlib, os, re, subprocess, sys

def find_cpp():
    # macOS 的 cpp 是 clang，不支援 -fpreprocessed，要找 GCC 的 cpp
    for c in [os.environ.get("CPP"), "cpp-16", "cpp-15", "cpp-14", "cpp-13", "cpp"]:
        try:
            if c and subprocess.run([c, "-fpreprocessed", "-P", "-dD"], input="int x;",
                                    capture_output=True, text=True).returncode == 0:
                return c
        except FileNotFoundError:
            pass
    sys.exit("找不到支援 -fpreprocessed 的 GCC cpp（可用環境變數 CPP 指定）")

CPP = find_cpp()

def H(code):
    out = subprocess.run([CPP, "-dD", "-P", "-fpreprocessed"], input=code,
                         capture_output=True, text=True).stdout
    return hashlib.md5(re.sub(r"\s", "", out).encode()).hexdigest()[:6]

def code_lines(L):
    # 去掉字串、字元與註解，只留下用來數大括號的程式碼
    res, inblk = [], False
    for s in L:
        s = re.sub(r'"(\\.|[^"\\])*"|\'(\\.|[^\'\\])*\'', '""', s)
        out, i = "", 0
        while i < len(s):
            if inblk:
                j = s.find("*/", i)
                if j < 0: break
                inblk, i = False, j + 2
            elif s.startswith("//", i): break
            elif s.startswith("/*", i): inblk, i = True, i + 2
            else: out, i = out + s[i], i + 1
        res.append(out)
    return res

def block_end(C, i):
    depth, started = 0, False
    for j in range(i, len(C)):
        if "{" in C[j]: started = True
        depth += C[j].count("{") - C[j].count("}")
        if started and depth <= 0: return j
    return len(C) - 1

MARK = re.compile(r'^\s*//\s*([0-9a-f]{6})\s*$')
ALT = re.compile(r'hash value 是 ([0-9a-f]{6})')
TRAIL = re.compile(r'\}\s*//\s*[0-9a-f]{6}\s*$')

def main():
    fix = "--fix" in sys.argv
    bad, ok = [], 0
    for root, _, fs in sorted(os.walk("codes")):
        for f in sorted(fs):
            if not f.endswith(".cpp"): continue
            p = os.path.join(root, f)
            with open(p, encoding="utf-8", newline="") as fp: raw = fp.read()
            L = raw.split("\n")  # 保留 \r，避免改掉 CRLF
            m = re.match(r'^\s*/\*.*?\*/', raw, flags=re.S)  # build.py 會去掉開頭的 /* */
            skip = raw[:m.end()].count("\n") + 1 if m else 0
            C = code_lines(L)
            changed = False
            for i in range(skip, len(L)):
                if TRAIL.search(L[i]):
                    bad.append(f"{p}:{i+1}: hash 要寫在區塊上方，不要寫在 }} 後面")
                    continue
                mm = MARK.match(L[i]) or ALT.search(L[i])
                if not mm: continue
                h = H("\n".join(L[i+1:block_end(C, i + 1) + 1]))
                if mm.group(1) == h:
                    ok += 1
                    continue
                bad.append(f"{p}:{i+1}: {mm.group(1)} 應為 {h}")
                if fix:
                    L[i] = L[i][:mm.start(1)] + h + L[i][mm.end(1):]
                    changed = True
            if changed:
                with open(p, "w", encoding="utf-8", newline="") as fp: fp.write("\n".join(L))
    print(f"正確 {ok} 個，{'已修正' if fix else '有問題'} {len(bad)} 個")
    for b in bad: print("  " + b)
    sys.exit(1 if bad and not fix else 0)

if __name__ == "__main__":
    main()
