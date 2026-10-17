#!/bin/sh
# 本機跑 Library Checker 驗證：sh tests/run.sh
# 需要 GCC 與 competitive-verifier（pip install competitive-verifier）
set -e
cd "$(dirname "$0")/.."
export PATH="$PWD/.venv/bin:/opt/homebrew/bin:$PATH" # macOS：讓 g++ 指向 Homebrew GCC
ulimit -s unlimited 2>/dev/null || ulimit -s hard    # 深層遞迴（SCC、LCA）需要大 stack
python3 tests/check_hash.py                          # 段落 hash 要是最新的
python3 tests/build_geometry.py                      # 組出 tests/geometry_lib.hpp
git add -N tests                                     # oj-resolve 只看 git 追蹤的檔案
competitive-verifier oj-resolve --include tests --config tests/config.toml > tests/verify_files.json
competitive-verifier verify --verify-json tests/verify_files.json --check-error "$@"
