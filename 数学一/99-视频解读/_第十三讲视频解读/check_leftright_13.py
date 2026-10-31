# -*- coding: utf-8 -*-
import re
fp = r'D:\study\数学一\第十三讲_多元函数微分学_精讲笔记.md'
lines = open(fp, encoding='utf-8').read().split('\n')
bad = 0
for i, ln in enumerate(lines, 1):
    # 匹配块级 $$...$$ 和行内 $...$
    for m in re.finditer(r'\$\$(.*?)\$\$|\$([^$]+)\$', ln):
        body = m.group(1) if m.group(1) is not None else (m.group(2) or '')
        nl = len(re.findall(r'\\left[.()|\\\[\]{}]', body))
        nr = len(re.findall(r'\\right[.()|\\\[\]{}]', body))
        if nl != nr:
            bad += 1
            print(f'L{i} left={nl} right={nr}: {body[:100]}')
print(f'不配对公式数: {bad}')
