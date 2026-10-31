from pathlib import Path
import re,json,hashlib
root=Path(r'D:\study')
path=root/'学习计划/02-第二轮完整知识点与验收清单.md'
text=path.read_text(encoding='utf-8')
source=(root/'学习计划/.planning/20261009-study-plan/408-audit/408资料审查与验收清单.md').read_text(encoding='utf-8')
source_part=source.split('## 剩余章节验收清单\n',1)[1].split('## 数据结构维护及跨科区分\n',1)[0]
checks=re.findall(r'^- \[ \] (.+)$',source_part,re.M)
missing_checks=[x for x in checks if '- [ ] '+x not in text]
lines=text.splitlines()
table_errors=[]
for i,line in enumerate(lines):
    if line.startswith('|'):
        # All tables in this artifact use plain-text cells, without pipes in links.
        count=line.count('|')
        if i and lines[i-1].startswith('|') and count!=lines[i-1].count('|'):
            table_errors.append(i+1)
links=[]
for link in re.findall(r'\[\[([^\]]+)\]\]',text):
    link=link.split('|',1)[0].split('#',1)[0]
    target=root/link
    exists=target.exists() or target.with_suffix('.md').exists()
    links.append({'target':link,'exists':exists})
result={
 'path':str(path),'characters':len(text),'lines':len(lines),
 'checkboxes':len(re.findall(r'^- \[ \]',text,re.M)),
 'math_lecture_sections':len(re.findall(r'^### 第[1-6]讲 ',text,re.M)),
 'knowledge_sections':text.count('#### 完整知识点'),
 'representative_math_sections':text.count('#### 代表验收动作'),
 'original_408_checks':len(checks),'missing_408_checks':missing_checks,
 '408_chapters':len(re.findall(r'^### (?:组成|OS|网络)',text,re.M)),
 'fence_lines':len(re.findall(r'^```',text,re.M)),
 'table_errors':table_errors,'unresolved_links':[x for x in links if not x['exists']],
 'daily_minutes':'90分钟完整知识复盘＋70分钟独立刷题＋20分钟订正＋30分钟高数维护' in text,
 'forbidden_draft_markers':re.findall(r'TODO|TBD|待补充|占位符|此处省略',text),
 'sha256':hashlib.sha256(path.read_bytes()).hexdigest()
}
assert result['math_lecture_sections']==12
assert result['knowledge_sections']==12 and result['representative_math_sections']==12
assert result['original_408_checks']==109 and not missing_checks
assert result['408_chapters']==16
assert result['fence_lines']%2==0 and not table_errors
assert result['daily_minutes'] and not result['forbidden_draft_markers']
(Path(__file__).parent/'final-checklist-validation.json').write_text(json.dumps(result,ensure_ascii=False,indent=2),encoding='utf-8')
print(json.dumps(result,ensure_ascii=False,indent=2))
