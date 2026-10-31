from pathlib import Path
import re
import json
import hashlib

work = Path(r'D:\study\408\.planning\ds-ch2-detail')
notes = Path(r'D:\study\408\01-数据结构\02-知识结构文档')
draft = work / 'ch2-draft.md'
text = draft.read_text(encoding='utf-8')

# Table cell separators cannot also be unescaped wikilink alias separators.
lines = text.splitlines()
for i, line in enumerate(lines):
    if line.startswith('|'):
        lines[i] = re.sub(r'\[\[(.*?)\]\]',
            lambda m: '[[' + re.sub(r'(?<!\\)\|', r'\\|', m.group(1)) + ']]', line)
text = '\n'.join(lines) + '\n'
text = re.sub(r'(?<![\w:])free\(', 'std::free(', text)
text = text.replace('static_cast<size_t>', 'static_cast<std::size_t>')
draft.write_text(text, encoding='utf-8')

errors = []
parts = re.findall(r'^## 第(.+?)部分：(.+)$', text, re.M)
expected = ['一','二','三','四','五','六','七','八','九','十','十一','十二','十三','十四']
if [p[0] for p in parts] != expected:
    errors.append('Part order does not match the fourteen-part map')

fence = None
math = False
blocks = 0
for number, line in enumerate(text.splitlines(), 1):
    if line.startswith('```'):
        if fence is None:
            fence = (number, line)
            blocks += 1
        else:
            fence = None
        continue
    if fence is not None:
        continue
    if line.strip() == '$$':
        math = not math
        continue
    if not math and len(re.findall(r'(?<!\\)\$', line)) % 2:
        errors.append(f'Unpaired inline math delimiter at line {number}')
if fence:
    errors.append(f'Unclosed code fence: {fence}')
if math:
    errors.append('Unclosed display math block')

links = []
for payload in re.findall(r'\[\[(.*?)\]\]', text):
    target = re.split(r'\\?\|', payload)[0].split('#')[0]
    if not target:
        continue
    path = notes / target
    if not path.suffix:
        path = path.with_suffix('.md')
    links.append(str(path.resolve()))
    if not path.is_file():
        errors.append(f'Missing link target: {target}')

for phrase in ['DeleteAllX', 'UniqueSorted', 'ReverseLinkList', 'KthFromEnd',
               'CycleEntry', 'FirstCommonNode', 'MergeSortedNodes', 'ConcatCircular',
               'FindMajority', '本章知识地图', '2027', '13–62', '25–74']:
    if phrase not in text:
        errors.append(f'Missing expected retained/source content: {phrase}')

video = work / 'sources' / 'video'
manifest = json.loads((video / 'manifest.json').read_text(encoding='utf-8'))
if len(manifest) != 12:
    errors.append('Expected twelve source videos')
for item in manifest:
    prefix = item['file'][:3]
    if not (video / (prefix + '.txt')).is_file():
        errors.append(f'Missing transcript {prefix}')
    if not (video / (prefix + '_sheet.jpg')).is_file():
        errors.append(f'Missing sampled frames {prefix}')

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

original = notes / '第2章-线性表.md'
backup = work / '第2章-线性表-改写前.md'
if not backup.is_file():
    errors.append('Original backup is missing')
elif sha(original) != sha(backup):
    errors.append('Original differs from saved backup before replacement')

report = {
    'chapter_parts': len(parts),
    'subsections': len(re.findall(r'^### ', text, re.M)),
    'characters': len(text),
    'bytes': draft.stat().st_size,
    'code_and_diagram_blocks': blocks,
    'resolved_links': len(links),
    'source_videos': len(manifest),
    'video_minutes': round(sum(x['duration_seconds'] for x in manifest) / 60, 2),
    'original_backup_sha256': sha(backup) if backup.is_file() else None,
    'draft_sha256': sha(draft),
    'errors': errors,
}
(work / 'document-check.json').write_text(json.dumps(report, ensure_ascii=False, indent=2), encoding='utf-8')
print(json.dumps(report, ensure_ascii=False, indent=2))
if errors:
    raise SystemExit(1)
