# -*- coding: utf-8 -*-
import re
fp = r'D:\study\数学一\第十三讲_多元函数微分学_精讲笔记.md'
txt = open(fp, encoding='utf-8').read()

# 修复 \atop -> \substack
# 匹配 _{内容\atop内容} 形式（内容不含花括号）
pat = re.compile(r'_\{([^{}]*?)\\atop\s*([^{}]*?)\}')
def repl(m):
    return r'_{\substack{' + m.group(1) + r'\\' + m.group(2) + r'}}'

new_txt, n = pat.subn(repl, txt)
open(fp, 'w', encoding='utf-8').write(new_txt)
print(f'修复 atop: {n} 处')

# 验证残留
remain = new_txt.count('\\atop')
print(f'残留 atop: {remain} 处')
remain_left = new_txt.count('\\left.')
print(f'残留 left.: {remain_left} 处')
