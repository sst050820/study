import sys,json
from pathlib import Path
sys.stdout.reconfigure(encoding='utf-8',errors='replace')
work=Path(r'D:\study\学习计划\.planning\20261009-material-audit')
rows=json.loads((work/'all-files-inventory.json').read_text(encoding='utf-8-sig'))
root=r'F:\study English\英语一'
eng=[r for r in rows if r['Root']==root and '.obsidian' not in r['FullName']]
vids={}
for r in eng:
    if r['Extension'].lower()=='.mp4':vids.setdefault(str(Path(r['FullName']).parent),[]).append(r['Name'])
for k,v in vids.items():
    print(json.dumps({'path':k,'count':len(v),'first_names':v[:6]},ensure_ascii=False))
print('PDFS',json.dumps([r['FullName'] for r in eng if r['Extension'].lower()=='.pdf'],ensure_ascii=False))
(work/'english-basic-inventory.json').write_text(json.dumps({'pdf':[r for r in eng if r['Extension'].lower()=='.pdf'],'video_groups':vids,'wordlists':[r for r in eng if r['Extension'].lower()=='.txt']},ensure_ascii=False,indent=2),encoding='utf-8')
