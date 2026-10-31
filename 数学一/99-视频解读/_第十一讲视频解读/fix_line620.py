# -*- coding: utf-8 -*-
import io, sys
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding="utf-8")
fp = r"D:\study\数学一\第十一讲_一元函数积分学的应用（二）_积分等式与积分不等式_精讲笔记.md"
lines = open(fp, encoding="utf-8").read().split("\n")
for i, ln in enumerate(lines, 1):
    if ln.startswith("| \u2468 |"):
        lines[i-1] = "| \u2468 | $f(b)-f(a)=f'(\\xi)(b-a)$\uff08\u4e0d\u5e26\u79ef\u5206 \u2192 \u62c9\u683c\u6717\u65e5\uff09\u6216 $=\\int_a^b f'(t)\\,dt$\uff08\u5e26\u79ef\u5206 \u2192 \u725b\u987f-\u83b1\u5e03\u5c3c\u8328\uff09 | \u5224\u65ad\u7528\u54ea\u79cd\u4e2d\u503c\u5de5\u5177 |"
        print("已重写 L%d" % i)
        break
open(fp, "w", encoding="utf-8").write("\n".join(lines))
