from pathlib import Path
import re, json, hashlib

base=Path(r'D:\study\408\.planning\co-all-detail')
final=Path(r'D:\study\408\02-计算机组成原理\02-知识结构文档')
manifest=json.loads((base/'original-manifest.json').read_text(encoding='utf-8'))
report=[]
for entry in manifest:
    name=entry['file']; p=base/'drafts'/name
    issues=[]
    backup=base/'originals'/name
    backup_ok=hashlib.sha256(backup.read_bytes()).hexdigest()==entry['sha256']
    if not backup_ok: issues.append('backup hash mismatch')
    if not p.exists():
        report.append({'file':name,'issues':['draft missing']}); continue
    t=p.read_text(encoding='utf-8-sig'); lines=t.splitlines()
    fences=[i+1 for i,line in enumerate(lines) if re.match(r'^\s*```',line)]
    if len(fences)%2: issues.append('unbalanced code fences')
    if '```mermaid' in t: issues.append('Mermaid remains')
    if '\ufffd' in t: issues.append('replacement character')
    if re.search(r'待补充|待编写|TODO|TBD',t): issues.append('possible placeholder')
    math_count=sum(1 for line in lines if re.match(r'^(?:>\s*)?\s*\$\$\s*$',line))
    if math_count%2: issues.append('unbalanced display math fences')
    in_code=False; table_width=None
    for number,line in enumerate(lines,1):
        if re.match(r'^\s*```',line):
            in_code=not in_code; table_width=None; continue
        if in_code: continue
        if line.startswith('|'):
            width=len(re.findall(r'(?<!\\)\|',line))
            if table_width is None: table_width=width
            elif width!=table_width: issues.append({'table_pipe_count':{'line':number,'expected':table_width,'actual':width}})
        else: table_width=None
    maps=re.findall(r'^[│ ]*[├└]─\s*(第[一二三四五六七八九十百]+部分：[^\n]+)',t,re.M)
    parts=re.findall(r'^##\s*(第[一二三四五六七八九十百]+部分：[^\n]+)',t,re.M)
    if name.startswith('第') and maps!=parts:
        issues.append({'map_body_difference':{'map':maps,'body':parts}})
    links=[]
    for raw in re.findall(r'\[\[([^\]]+)\]\]',t):
        target=raw.replace('\\|','|').split('|',1)[0].split('#',1)[0]
        if not target: continue
        dest=final/target
        if dest.suffix.lower() not in ['.md','.pdf','.mp4','.png','.jpg','.txt']:
            dest=Path(str(dest)+'.md')
        # Files in this batch may be drafts until publication.
        exists=dest.exists() or (dest.parent==final and (base/'drafts'/dest.name).exists())
        if not exists: links.append({'target':target,'resolved':str(dest)})
    if links: issues.append({'broken_links':links})
    report.append({'file':name,'bytes':p.stat().st_size,'characters':len(t),'parts':len(parts),
        'numbered_subsections':len(re.findall(r'^###\s*\d+\.\d+',t,re.M)),
        'backup_hash_ok':backup_ok,'issues':issues})
(base/'document-checks.json').write_text(json.dumps(report,ensure_ascii=False,indent=2),encoding='utf-8')
print(json.dumps(report,ensure_ascii=False,indent=2))
