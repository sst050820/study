# -*- coding: utf-8 -*-
# 切分第14讲 asr 单行长文本为分块多行文件
import os, io, sys
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding="utf-8")

WORK = r"D:\study\数学一\_第十四讲视频解读"
CHUNK = 6000
for i in range(1, 8):
    src = os.path.join(WORK, f"asr_{i:02d}.txt")
    dst = os.path.join(WORK, f"asr_{i:02d}_chunk.txt")
    if not os.path.exists(src):
        print("缺", src); continue
    txt = open(src, encoding="utf-8").read()
    chunks = [txt[j:j+CHUNK] for j in range(0, len(txt), CHUNK)]
    with open(dst, "w", encoding="utf-8") as f:
        f.write("\n".join(f"===== 第{k+1}块 =====" + c for k, c in enumerate(chunks)))
    print(f"asr_{i:02d}: {len(txt)}字 -> {len(chunks)}块")
