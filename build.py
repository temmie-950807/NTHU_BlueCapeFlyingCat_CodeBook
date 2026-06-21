import sys
from os import walk, system, popen
from os.path import join, split, splitext
from subprocess import run as subprocess_run
from subprocess import PIPE as subprocess_PIPE
from subprocess import Popen as subprocess_Popen
from subprocess import STDOUT as subprocess_STDOUT
from hashlib import md5
import threading
import re
import tempfile
import atexit

from datetime import datetime
try:
    from zoneinfo import ZoneInfo  # Python 3.9+
except Exception:
    ZoneInfo = None
import re
import os

# 用於存儲需要清理的臨時文件
temp_files_to_cleanup = []

def cleanup_temp_files():
    """清理所有臨時文件"""
    for temp_file in temp_files_to_cleanup:
        try:
            if os.path.exists(temp_file):
                os.remove(temp_file)
        except Exception:
            pass

# 註冊清理函數
atexit.register(cleanup_temp_files)

def update_version_in_setup(setup_path: str, tz: str = "Asia/Taipei") -> None:
    """將 Codebook-setup.tex 的版本字串覆蓋為今天日期（台北時區）"""
    if not os.path.exists(setup_path):
        colored_warning(f"{setup_path} not found; skip version update")
        return

    # 產生日期（台北時區）
    if ZoneInfo is not None:
        today = datetime.now(ZoneInfo(tz)).strftime("%Y-%m-%d")
    else:
        # 後備：無 zoneinfo 就用系統本地時間
        today = datetime.now().strftime("%Y-%m-%d")

    with open(setup_path, "r", encoding="utf-8") as f:
        text = f.read()

    # 依序嘗試三種常見寫法：\newcommand{\Version}{...}、\def\Version{...}、\Version{...}
    patterns = [
        (r'(?m)^(\\newcommand\{\\Version\}\{)[^}]*\}', r'\g<1>' + today + r'}'),
        (r'(?m)^(\\def\\Version\{)[^}]*\}',            r'\g<1>' + today + r'}'),
        (r'(?m)^(\\Version\{)[^}]*\}',                r'\g<1>' + today + r'}'),
    ]

    new_text = text
    replaced = False
    for pat, repl in patterns:
        new_text, n = re.subn(pat, repl, new_text)
        if n > 0:
            replaced = True

    # 若找不到任何定義，就在檔案開頭補上一行
    if not replaced:
        prepend = f"\\newcommand{{\\Version}}{{{today}}}\n"
        new_text = prepend + new_text
        normal_print(f"    Injected \\version{{{today}}} into {setup_path}")

    if new_text != text:
        with open(setup_path, "w", encoding="utf-8") as f:
            f.write(new_text)
        colored_warning(f"Updated version -> {today}")
    else:
        normal_print("    Version unchanged (already up to date)")

sys.stdin.reconfigure(encoding="utf-8")
sys.stdout.reconfigure(encoding="utf-8")

splitter = "\\" if sys.platform == "win32" else "/"

RequireOptionDict = {
    ".cpp": "includecpp",
    ".py": "includepy",
    ".tex": "includetex",
    ".vimrc": "includevim",
}

# ANSI 顏色碼
class Colors:
    RESET = '\033[0m'
    ORANGE = '\033[33m'    # 橘色 - 警告訊息

def colored_warning(message):
    """印出橘色警告訊息"""
    if sys.platform == "win32":
        # Windows 可能不支援 ANSI 顏色，嘗試啟用
        system("color")
    print(f"{Colors.ORANGE}    [Warning]: {message}{Colors.RESET}")

def normal_print(message):
    """印出普通訊息（使用預設終端顏色）"""
    print(message)

def check_pdf_pages(pdf_path, max_pages=25):
    """檢查 PDF 頁數，超過指定頁數則顯示警告"""
    try:
        result = subprocess_run(
            ["pdfinfo", pdf_path],
            stdout=subprocess_PIPE,
            stderr=subprocess_PIPE,
            text=True
        )

        if result.returncode != 0:
            colored_warning(f"Unable to read PDF file: {pdf_path}")
            return

        # 從 pdfinfo 輸出中提取頁數
        for line in result.stdout.split('\n'):
            if line.strip().startswith('Pages:'):
                pages_match = re.search(r'Pages:\s*(\d+)', line)
                if pages_match:
                    pages = int(pages_match.group(1))
                    normal_print(f"    PDF page count: {pages}")
                    if pages > max_pages:
                        colored_warning(f"PDF page count ({pages}) exceeds {max_pages} page limit!")
                    return pages

        colored_warning("Unable to get page count information from PDF")

    except FileNotFoundError:
        colored_warning("pdfinfo tool not found, unable to check PDF page count")
    except Exception as e:
        colored_warning(f"Error occurred while checking PDF page count: {str(e)}")


def toLatex(string):
    string = string.replace("_", " ")
    string = string.replace("&", "\\&")
    return string


def replace(string):
    string = string.replace("\\", "/")
    return string

def remove_long_comment_from_start(file_path):
    """移除文件開頭的長註解，返回處理後的內容和是否有長註解"""
    try:
        with open(file_path, "r", encoding="utf-8") as f:
            content = f.read()

        # 檢查是否以 /* 開頭
        if content.strip().startswith("/*"):
            # 找到第一個 */ 的位置
            end_pos = content.find("*/")
            if content.find("*/\n"):
                end_pos += 1
            if end_pos != -1:
                # 移除從開頭到 */ 的內容（包括 */）
                remaining_content = content[end_pos + 2:]
                return remaining_content, True
            else:
                # 如果沒有找到結束標記，返回原內容
                return content, True
        else:
            return content, False
    except Exception:
        return None, False

def has_long_comment_at_start(file_path):
    """檢查文件開頭是否有長註解 (/**/)"""
    _, has_comment = remove_long_comment_from_start(file_path)
    return has_comment

def get_hash(file_path):
    # 檢查是否有開頭長註解
    processed_content, has_comment = remove_long_comment_from_start(file_path)

    if has_comment:
        colored_warning(f"File {file_path} contains long comment at start, removing comment for compilation")

        # 創建臨時文件
        temp_fd, temp_path = tempfile.mkstemp(suffix='.cpp', text=True)
        temp_files_to_cleanup.append(temp_path)

        try:
            with os.fdopen(temp_fd, 'w', encoding='utf-8') as temp_file:
                temp_file.write(processed_content)

            # 使用臨時文件計算 hash
            cpp_process = subprocess_run(
                ["cpp", "-dD", "-P", "-fpreprocessed", temp_path],
                stdout=subprocess_PIPE,
                stderr=subprocess_PIPE,
                text=True
            )
        except Exception as e:
            # 確保文件描述符被關閉
            try:
                os.close(temp_fd)
            except:
                pass
            raise e
    else:
        # 沒有長註解，直接處理原文件
        cpp_process = subprocess_run(
            ["cpp", "-dD", "-P", "-fpreprocessed", file_path],
            stdout=subprocess_PIPE,
            stderr=subprocess_PIPE,
            text=True
        )

    if cpp_process.returncode != 0:
        raise RuntimeError(f"cpp failed: {cpp_process.stderr}")

    no_whitespace = ''.join(cpp_process.stdout.split())
    md5_hash = md5(no_whitespace.encode()).hexdigest()

    return md5_hash[:6]

def split_sort_value(name):
    for i in range(0, len(name)):
        if not name[i].isdigit():
            return [int(name[:i]), name[i+1:]]
    return [999, name]

def PrepareFileDict(CurPath):
    FileDict = {}
    for root, _, files in walk(CurPath):
        for filename in files:
            fullpath = join(root, filename)
            name, file_extension = splitext(filename)
            if file_extension not in RequireOptionDict:
                colored_warning(f"{name}{file_extension} will not be included in compilation")
                continue
            if fullpath[0:3] == "." + splitter + ".":
                continue
            if root == CurPath:
                continue
            DirName = toLatex(split(root)[-1])
            if DirName not in FileDict:
                FileDict[DirName] = []
            sort_value = 999
            printed_name = toLatex(name)
            if len(printed_name) > 0 and printed_name[0].isdigit():
                sort_value, printed_name = split_sort_value(printed_name)
            FileDict[DirName].append((file_extension, sort_value, printed_name, replace(fullpath)))
    return FileDict


def cmp(x):
    val = 0
    if x == "Misc":
        val = -1
    return [val, x]

def texCodeGen(out, FileDict, FileList):
    for key in sorted(FileDict.keys(), key=cmp):
        out.write("\\section{" + key + "}\n")
        for file_extension, sort_value, name, path in sorted(FileDict[key], key = lambda x : [x[1], x[2]]):
            # 對於 C++ 文件，如果有長註解需要特殊處理
            if file_extension == ".cpp":
                processed_content, has_comment = remove_long_comment_from_start(path)

                if has_comment:
                    # 創建臨時文件用於 LaTeX 包含
                    temp_fd, temp_path = tempfile.mkstemp(suffix='.cpp', text=True)
                    temp_files_to_cleanup.append(temp_path)

                    try:
                        with os.fdopen(temp_fd, 'w', encoding='utf-8') as temp_file:
                            temp_file.write(processed_content)

                        # 使用臨時文件路徑
                        actual_path = replace(temp_path)
                        FileList.append(actual_path)
                    except Exception as e:
                        try:
                            os.close(temp_fd)
                        except:
                            pass
                        raise e
                else:
                    actual_path = path
                    FileList.append(actual_path)

                hash_value = get_hash(path)
                if hash_value:
                    out.write(
                        "  \\includecppwithhash{"
                        + name
                        + "}{"
                        + actual_path
                        + "}{"
                        + hash_value
                        + "}\n"
                    )
                else:
                    out.write(
                        "  \\includecpp{"
                        + name
                        + "}{"
                        + actual_path
                        + "}\n"
                    )
            else:
                FileList.append(path)
                out.write(
                    "  \\"
                    + RequireOptionDict[file_extension]
                    + "{"
                    + name
                    + "}{"
                    + path
                    + "}\n"
                )

def print_progress_bar(progress, width=40):
    """
    以浮點數 progress 顯示進度條，會覆蓋掉同一行。
    progress: 介於 0 到 1 的浮點數
    width: 進度條寬度（格數）
    """
    progress = max(0, min(progress, 1))
    filled = int(width * progress)
    bar = '#' * filled + '-' * (width - filled)
    percent = int(progress * 100)
    # 進度條使用預設顏色
    sys.stdout.write(f"\r[3] Compiling LaTeX : [{bar}] {percent}%")
    sys.stdout.flush()

def monitor_output(stream, n, first_run, FileList):
    for line in iter(stream.readline, ''):
        line = line.strip()
        while len(FileList) > 0:
            if FileList[-1] in line:
                FileList.pop()
                print_progress_bar((n - len(FileList)) / n / 2 + int(not first_run) * 0.5)
            else:
                break

    stream.close()

def GenerateCodebook(FileList, first_run):
    command = [
        "xelatex", "-interaction=nonstopmode", "-synctex=1", "--extra-mem-bot=10000000", "Codebook.tex"
    ]
    proc = subprocess_Popen(
        command,
        stdout=subprocess_PIPE,
        stderr=subprocess_STDOUT,
        universal_newlines=True,
        bufsize=1,
        encoding='utf-8'
    )

    n = len(FileList)
    FileList.reverse()
    for i in range(n):
        if FileList[i][:2] == "./":
            FileList[i] = FileList[i][2:]

    thread = threading.Thread(target=monitor_output, args=(proc.stdout, n, first_run, FileList))
    thread.start()
    proc.wait()
    thread.join()
    if not first_run:
        print() # New line after the progress bar

if __name__ == "__main__":
    normal_print("[#] Start Processing Code Book List...")

        # 在編譯前更新 Codebook-setup.tex 的 \version
    update_version_in_setup("Codebook-setup.tex")

    normal_print("[1] Get Codes...")

    FileDict = PrepareFileDict("./codes")
    
    total_files = 0
    for key in FileDict.keys():
        total_files += len(FileDict[key])
    normal_print("    There are " + str(len(FileDict)) + " folder"+ "(s)" * min(len(FileDict) - 1, 1) + " and " + str(total_files) + " file"+ "(s)" * min(total_files - 1, 1))
    normal_print("[2] Prepare Codes...")
    FileList = list()
    with open("list.tex", "w", encoding="utf-8") as fout:
        texCodeGen(fout, FileDict, FileList)

    GenerateCodebook(FileList[:], True)
    GenerateCodebook(FileList[:], False)
    normal_print("[#] Done!")

    # 檢查生成的 PDF 頁數
    normal_print("[4] Checking PDF page count...")
    check_pdf_pages("Codebook.pdf", 25)

    if sys.platform == "win32":
        system("pause")
