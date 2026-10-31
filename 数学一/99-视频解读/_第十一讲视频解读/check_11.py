# -*- coding: utf-8 -*-
# 第10讲笔记校验：标题层级/$配对/表格列数/例题习题覆盖/禁词/公式内裸竖线
import re, io, sys
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding="utf-8")

FP = r"D:\study\数学一\第十一讲_一元函数积分学的应用（二）_积分等式与积分不等式_精讲笔记.md"
txt = open(FP, encoding="utf-8").read()
lines = txt.split("\n")
errs = []

# 1. 标题层级
for i, ln in enumerate(lines, 1):
    if re.match(r"^#", ln):
        m = re.match(r"^(#{1,6})\s+(.*)", ln)
        lv = len(m.group(1))
        if lv > 3:
            errs.append(f"[标题层级] L{i}: {ln[:60]}")

# 2. $ 配对（逐行统计单行内的 $ 数量，奇数报错）
for i, ln in enumerate(lines, 1):
    c = ln.count("$")
    if c % 2 == 1:
        errs.append(f"[$不配对] L{i}: {ln[:80]}")

# 3. 表格列数一致性
def colcount(ln):
    return len([s for s in ln.strip().split("|")]) - 1  # 首尾|使split产生空串

for i, ln in enumerate(lines, 1):
    if ln.strip().startswith("|") and not re.match(r"^\s*\|[\s\-|]+\|\s*$", ln):
        cc = colcount(ln)
        # 找上一个表头
        j = i - 1
        while j >= 1 and not (lines[j-1].strip().startswith("|")):
            j -= 1
        # 简单处理：连续表格块内比较
        if i > 1 and lines[i-2].strip().startswith("|"):
            prev = colcount(lines[i-2])
            if cc != prev:
                errs.append(f"[表格列数] L{i}: {cc}列 vs 上一行{prev}列: {ln[:80]}")

# 4. 例题习题覆盖
for n in range(1, 12):
    if f"例11.{n}" not in txt:
        errs.append(f"[缺例] 例11.{n}")
for n in range(1, 7):
    if f"11.{n}" not in txt.replace("例11.", ""):
        if f"习题11.{n}" not in txt and f"| 11.{n} |" not in txt:
            errs.append(f"[缺习题] 11.{n}")

# 5. 禁词
for w in ["教材块", "视频块", "教材部分", "视频部分"]:
    if w in txt:
        errs.append(f"[禁词] 出现：{w}")

# 6. 公式内裸竖线：行内 $...$ 内出现 | 且不是 \vert / \big| 等已转义形式
for i, ln in enumerate(lines, 1):
    for m in re.finditer(r"\$([^$]+)\$", ln):
        inner = m.group(1)
        # 去掉 \vert \big| \Big| \bigg| 等已转义
        clean = re.sub(r"\\[a-zA-Z]*\|", "", inner)
        clean = re.sub(r"\\big\|", "", clean)
        clean = re.sub(r"\\Big\|", "", clean)
        if "|" in clean:
            errs.append(f"[公式内裸竖线] L{i}: ...{inner[:70]}...")

print("总行数:", len(lines), " 字符数:", len(txt))
if errs:
    print("发现 %d 个问题：" % len(errs))
    for e in errs:
        print(" -", e)
else:
    print("全部校验通过：标题层级OK、$配对OK、表格列数一致、例题习题全覆盖、无禁词、公式内无裸竖线")
