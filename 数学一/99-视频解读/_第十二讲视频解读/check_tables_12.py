# -*- coding: utf-8 -*-
# 表格列数独立体检 + HTML 标签嫌疑
import re, sys, io
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding="utf-8")
fp = r"D:\study\数学一\第十二讲_一元函数积分学的应用（三）_物理应用与经济应用_精讲笔记.md"
lines = open(fp, encoding="utf-8").read().split("\n")

def cc(s):
    segs = re.split(r"(?<!\\)\|", s.strip())
    return len(segs) - 1

tables, cur = [], None
for i, ln in enumerate(lines, 1):
    s = ln.strip()
    if s.startswith("|"):
        if cur is None:
            cur = {"start": i, "rows": []}
        cur["rows"].append((i, s))
    else:
        if cur:
            tables.append(cur)
            cur = None
if cur:
    tables.append(cur)

bad = 0
for t in tables:
    n = cc(t["rows"][0][1])
    for (i, s) in t["rows"][1:]:
        m = cc(s)
        if m != n:
            bad += 1
            print("列数不符 L%d: %d vs 表头%d: %s" % (i, m, n, s[:80]))
    if re.search(r"<(div|span|table|tr|td|br|img)", t["rows"][0][1], re.I):
        bad += 1
        print("HTML标签嫌疑 L%d: %s" % (t["start"], t["rows"][0][1][:80]))
print("表格总数: %d, 列数不符/标签嫌疑: %d" % (len(tables), bad))
