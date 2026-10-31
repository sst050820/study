from pathlib import Path
from collections import Counter
from datetime import date, timedelta
import json, re, hashlib, sys

sys.stdout.reconfigure(encoding='utf-8')
root=Path(r'D:\study\学习计划')
work=root/'.planning'/'20261009-material-audit'
manifest=json.loads((work/'materials-status.json').read_text(encoding='utf-8'))
raw=json.loads((work/'all-files-inventory.json').read_text(encoding='utf-8-sig'))
errors=[]
def check(ok,message):
    if not ok:errors.append(message)
raw_pdfs={x['FullName'] for x in raw if x['Extension'].lower()=='.pdf'}
catalog_pdfs={x['path'] for x in manifest['pdfs']}
check(raw_pdfs==catalog_pdfs,'all source PDFs must be accounted for exactly once')
check(len(raw_pdfs)==215,'source PDF count')
check(sum(x['Extension'].lower()=='.mp4' for x in raw)==720,'all source video count')
check(len(manifest['candidates'])==35,'candidate group count')
for p in catalog_pdfs:check(Path(p).is_file(),'missing PDF '+p)
all_video_paths=[x['FullName'] for x in raw if x['Extension'].lower()=='.mp4']
check(all(Path(p).is_file() for p in all_video_paths),'missing source video')

report=root/'04-全部资料覆盖审查与待选清单.md'
t=report.read_text(encoding='utf-8')
check(not any(ord(c)<32 and c not in '\t\n\r' for c in t),'control character in report/Windows path')
check('\ufffd' not in t,'replacement character in report')
width=None
for n,line in enumerate(t.splitlines(),1):
    if line.startswith('|'):
        w=len(re.findall(r'(?<!\\)\|',line))
        if width is None:width=w
        check(w==width,f'table columns differ on {n}')
    else:width=None
for path in re.findall(r'`([DF]:\\[^`]+)`',t):
    check(Path(path).exists(),'bad report path '+path)
for c in manifest['candidates']:
    check(f"| {c['id']} |" in t,'candidate missing from reviewable report '+c['id'])

schedule=json.loads((root/'.planning'/'20261009-study-plan'/'schedule.json').read_text(encoding='utf-8'))
check(len(schedule)==71,'grammar study days')
for i,e in enumerate(schedule):
    check(e['date']==(date(2026,10,9)+timedelta(days=i)).isoformat(),'date drift')
    check('语法20分钟' in e['english'],'daily grammar omitted '+e['date'])
    if e['english_mock']:
        check('160min' in e['408'],'408 mock-day grammar budget '+e['date'])
        check('做卷180min' not in e['408'],'408 full mock conflicts with English full mock '+e['date'])

# Same non-English coverage is preserved, except the 9 English mock days' time reallocation.
backup=work/'before-grammar-update'
old=(backup/'01-2026-10-09至12-20每日任务表.md').read_text(encoding='utf-8')
new=(root/'01-2026-10-09至12-20每日任务表.md').read_text(encoding='utf-8')
for subject in ['数学','政治']:
    pattern=fr'^- \[ \] {subject}：.*$'
    check(re.findall(pattern,old,re.M)==re.findall(pattern,new,re.M),subject+' original tasks should be preserved')
check((backup/'02-第二轮完整知识点与验收清单.md').read_bytes()==(root/'02-第二轮完整知识点与验收清单.md').read_bytes(),'complete math/408 checklist changed unexpectedly')
old_ds=re.split(r'^### (\d{4}-\d{2}-\d{2}) .*$',old,flags=re.M)
new_ds=re.split(r'^### (\d{4}-\d{2}-\d{2}) .*$',new,flags=re.M)
for d,ob,nb in zip(old_ds[1::2],old_ds[2::2],new_ds[2::2]):
    if d<= '2026-12-18' and not next(x for x in schedule if x['date']==d)['english_mock']:
        check(re.search(r'^- \[ \] 408：.*$',ob,re.M)[0]==re.search(r'^- \[ \] 408：.*$',nb,re.M)[0],'408 task drift '+d)
check(len([x for x in schedule if x['english_mock']])==9,'mock day count')
for f in root.glob('0[0-4]-*.md'):
    text=f.read_text(encoding='utf-8-sig')
    check(not any(ord(c)<32 and c not in '\t\n\r' for c in text),'control character '+f.name)

result={'passed':not errors,'all_pdf_paths_accounted':len(catalog_pdfs),'all_video_paths_verified':len(all_video_paths),
        'candidate_groups':35,'daily_grammar_tasks':71,'english_mock_days_with_time_reallocation':9,
        'math_politics_tasks_preserved':True,'knowledge_checklist_sha256':hashlib.sha256((root/'02-第二轮完整知识点与验收清单.md').read_bytes()).hexdigest(),
        'audit_report_lines':len(t.splitlines()),'errors':errors}
(work/'audit-validation.json').write_text(json.dumps(result,ensure_ascii=False,indent=2),encoding='utf-8')
print(json.dumps(result,ensure_ascii=False,indent=2))
sys.exit(0 if not errors else 1)
