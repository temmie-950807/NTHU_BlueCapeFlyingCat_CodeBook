# 段落 hash：// xxxxxx 涵蓋下一行開始到該區塊結束的 }
cat file.cpp  | cpp -dD -P -fpreprocessed | tr -d "[:space:]" | md5sum | cut -c-6