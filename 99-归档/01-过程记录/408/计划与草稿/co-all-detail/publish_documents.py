from pathlib import Path
import json, hashlib, os, re
base=Path(r'D:\study\408\.planning\co-all-detail')
target=Path(r'D:\study\408\02-计算机组成原理\02-知识结构文档').resolve()
manifest=json.loads((base/'original-manifest.json').read_text(encoding='utf-8'))
checks=json.loads((base/'document-checks.json').read_text(encoding='utf-8'))
assert len(manifest)==8 and len(checks)==8
assert all(not row['issues'] for row in checks), 'Draft document checks have unresolved issues'
pending=[]
for entry in manifest:
    name=entry['file']
    dst=(target/name).resolve()
    assert dst.parent==target, 'Destination is outside the requested notes folder'
    original=dst.read_bytes()
    archived=(base/'originals'/name).read_bytes()
    assert hashlib.sha256(original).hexdigest()==entry['sha256'], f'Original changed since backup: {name}'
    assert hashlib.sha256(archived).hexdigest()==entry['sha256'], f'Backup changed: {name}'
    data=(base/'drafts'/name).read_bytes()
    pending.append((dst,data))
results=[]
for dst,data in pending:
    temporary=dst.with_name(dst.name+'.codex-write-tmp')
    assert not temporary.exists(), f'Temporary destination already exists: {temporary}'
    temporary.write_bytes(data)
    os.replace(temporary,dst)
    written=dst.read_bytes()
    assert written==data
    t=written.decode('utf-8-sig')
    results.append({'file':dst.name,'bytes':len(written),'sha256':hashlib.sha256(written).hexdigest(),
      'parts':len(re.findall(r'^## 第[一二三四五六七八九十百]+部分：',t,re.M)),
      'written_equals_draft':True})
assert set(p.name for p in target.glob('*.md'))==set(e['file'] for e in manifest), 'Unexpected markdown file set'
(base/'published-manifest.json').write_text(json.dumps(results,ensure_ascii=False,indent=2),encoding='utf-8')
print(json.dumps(results,ensure_ascii=False,indent=2))
