# -*- coding: utf-8 -*-
# 全面扫描第13讲MD中可能导致渲染问题的LaTeX写法
import re
fp = r'D:\study\数学一\第十三讲_多元函数微分学_精讲笔记.md'
txt = open(fp, encoding='utf-8').read()
lines = txt.split('\n')

issues = []

# 1. 残留 \atop / \left. / \not\
for i, ln in enumerate(lines, 1):
    for pat, name in [(r'\\atop', 'atop'), (r'\\left\.', 'left-dot'), (r'\\not[\\\s]', 'not-slash')]:
        if re.search(pat, ln):
            issues.append(f'L{i} [{name}]: {ln[:80]}')

# 2. 检查每个 $...$ 公式内是否有裸下划线 _ 后面不跟 { 或字母（可能导致下标错误）
for i, ln in enumerate(lines, 1):
    for m in re.finditer(r'\$([^$]+)\$', ln):
        body = m.group(1)
        # _ 后面必须跟 { 或字母
        for mm in re.finditer(r'(?<![\\a-zA-Z])_(?![a-zA-Z{])', body):
            issues.append(f'L{i} [异常下标]: ...{body[max(0,mm.start()-15):mm.start()+15]}...')

# 3. 检查连续两个反斜杠 \\ 是否出现在非cases/非substack/matrix的行内公式中
for i, ln in enumerate(lines, 1):
    for m in re.finditer(r'\$([^$]+)\$', ln):
        body = m.group(1)
        if r'\\' in body and not any(env in body for env in ['cases', 'substack', 'matrix', 'vmatrix', 'aligned']):
            issues.append(f'L{i} [行内裸换行]: {body[:80]}')

# 4. 检查未闭合的花括号（每个公式内 { 和 } 数量）
for i, ln in enumerate(lines, 1):
    for m in re.finditer(r'\$\$(.*?)\$\$|\$([^$]+)\$', ln):
        body = m.group(1) if m.group(1) is not None else (m.group(2) or '')
        if body.count('{') != body.count('}'):
            issues.append(f'L{i} [花括号不配对] open={body.count("{")} close={body.count("}")}: {body[:80]}')

if issues:
    print(f'发现 {len(issues)} 个潜在问题：')
    for x in issues:
        print(' -', x)
else:
    print('未发现潜在渲染问题：无atop/left-dot/not、下标正常、无行内裸换行、花括号全配对')
