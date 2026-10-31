# -*- coding: utf-8 -*-
# 清洗第11讲笔记：行内/块公式内的裸竖线 | 一律转义为 \vert 系列（表格串格根治）
import re, io, sys
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding="utf-8")

FP = r"D:\study\数学一\第十一讲_一元函数积分学的应用（二）_积分等式与积分不等式_精讲笔记.md"
txt = open(FP, encoding="utf-8").read()

def fix_math(m):
    s = m.group(0)
    # 先处理已转义序列，避免字面竖线残留
    s = s.replace(r"\Big|", r"\Big\vert")   # \Big| -> \Big\vert（无字面竖线）
    s = s.replace(r"\big|", r"\big\vert")
    s = s.replace(r"\|", r"\vert")
    # 剩余字面竖线 -> \vert
    s = s.replace("|", r"\vert")
    return s

# 按行处理，跳过代码块
out_lines = []
in_code = False
for ln in txt.split("\n"):
    if ln.strip().startswith("```"):
        in_code = not in_code
        out_lines.append(ln)
        continue
    if in_code:
        out_lines.append(ln)
        continue
    # 处理行内 $...$（单美元包围，不跨越行）
    ln = re.sub(r"\$[^$\n]+\$", fix_math, ln)
    # 处理 $$...$$ 块（同一行内完整闭合）
    ln = re.sub(r"\$\$[^$\n]+\$\$", fix_math, ln)
    out_lines.append(ln)

new = "\n".join(out_lines)
open(FP, "w", encoding="utf-8").write(new)
print("清洗完成，字符数:", len(txt), "->", len(new))
