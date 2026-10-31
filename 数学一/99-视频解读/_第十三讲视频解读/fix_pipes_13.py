# -*- coding: utf-8 -*-
# 第13讲笔记修复：①#### 例13.x -> ### 例13.x；②### 13.N -> ### 习题 13.N；③公式内裸竖线 -> \vert（补空格）
import re, io, sys
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding="utf-8")

FP = r"D:\study\数学一\第十三讲_多元函数微分学_精讲笔记.md"
txt = open(FP, encoding="utf-8").read()

# ① 标题层级
txt = re.sub(r"^#### (例13\.\d+)", r"### \1", txt, flags=re.M)
# ② 习题标题（### 13.N ... -> ### 习题 13.N ...）
txt = re.sub(r"^(### )(\d+\.\d+) ", r"\1习题 \2 ", txt, flags=re.M)

# ③ 公式内裸竖线对 |X| -> \vert X\vert（仅在 $...$ / $$...$$ 内，表格分隔符不受影响）
def rep_in_formula(s):
    # 已转义形式占位保护（\vert / \big| 等）
    s2 = re.sub(r"\\[a-zA-Z]*\|", "\x00", s)
    s2 = re.sub(r"\|([^|\x00]+)\|", r"\\vert \1\\vert", s2)
    return s2

new = re.sub(r"\$[^$\n]+\$", lambda m2: rep_in_formula(m2.group(0)), txt)
open(FP, "w", encoding="utf-8").write(new)
print("处理完成")
# 统计残留裸竖线（公式内）
bad = 0
for i, ln in enumerate(new.split("\n"), 1):
    for m in re.finditer(r"\$([^$]+)\$", ln):
        clean = re.sub(r"\\[a-zA-Z]*\|", "", m.group(1))
        if "|" in clean:
            bad += 1
            print("残留 L%d: %s" % (i, m.group(1)[:70]))
print("残留裸竖线公式数:", bad)
