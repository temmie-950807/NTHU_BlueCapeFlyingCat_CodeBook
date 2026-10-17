# 依 codes/Geometry 的編號把幾何模板組成 tests/geometry_lib.hpp：python3 tests/build_geometry.py
# 05~09 是 point 的成員、11 是 line 的成員、13~22 是 polygon 的成員、24~27 是 Cir 的成員
# 02_convex_hull 與 03_SVG_writer 是獨立版本，不放進來
# 已知未修的問題：25 用到 pb、27 用到 rngf，測試要在 include 前自己定義
import os, re
D = "codes/Geometry"
files = sorted(f for f in os.listdir(D) if re.match(r"\d\d_", f))
num = lambda f: int(f[:2])
owner = {4: range(5, 10), 10: range(11, 12), 12: range(13, 23), 23: range(24, 28)}
read = lambda f: open(os.path.join(D, f), encoding="utf-8").read().replace("\r\n", "\n").rstrip() + "\n"
out = ["// 由 tests/build_geometry.py 產生，不要手動修改\n"]
for f in files:
    n = num(f)
    if n in (2, 3) or any(n in r for r in owner.values()): continue
    src = read(f)
    if n in owner:
        members = "".join(f"// ---- {g}\n" + read(g) for g in files if num(g) in owner[n])
        k = src.rstrip().rfind("};")
        src = src[:k] + members + src[k:]
    out.append(f"// ==== {f}\n" + src)
open("tests/geometry_lib.hpp", "w", encoding="utf-8").write("".join(out))
print("wrote tests/geometry_lib.hpp")
