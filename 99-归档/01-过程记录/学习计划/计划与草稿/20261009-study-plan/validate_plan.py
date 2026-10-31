import sys
sys.stdout.reconfigure(encoding='utf-8', errors='replace')
from pathlib import Path
from datetime import date, timedelta
import re, json, hashlib

root=Path(r'D:\study')
folder=root/'学习计划'
files=sorted(folder.glob('0[0-3]-*.md'))
errors=[]; warnings=[]; report={}
def require(ok,msg):
    if not ok: errors.append(msg)
require(len(files)==4,'正式文件应为4份')
all_notes={}
for f in root.rglob('*'):
    if f.is_file(): all_notes.setdefault(f.stem,[]).append(f)
for f in files:
    text=f.read_text(encoding='utf-8-sig')
    require('\ufffd' not in text,f'{f.name}: replacement character')
    require(text.startswith('---\n'),f'{f.name}: no frontmatter')
    fences=re.findall(r'^```.*$',text,re.M)
    require(len(fences)%2==0,f'{f.name}: unmatched code fences')
    table_width=None
    for n,line in enumerate(text.splitlines(),1):
        if line.startswith('|'):
            width=len(re.findall(r'(?<!\\)\|',line))
            if table_width is None: table_width=width
            require(width==table_width,f'{f.name}:{n}: inconsistent table columns')
        else: table_width=None
    links=re.findall(r'\[\[([^\]]+)\]\]',text)
    for link in links:
        target=link.split('|')[0].rstrip('\\').split('#')[0]
        p=root/target
        candidates=[p,p.with_suffix('.md')] if not p.suffix else [p]
        if not any(c.exists() for c in candidates):
            local=f.parent/target
            local_candidates=[local,local.with_suffix('.md')] if not local.suffix else [local]
            require(any(c.exists() for c in local_candidates) or target in all_notes,f'{f.name}: missing wikilink {target}')
    report[f.name]={'lines':len(text.splitlines()),'chars':len(text),'checkboxes':len(re.findall(r'^- \[ \]',text,re.M)),'sha256':hashlib.sha256(f.read_bytes()).hexdigest()}

daily=(folder/'01-2026-10-09至12-20每日任务表.md').read_text(encoding='utf-8')
chunks=re.split(r'^### (\d{4}-\d{2}-\d{2}) .*$',daily,flags=re.M)
dates=chunks[1::2]; bodies=chunks[2::2]
expected=[(date(2026,10,9)+timedelta(days=i)).isoformat() for i in range(73)]
require(dates==expected,'每日日期应连续73天（71学习+2考试）且不重不漏')
data=dict(zip(dates,bodies))
sentences=[]; readings=[]
for ds,body in list(data.items())[:71]:
    require(len(re.findall(r'^- \[ \] (数学|408|英语|政治)：',body,re.M))==4,f'{ds}: should have 4 subject tasks')
    m=re.search(r'长难句(\d+)—(\d+)',body)
    if m: sentences+=list(range(int(m[1]),int(m[2])+1))
    m=re.search(r'基础第(\d+)篇',body)
    if m: readings.append(int(m[1]))
require(sentences==list(range(25,89)),'长难句25—88应各安排1次')
require(readings==list(range(8,59)),'基础训练8—58应各安排1次')
require('长难句88—88' in data['2026-10-30'],'第88句应在10/30')
require('基础第58篇' in data['2026-11-28'],'第58篇应在11/28')

for y in range(2009,2027):
    math_new=[ds for ds,b in data.items() if re.search(fr'数学：{y}年数学一做卷',b)]
    math_review=[ds for ds,b in data.items() if re.search(fr'数学：{y}年数学一订正',b)]
    co_new=[ds for ds,b in data.items() if re.search(fr'408：{y}年408做卷',b)]
    co_review=[ds for ds,b in data.items() if re.search(fr'408：{y}年408订正',b)]
    require(all(len(x)==1 for x in [math_new,math_review,co_new,co_review]),f'{y}: 数学/408首次做卷及订正各1次')
    if all(len(x)==1 for x in [math_new,math_review,co_new,co_review]):
        require(date.fromisoformat(math_new[0])+timedelta(days=1)==date.fromisoformat(math_review[0]),f'{y}: 数学订正不是翌日')
        require(date.fromisoformat(co_new[0])+timedelta(days=1)==date.fromisoformat(co_review[0]),f'{y}:408订正不是翌日')
    require((root/'数学一'/'03-历年真题'/f'{y}.pdf').exists(),f'{y}数学卷不存在')
    answer_name=f'{y}年考研数学一真题解析.pdf' if y==2023 else f'{y}解析.pdf'
    require((root/'数学一'/'04-真题解析'/answer_name).exists(),f'{y}数学解析不存在')
    require((root/'408'/'90-复习资料'/'02-历年真题'/'真题'/f'{y}.pdf').exists(),f'{y}408卷不存在')
    require((root/'408'/'90-复习资料'/'02-历年真题'/'答案'/f'{y}-answer.pdf').exists(),f'{y}408答案不存在')

for y in range(2018,2027):
    fresh=[ds for ds,b in data.items() if re.search(fr'英语：{y}英一整套180',b)]
    review=[ds for ds,b in data.items() if re.search(fr'英语：{y}英一订正120',b)]
    require(len(fresh)==len(review)==1,f'{y}: 英语做卷订正各1次')
    if len(fresh)==len(review)==1:
        require(date.fromisoformat(fresh[0])+timedelta(days=1)==date.fromisoformat(review[0]),f'{y}:英语订正非翌日')
    require((Path(r'F:\study English\英语一\01-历年真题')/f'{y}.pdf').exists(),f'{y}英语卷不存在')

main=(folder/'00-2027考研全科学习总计划.md').read_text(encoding='utf-8')
require('数学210+408 240+英语120+政治60=630' in main,'普通日分钟公式不正确')
require(210+240+120+60==630,'普通日总分钟')
require(210+160+200+60==630,'英语模考日含语法总分钟')
require('210+160+200+60=630' in main,'模考预算需明确含语法')
require('第三章第三节' in main and 'PDF第92页' in main,'语法实际进度与起点')
for ds,body in list(data.items())[:71]:
    require('语法20分钟' in body,f'{ds}: missing daily grammar')
require('主语从句用于写作' in data['2026-10-09'],'语法应从实际第三章第三节开始')
require(5+15+20+15+5==60 and 5+10+20+15+10==60 and 5+20+15+20==60 and 5+15+10+20+10==60 and 15+15+25+5==60,'政治各期分钟不闭合')
require('2026年12月19—20日' in main,'考试日期')
require('可以边背词边做真题' in main,'自主单词约束')
require('60min' in data['2026-12-18'] and '做卷180' not in data['2026-12-18'],'12/18应减量不新卷')
check=(folder/'02-第二轮完整知识点与验收清单.md').read_text(encoding='utf-8-sig')
require(check.count('#### 完整知识点')==12,'线代概率12讲完整知识块')
require('七日专题循环' not in check and '按七日循环' not in check,'高数两处轮转矛盾')
require('109项' in check,'408知识清单覆盖')
result={'passed':not errors,'files':report,'days':len(dates),'study_days':71,'exam_days':2,'math_annual_sets':18,'408_annual_sets':18,'english_annual_sets':9,'sentence_remaining':len(sentences),'reading_remaining':len(readings),'errors':errors,'warnings':warnings}
(folder/'.planning'/'20261009-study-plan'/'validation.json').write_text(json.dumps(result,ensure_ascii=False,indent=2),encoding='utf-8')
print(json.dumps(result,ensure_ascii=False,indent=2))
sys.exit(0 if not errors else 1)
