from pathlib import Path
from datetime import date,timedelta
from collections import Counter
import json,re,hashlib,sys

sys.stdout.reconfigure(encoding='utf-8')
base=Path(r'D:\study\学习计划')
work=base/'.planning'/'20261010-doubao-handoff'
out=base/'豆包每日提醒参考'
full=(out/'02-豆包每日学习计划完整参考.md').read_text(encoding='utf-8')
pure=(out/'01-每日固定时间表.md').read_text(encoding='utf-8')
tables=json.loads((work/'tables.json').read_text(encoding='utf-8'))
calendar=json.loads((work/'calendar.json').read_text(encoding='utf-8'))
sources=json.loads((work/'source-manifest.json').read_text(encoding='utf-8'))
errors=[]
def check(ok,msg):
    if not ok:errors.append(msg)
def minute(t):
    h,m=map(int,t.split(':'));return h*60+m
def totals(rows):
    c=Counter()
    for x in rows:
        if x['subject']!='生活休息':c[x['subject']]+=minute(x['end'])-minute(x['start'])
    return dict(c)
expected_normal={'数学':210,'408':240,'英语':120,'政治':60}
expected_english={'数学':210,'408':140,'英语':220,'政治':60}
expected_light={'数学':60,'408':60,'英语':60,'政治':60}
for key,rows in tables['tables'].items():
    check(totals(rows)==(expected_light if key=='L' else expected_english if key.startswith('E') else expected_normal),'budget '+key)
    for i,x in enumerate(rows):
        check(minute(x['end'])>minute(x['start']),'positive interval '+key)
        if i:check(rows[i-1]['end']==x['start'],'gap/overlap '+key+' '+x['start'])
    for subject,start in [('数学','08:30'),('408','14:00'),('英语','14:00')]:
        need=(subject=='数学' and key in ['M','MC','EM']) or (subject=='408' and key in ['C','MC']) or (subject=='英语' and key in ['EM','ER'])
        if need:
            check(any(x['subject']==subject and x['start']==start and minute(x['end'])-minute(x['start'])==180 for x in rows),'continuous 180 '+key+' '+subject)
for key,rows in tables['evenings'].items():
    check(sum(minute(x['end'])-minute(x['start']) for x in rows)==120,'evening total '+key)
    check(rows[0]['start']=='18:30' and rows[-1]['end']=='20:30','evening span '+key)
    check(rows[0]['end']=='18:50','grammar20 '+key)
    for i in range(1,len(rows)):check(rows[i-1]['end']==rows[i]['start'],'evening gap '+key)

heads=re.findall(r'^#### (\d{4}-\d{2}-\d{2}) 星期',full,re.M)
expected=[(date(2026,10,9)+timedelta(days=i)).isoformat() for i in range(73)]
check(heads==expected,'73 day headings in order')
check(len(calendar)==71,'71 study days')
daily_text=full.split('## 6. 每个日期的具体任务：71学习日＋2考试日',1)[1].split('## 7. 数学与408',1)[0]
chunks=re.split(r'^#### (\d{4}-\d{2}-\d{2}) .*$',daily_text,flags=re.M)
daily=dict(zip(chunks[1::2],chunks[2::2]))
sentences=[];readings=[]
for e in calendar:
    body=daily[e['date']]
    check(len(re.findall(r'^- \[[ x]\] (数学|408|英语|政治)：',body,re.M))==4,'4 subjects '+e['date'])
    check('语法20分钟' in body,'daily grammar '+e['date'])
    check('**'+e['template']+'**' in body,'template in day '+e['date'])
    match=re.search(r'长难句(\d+)—(\d+)',body)
    if match:sentences.extend(range(int(match[1]),int(match[2])+1))
    match=re.search(r'基础第(\d+)篇',body)
    if match:readings.append(int(match[1]))
check(sentences==list(range(25,89)),'sentences 25—88')
check(readings==list(range(8,59)),'readings 8—58')
check(len(re.findall(r'^- \[x\] (数学|408|英语|政治)：',daily_text,re.M))==4,'only Oct9 completed marks')
check('任务已给出，尚未确认完成' in daily['2026-10-10'],'Oct10 not complete')
check(all(not e['confirmed'] for e in calendar[1:]),'future progress must not be inferred')
check('具体习题号未逐一反馈' in full,'specific math questions not inferred')
check('12/11是英语整卷日' in full and '不能再塞180分钟完整卷' in full,'Dec11 cannot host 408 makeup full paper')
check('资料检查' in daily['2026-11-20'] and '资料检查' in daily['2026-12-04'],'both politics material checks explicit')

for y in range(2009,2027):
    check(sum(bool(re.search(fr'数学：{y}年数学一做卷',b)) for b in daily.values())==1,'math new '+str(y))
    check(sum(bool(re.search(fr'数学：{y}年数学一订正',b)) for b in daily.values())==1,'math review '+str(y))
    check(sum(bool(re.search(fr'408：{y}年408做卷',b)) for b in daily.values())==1,'408 new '+str(y))
    check(sum(bool(re.search(fr'408：{y}年408订正',b)) for b in daily.values())==1,'408 review '+str(y))
for y in range(2018,2027):
    check(sum(f'英语：{y}英一整套180' in b for b in daily.values())==1,'English new '+str(y))
    check(sum(f'英语：{y}英一订正120' in b for b in daily.values())==1,'English review '+str(y))
check(sum(e['template']=='EM' for e in calendar)==5,'math English full intersection5')
check(sum(e['template']=='MC' for e in calendar)==3,'math 408 redo intersection3')

original_check=(base/'02-第二轮完整知识点与验收清单.md').read_text(encoding='utf-8-sig')
knowledge=full.split('## 7. 数学与408的完整知识、习题与验收清单',1)[1].split('## 8. 四科资料入口',1)[0]
source_items=re.findall(r'^- \[ \] .*$',original_check,re.M)
new_items=re.findall(r'^- \[ \] .*$',knowledge,re.M)
check(source_items==new_items,'all knowledge/verification checklist items embedded verbatim')
check(knowledge.count('##### 完整知识点')==12,'12 math lectures full knowledge blocks')
check('109项完整知识点与验收' in knowledge,'408109')
candidate_ids=re.findall(r'^\| ((?:M|C|E|P)\d+) \|',full,re.M)
expected_ids=[f'{p}{i}' for p,n in [('M',12),('C',7),('E',8),('P',8)] for i in range(1,n+1)]
check(candidate_ids==expected_ids,'all35 candidate groups still not chosen')
check('已经写入固定时间表' in full and '无需再读取Codex本地00—04' in full,'self contained usage')
check('[[' not in full and '![[ ' not in full,'no unresolved Obsidian links/embeds')
for filename,text in [('full',full),('pure',pure)]:
    check(not any(ord(c)<32 and c not in '\n\r\t' for c in text),'control character '+filename)
    check('\ufffd' not in text,'replacement character '+filename)
    check(len(re.findall(r'^```',text,re.M))%2==0,'code fence balance '+filename)
    width=None
    for i,line in enumerate(text.splitlines(),1):
        if line.startswith('|'):
            w=len(re.findall(r'(?<!\\)\|',line))
            if width is None:width=w
            check(w==width,'table columns '+filename+' '+str(i))
        else:width=None
for path in re.findall(r'`([DF]:\\[^`]+)`',full):
    if 'YYYY' not in path:check(Path(path).exists(),'missing real material '+path)
for item in sources:
    check(hashlib.sha256(Path(item['path']).read_bytes()).hexdigest()==item['sha256'],'source file mutated '+item['path'])
result={'passed':not errors,'files':[{'name':p.name,'bytes':p.stat().st_size,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in out.glob('*.md')],
        'study_days':71,'exam_days':2,'timetable_templates':len(tables['tables']),'english_evening_variants':len(tables['evenings']),
        'math_new_sets':18,'408_new_sets':18,'english_new_sets':9,'sentence_tasks':len(sentences),'reading_tasks':len(readings),
        'embedded_knowledge_checkboxes':len(new_items),'candidate_groups':len(candidate_ids),'source_files_preserved':len(sources),'errors':errors}
(work/'validation.json').write_text(json.dumps(result,ensure_ascii=False,indent=2),encoding='utf-8')
print(json.dumps(result,ensure_ascii=False,indent=2))
sys.exit(0 if not errors else 1)
