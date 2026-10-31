from pathlib import Path
from collections import Counter
from datetime import date,timedelta
import json,re,hashlib

B=Path(r'D:\study\学习计划');W=B/'.planning'/'20261026-daytime-only';O=B/'豆包每日提醒参考'
tables=json.loads((W/'tables.json').read_text(encoding='utf-8'))
cal=json.loads((W/'calendar.json').read_text(encoding='utf-8'))
full=(O/'02-豆包每日学习计划完整参考.md').read_text(encoding='utf-8')
pure=(O/'01-每日固定时间表.md').read_text(encoding='utf-8')
errors=[]
def check(ok,message):
    if not ok:errors.append(message)
def minute(t):h,m=map(int,t.split(':'));return h*60+m
def span(r):return minute(r['end'])-minute(r['start'])
expected={'R':{'数学':150,'408':150,'英语':90,'政治':60},'M':{'数学':195,'408':105,'英语':90,'政治':60},'C':{'数学':105,'408':195,'英语':90,'政治':60},'E':{'数学':105,'408':65,'英语':220,'政治':60},'L':{'数学':60,'408':60,'英语':60,'政治':60}}
for k,rows in tables.items():
    check(rows[0]['start']=='08:00' and rows[-1]['end']=='18:00',k+'边界')
    counts=Counter()
    for i,r in enumerate(rows):
        check(span(r)>0,k+'正向时段')
        if i:check(rows[i-1]['end']==r['start'],k+'重叠或空隙')
        counts[r['subject']]+=span(r)
        rendered=f"| {r['start']}—{r['end']} | {r['subject']}：{r['label']} | {span(r) if r['subject']!='休息用餐' else '—'} |"
        check(rendered in pure and rendered in full,k+'实际文档缺行'+rendered)
    check({s:counts[s] for s in expected[k]}==expected[k],k+'分科预算')
    check(counts['休息用餐']==(360 if k=='L' else 150),k+'休息总量')
    lunch=[r for r in rows if r['start']=='12:00' and r['end']=='13:00' and r['subject']=='休息用餐']
    dinner=[r for r in rows if r['start']=='17:30' and r['end']=='18:00' and r['subject']=='休息用餐']
    check(len(lunch)==len(dinner)==1,k+'用餐')
    grammar=sum(span(r) for r in rows if r['subject']=='英语' and '语法' in r['label'])
    check(grammar==20,k+'语法预算')
    mocks=[r for r in rows if '限时180分钟' in r['label']]
    check(len(mocks)==(1 if k in 'MCE' else 0),k+'单卷数量')
    check(all(span(r)==180 for r in mocks),k+'整卷连续180')
    if k=='M':check(mocks[0]['start']=='08:30',k+'模考时间')
    if k in ['C','E']:check(mocks[0]['start']=='14:00',k+'模考时间')

check(len(cal)==71,'学习日71')
check([e['date'] for e in cal]==[(date(2026,10,9)+timedelta(days=i)).isoformat() for i in range(71)],'日期连续')
check([e['date'] for e in cal if e['confirmed']]==['2026-10-09'],'进度确认边界')
papers={k:[e['paper_year'] for e in cal if e['paper_subject']==k] for k in 'MCE'}
check(papers['M']==list(range(2009,2027)),'数学18年')
check(papers['C']==list(range(2009,2027)),'40818年')
check(papers['E']==list(range(2018,2027)),'英语9年')
check(len([e for e in cal if e['paper_subject']])==45,'45套每日至多一套')
for e in cal:
    check(e['budget']==expected[e['template']],e['date']+'预算')
    check(e['budget_min']==sum(expected[e['template']].values()),e['date']+'合计')
    heading=f"### {e['date']} "
    check(heading in full,e['date']+'实际日历缺项')
    section=full.split(heading,1)[1].split('\n### ',1)[0]
    for subject in ['数学','408','英语','政治']:check(f'] {subject}：' in section,e['date']+subject+'遗漏')
    check(('语法20' in e['english']) or '语法错点20' in e['english'],e['date']+'语法任务遗漏')
    if e['paper_subject']:
        raw={'M':e['math'],'C':e['408'],'E':e['english']}[e['paper_subject']]
        check('180分钟' in raw,e['date']+'完整卷时长')
    if e['paper_subject']=='E':check('上一已做卷' in e['english'] and '不预看新卷' in e['english'],e['date']+'模考前语法不使用未做新卷')
    check(not any(x in str(e) for x in ['120分钟','630分钟','160min','30min']),e['date']+'旧预算残留')
for d in ['2026-12-19','2026-12-20']:check(f'### {d} · 考试日' in full,'考试日期遗漏')

source2=next(B.glob('02-*.md'))
orig=(W/'before'/source2.relative_to(B)).read_text(encoding='utf-8-sig')
orig_checks=[l.strip() for l in orig.splitlines() if re.match(r'\s*- \[[ x]\]',l)]
new_checks=[l.strip() for l in source2.read_text(encoding='utf-8').splitlines() if re.match(r'\s*- \[[ x]\]',l)]
check(new_checks==orig_checks,'完整知识项和题号修改或删除')
check(all(x in full for x in new_checks),'完整参考漏内嵌知识项')
old_full=(W/'before'/'豆包每日提醒参考'/'02-豆包每日学习计划完整参考.md').read_text(encoding='utf-8')
old_candidates=set(re.findall(r'^\| ([MCEP]\d+) \|',old_full,re.M))
new_candidates=set(re.findall(r'^\| ([MCEP]\d+) \|',full,re.M))
check(len(old_candidates)==35 and new_candidates==old_candidates,'35组候选丢失或增加')
for p in [*sorted(B.glob('0[0-4]-*.md')),O/'01-每日固定时间表.md',O/'02-豆包每日学习计划完整参考.md']:
    text=p.read_text(encoding='utf-8')
    check('\ufffd' not in text,str(p)+'编码')
    check(len(re.findall(r'^```',text,re.M))%2==0,str(p)+'围栏')
    check(not re.search(r'630|210分钟|240分钟：|120分钟|120min|160min|18:30|19:\d\d|20:\d\d|21:\d\d|22:\d\d|23:\d\d',text),str(p)+'旧学习时长/晚间残留')
    if p.parent==O:check('[[' not in text,str(p)+'交接依赖wikilink')
    for line in text.splitlines():
        if line.startswith('| ') and re.search(r'\d\d:\d\d—\d\d:\d\d',line):
            a,b=re.search(r'(\d\d:\d\d)—(\d\d:\d\d)',line).groups()
            check(480<=minute(a)<minute(b)<=1080,str(p)+'表外时段'+line)
source3=next(B.glob('03-*.md'))
check(source3.read_bytes()==(W/'before'/source3.relative_to(B)).read_bytes(),'03资源索引应保留')
result={'passed':not errors,'templates':{k:expected[k] for k in tables},'study_days':len(cal),'exam_days':2,'new_papers':{k:len(v) for k,v in papers.items()},'knowledge_items_preserved':len(new_checks),'candidate_groups':len(new_candidates),'backed_up_files':len(list((W/'before').rglob('*.md'))),'files':[{'path':str(p),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in [O/'01-每日固定时间表.md',O/'02-豆包每日学习计划完整参考.md']],'errors':errors}
(W/'validation.json').write_text(json.dumps(result,ensure_ascii=False,indent=2),encoding='utf-8')
print(json.dumps(result,ensure_ascii=False,indent=2))
raise SystemExit(0 if not errors else 1)
