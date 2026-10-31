from pathlib import Path
import hashlib
import json
import re

base = Path(r'D:\study\数学一')
notes = base / '07-讲义' / '线性代数'
out = base / '06-视频解读' / '_线代前三讲'
backup = out / '审查备份_20261008_175221'
expected_backup = {
    '01-行列式.md': '435e81eee70cd3a5511e6f5771a6bbdb125dac59a6c4c4a41da9cbe606ee1d3a',
    '02-矩阵.md': 'be80a9678b2a69a60cf2f50e17bf2185bac5a7b87981a90f7380abd6857efc29',
    '03-向量组.md': 'a6e679161536fb00e7a602f011ba6e92df0a9d4d7d2c00e135077d5571dd9cfa',
}

def digest(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()

def inspect(p):
    text = p.read_text(encoding='utf-8')
    lines = text.splitlines()
    issues = []
    if '\ufffd' in text:
        issues.append('包含替换字符')
    if len(re.findall(r'^# ', text, re.M)) != 1:
        issues.append('一级标题数量异常')
    if re.search(r'^#{4,} ', text, re.M):
        issues.append('出现四级或更深标题')
    if text.count('$$') % 2:
        issues.append('显示数学分隔符不成对')
    if re.sub(r'\$\$', '', text).count('$') % 2:
        issues.append('行内数学分隔符不成对')
    if re.search('当前为教材整理稿|视频融合与最终检查进行中|TODO|待核实', text):
        issues.append('残留草稿标记')
    fence = None
    cols = None
    for line in lines:
        match = re.match(r'^\s*(`{3,}|~{3,})', line)
        if match:
            token = match.group(1)
            if fence is None:
                fence = token
            elif token[0] == fence[0] and len(token) >= len(fence):
                fence = None
            cols = None
            continue
        if fence is not None:
            continue
        if line.startswith('|'):
            count = len(re.split(r'(?<!\\)\|', line)) - 2
            if cols is not None and cols != count:
                issues.append('表格列数不一致: ' + line[:70])
            cols = count
        else:
            cols = None
    if fence is not None:
        issues.append('代码围栏未闭合')
    stack = []
    for action, env in re.findall(r'\\(begin|end)\{([^}]+)\}', text):
        if action == 'begin':
            stack.append(env)
        elif not stack or stack.pop() != env:
            issues.append('数学环境闭合次序异常: ' + env)
    if stack:
        issues.append('未闭合数学环境: ' + ', '.join(stack))
    for target in re.findall(r'\[\[([^\]|]+)(?:\|[^\]]+)?\]\]', text):
        target = target.split('#', 1)[0]
        if not target:
            continue
        destination = base / target
        if not destination.suffix:
            destination = Path(str(destination) + '.md')
        if not destination.is_file():
            issues.append('双链目标缺失: ' + target)
    return {
        '文档': p.name, '字符数': len(text), '行数': len(lines),
        'SHA256': digest(p), '排版问题': issues,
    }

audit = [inspect(notes / name) for name in expected_backup]
archive = []
for name, expected in expected_backup.items():
    p = backup / name
    actual = digest(p) if p.is_file() else None
    archive.append({'文档': name, '备份SHA256': actual, '与审查前记录相符': actual == expected})

calculations = []
for name in ['计算核对记录.json', '审查计算记录.json']:
    items = json.loads((out / name).read_text(encoding='utf-8'))
    calculations.append({'文件': name, '条数': len(items), '相符条数': sum(item['相符'] for item in items)})

result = {
    '日期': '2026-10-08', '状态': '用户要求的三讲教材与视频审查',
    '原稿备份目录': str(backup), '备份核对': archive,
    '当前笔记': audit, '选取计算核对': calculations,
    '审查说明文档': [inspect(out / name) for name in ['前三讲审查报告.md', '教材视频核对与范围.md', '工作进度.md']],
    '覆盖边界': '教材103页逐页复核；17段视频用既有语音转写重点段落和603张既有时间线抽样帧复核，并非全部原始帧或每次教学画面变化。',
}
(out / '排版与文件摘要.json').write_text(json.dumps(audit, ensure_ascii=False, indent=2), encoding='utf-8')
(out / '审查文件摘要.json').write_text(json.dumps(result, ensure_ascii=False, indent=2), encoding='utf-8')
print(json.dumps(result, ensure_ascii=False, indent=2))
if any(item['排版问题'] for item in audit + result['审查说明文档']) or any(not item['与审查前记录相符'] for item in archive) or any(item['条数'] != item['相符条数'] for item in calculations):
    raise SystemExit(1)
