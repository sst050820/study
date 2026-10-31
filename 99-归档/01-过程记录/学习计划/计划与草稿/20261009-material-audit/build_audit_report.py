from pathlib import Path
from collections import Counter
from concurrent.futures import ThreadPoolExecutor
from datetime import datetime
import json, re, sys, subprocess, shutil, hashlib

sys.stdout.reconfigure(encoding='utf-8')
ROOT = Path(r'D:\study')
FOLDER = ROOT / '学习计划'
WORK = FOLDER / '.planning' / '20261009-material-audit'
def read(name):
    return json.loads((WORK/name).read_text(encoding='utf-8-sig'))
math = read('math-report.json')
co = read('408-report.json')
pol = read('politics-report.json')
eng = read('english-basic-inventory.json')
raw = read('all-files-inventory.json')

# Explicit choices only; no optional candidate is added to the daily schedule here.
CANDIDATES = [
('M1','数学','核心计算通关讲义87页＋7段配套视频','未排专项','选择最弱的不定积分类型，20—30分钟替换高数维护中的重复已会题'),
('M2','数学','零基础通关讲义52页＋15段视频','仅补缺，无固定排课','只有逻辑、代数、函数等卡住时补一个点；替换知识复盘，不整本重学'),
('M3','数学','三本公式手册＋25页公式大全＋Obsidian完善版','有公式回忆任务，但未指定资料','选一个版本做查表/闭卷自测，替换原5分钟公式回忆，不把各版本都背一遍'),
('M4','数学','赠送答题卡3页','未指定','旧版题号布局，适合书写/涂卡练习；近年模考按对应卷题号用匹配卡或空白纸'),
('M5','数学','1000题数一基础篇541页','未纳入','按薄弱专题选1—3题替换同类题；本地未发现配套解析，不安排整本加刷'),
('M6','数学','1987—2008原卷与解析，22年44份PDF','未纳入','仅选薄弱专题旧题，替换已有维护/刷题；不新增22套模考'),
('M7','数学','1987—2009、2010—2019两份答案速查','未指定','用于对应年度快速核分2—5分钟，详细订正仍看解析'),
('M8','数学','数学二高数完整版/默写版，各10页','未纳入，低优先','仅查两考试的共通知识，不能替代数学一完整范围'),
('M9','数学','高数教材六个附录','主课已学；附录未单列','图像变换、平面/空间图形、泰勒、双曲函数、变形技巧，按缺口替换维护'),
('M10','数学','线代强化九讲视频32段','知识已排；完整观看未排','选不熟片段替换对应知识块；基础六讲与强化九讲要按知识映射，不按章号硬套'),
('M11','数学','概率六讲视频38段','知识已排；完整观看未排','选定义条件和典型题薄弱片段替换知识复盘；全部原速21.91小时，超过12天的18小时知识预算'),
('M12','数学','高数18讲主课133段及基础导学1段','高数二轮已完成；仅按需','已有错题需要时回看片段；不默认重新看100多小时主课'),
('C1','408','数据结构代码49份CPP','未排逐份默写/运行','每次20分钟闭卷写算法、核边界/复杂度，替换既有DS维护'),
('C2','408','四科大题讲义4份PDF','未排专项','旧题截图、手写方法和考频总结；每次20—30分钟替换当天一道综合题'),
('C3','408','四科背诵知识点4份PDF','未指定','对应主题闭卷回忆10—15分钟，替换知识图回忆；旧删减标记按2027主教材核'),
('C4','408','分章讲义26份PDF','未排独立通读','按当前章查漏15—20分钟；内容与主教材重叠，旧考纲注记不决定2027取舍'),
('C5','408','知识图谱17份XMind、16张PNG、1份PDF及15篇图谱笔记','仅按需查漏','遮图重画主干10分钟，替换知识回忆；不同格式不当新教材重复学习'),
('C6','408','27篇分章真题Markdown及分类汇总','仅按需，未排另刷整套','2009—2024共752道去重题，和年度卷重叠；二轮选题可用，须回原PDF核失真题干'),
('C7','408','DS85、计组95、OS68段视频，共248段','仅按需查漏；完整观看未排','替换当天知识补缺片段；DS包缺排序，OS包只含1—4章，未发现网络视频'),
('E1','英语','语法长难句精讲33段','PDF已新增；视频未固定排课','从第三章第三节选对应片段，放进已有20分钟语法块，保留独立分析'),
('E2','英语','语法补弱49个文件＋语法点拨6段','未固定排课','遇到词性、时态、非谓语等缺口才替换语法块；补弱文件夹有词汇课和疑似重复文件'),
('E3','英语','88长难句实战训练22段','88句题目已排；视频未指定','当天句子独立分析后选讲解片段，替换长难句查解时间'),
('E4','英语','语篇精读33、直播带练10、超精读6段','58篇训练已排；视频未固定','只看当天篇章难点，替换原阅读查解时间；直播课可能对应两篇'),
('E5','英语','基础阶段答疑课1段','未排','只按实际问题查对应内容，替换回顾/查漏，不增加每日任务'),
('E6','英语','2002—2008旧卷7份','未纳入','可选个别翻译/阅读/作文素材替换旧卷补充；不建议增加7套模考'),
('E7','英语','2009—2017旧卷9份中尚未使用的部分','只有补充题型排期；没有9套整卷','若想完整覆盖需替换现有英语任务并调整目标；当前不算已做完9套'),
('E8','英语','词汇补弱30、第二轮15、核心单词精讲41段','没有固定观看日程；词量由你自管','继续自主选用；若选视频，用已有词汇时段替换，不重排新词数量'),
('P1','政治','上册组内自测＋62页独立下册自测本','仅索引提及，无专题/Day日期','上册自测按当天主题；下册只做本地有正文可核的部分，替换回忆/周复盘'),
('P2','政治','核心重点必背500句35页','作为辅助提及；未排条目','每天选错题相关2—3条判正误/改错，替换原回忆块'),
('P3','政治','2022—2025真题分析题','输出时间已有；来源题号未指定','优先2024/2025第34题单小问填入已有输出日；2023缺分析答案，不能直接订正'),
('P4','政治','肖1000材料分析题与对应解析','选择已排；材料未指定','只选材料/设问/解析齐全的小问替换现有主观题，思法末尾截断题排除'),
('P5','政治','考点清单书内练习','知识已排；练习未单列','更基础的同主题题可替换肖1000代表题，保持原练题20分钟'),
('P6','政治','下册重点会议线索与马原原理18页片段','主题泛有安排；未落实页面','对应史纲/马原回忆块用作查漏，不能替代完整年度时政及五类分析资料'),
('P7','政治','2026政治回忆版6页','仅参考；没有做卷日','缺多数选择题干/选项，只参考考点或设问，不按完整卷统计'),
('P8','政治','腿姐背诵手册逐日计划3页','仅参考','依赖缺失下册页码，不能整表照抄；与全科日程协调后仅借复习结构'),
]
codes = {x[0] for x in CANDIDATES}
catalog = []
def add(path,subject,status,choice='',note='',pages=None):
    p = Path(path)
    assert p.is_file(), str(p)
    assert not choice or choice in codes
    catalog.append({'path':str(p),'subject':subject,'status':status,'choice':choice,'note':note,'pages':pages})

for x in math['pdfs']:
    p=Path(x['path']); name=p.name
    if '03-历年真题' in str(p) or '04-真题解析' in str(p):
        year=int(re.search(r'(19|20)\d{2}', name)[0])
        add(p,'数学','明确排期' if year>=2009 else '未纳入候选','' if year>=2009 else 'M6','年度做卷/订正' if year>=2009 else '旧年专题选题',x['pages'])
        continue
    if name=='27张宇基础30讲线代.pdf':st,c='明确排期',''
    elif name=='27张宇基础30讲概率.pdf':st,c='明确排期',''
    elif name=='27张宇基础30讲（高数）.pdf':st,c='已完成维护；附录候选','M9'
    elif '核心计算' in name:st,c='未纳入候选','M1'
    elif '零基础' in name:st,c='按需补缺','M2'
    elif name=='1000题数一基础篇.pdf':st,c='未纳入候选','M5'
    elif '数学二' in str(p):st,c='未纳入候选','M8'
    elif '答案速查' in str(p):st,c='工具未指定','M7'
    elif '答题卡.pdf' in name:st,c='工具未指定','M4'
    else:st,c='公式工具未指定；版本择一','M3'
    add(p,'数学',st,c,'',x['pages'])

for x in co['pdfs']:
    p=Path(x['path']); name=p.name
    if '02-历年真题' in str(p):st,c='明确排期',''
    elif name=='大题.pdf':st,c='未纳入候选','C2'
    elif name=='背诵知识点.pdf':st,c='工具未指定','C3'
    elif '03-分章讲义' in str(p):st,c='未排独立通读','C4'
    elif '04-知识图谱' in str(p):st,c='按需查漏','C5'
    elif '01-数据结构' in str(p):st,c='已完成维护',''
    else:st,c='明确排期',''
    add(p,'408',st,c,x.get('purpose',''),x['pages'])

for x in pol['pdfs']:
    p=Path(x['path']); name=p.name
    if name=='27版肖1000试题分册.pdf':c='P4'
    elif name=='27版肖1000答案解析册.pdf':c='P4'
    elif '考点清单' in name:c='P5'
    elif '500句' in name:c='P2'
    elif '完整版' in name or '自测本' in name:c='P1'
    elif '上册专题' in name:c='P1'
    elif '下册重点' in name:c='P6'
    elif '逐日计划' in name:c='P8'
    elif name=='2026.pdf':c='P7'
    else:c='P3'
    add(p,'政治',x['status'],c,x['use_and_coverage'],x['pages'])

for x in eng['pdf']:
    p=Path(x['FullName']);name=p.name
    if '历年真题' in str(p):
        year=int(p.stem)
        st,c=('明确整卷排期','') if year>=2018 else (('补充题型已排；整卷未排','E7') if year>=2009 else ('未纳入候选','E6'))
    elif '不就是' in name:st,c='本次新增：每日20分钟',''
    elif '88句' in name or '58篇' in name:st,c='明确排期',''
    else:st,c='用户自主词汇；不排数量',''
    add(p,'英语',st,c)
assert len(catalog)==215 and len({x['path'] for x in catalog})==215

# Metadata-only reading of English videos, in parallel. Never transcribe or watch implicitly.
videos=[]
for group,names in eng['video_groups'].items():
    for name in names:
        videos.append({'path':str(Path(group)/name),'group':group})
assert len(videos)==246
ffprobe=shutil.which('ffprobe')
def duration(item):
    if not ffprobe:return dict(item,seconds=None,error='ffprobe unavailable')
    try:
        r=subprocess.run([ffprobe,'-v','error','-show_entries','format=duration','-of','json',item['path']],capture_output=True,text=True,encoding='utf-8',timeout=20)
        return dict(item,seconds=float(json.loads(r.stdout)['format']['duration']))
    except Exception as e:return dict(item,seconds=None,error=str(e))
with ThreadPoolExecutor(max_workers=8) as pool:
    videos=list(pool.map(duration,videos))
(WORK/'english-video-metadata.json').write_text(json.dumps(videos,ensure_ascii=False,indent=2),encoding='utf-8')

out=['''---
title: 全部资料覆盖审查与待选清单
updated: 2026-10-09
status: 语法已纳入；其他候选待选择
tags:
  - 考研/2027
  - 学习计划
---

# 全部资料覆盖审查与待选清单

[[00-2027考研全科学习总计划]] · [[01-2026-10-09至12-20每日任务表]] · [[03-资料索引与使用限制]]

## 结论与选择方式

英语语法已从第三章第三节接着排，每天20分钟。其他资料没有自动加进日程。请选下面的编号；同一资料已经有学习任务但没有指定版本，和完全没安排的资料，分别注明。

所有新选择优先替换原时间块，普通日仍630分钟，政治仍60分钟。要整本新增题册或重看整套课程，须重新分配目标；不会直接把它们叠到每天任务后面。

本次清点覆盖D盘408、数学一、政治和F盘英语一。当前资料共215份PDF、720段MP4；按实际目录和文件用途核对现有四份计划。数学/408视频时长已读元数据，英语视频也仅核时长。PDF读取结构、目录/书签、部分首末页与关键样页；没有逐页精读所有书，也没有看完720段视频。

## 已有主线，继续执行

| 科目 | 已安排的核心资料 | 不能误计为漏排的内容 |
|---|---|---|
| 数学 | 线代六讲、概率六讲主教材及习题；2009—2026原卷/解析 | 高数十八讲已完成，每日30分钟维护；同卷题解合并副本不是新卷 |
| 408 | 计组剩余、OS、网络2027教材及知识笔记；2009—2026原卷/答案 | DS已完成，保留维护；分类题与年度题重叠 |
| 英语 | 88句25—88、58篇8—58、每日语法；2018—2026九套；2009—2017部分题型 | 两份词汇PDF及11份词表由你自主使用，不重新排词量 |
| 政治 | 考点清单知识、肖1000选择与订正；2022—2025选择；回忆/分析输出时段 | 上册与500句虽被提及，其自测/条目尚未明确；下表列待落实来源 |

## 尚未固定安排的全部候选组

下表没有“必须全部加入”的意思。重复格式、旧计划参考、已学课程回看也列出，方便判断它们为什么没有独立作业。

| 编号 | 科目 | 资料 | 当前状态 | 若选择，怎么放进现有时间 |
|---|---|---|---|---|
''']
for c,s,label,st,method in CANDIDATES:
    out.append(f'| {c} | {s} | {label} | {st} | {method} |\n')
out.append('''
## 数学：15份教材/工具原件的逐一对应

公式M3包含190页张宇版、68页完善版打印PDF、40页随身版、25页公式大全和完善版MD，选择一个作查表工具即可。高数基础30讲实际是18讲；全套30讲由18+6+6组成，不是另外还有12讲高数没学。

| 文件实际路径 | PDF页数 | 状态 | 选择编号 |
|---|---:|---|---|
''')
for x in catalog:
    if x['subject']=='数学' and not any(k in x['path'] for k in ['03-历年真题','04-真题解析']):
        out.append(f"| `{x['path']}` | {x['pages']} | {x['status']} | {x['choice'] or '已排'} |\n")
out.append(r'''
年度40卷＋40解析：`D:\study\数学一\03-历年真题`与`D:\study\数学一\04-真题解析`。1987—2008共22年未排（M6），2009—2026共18年已排。2024—2026原卷和解析含相同题解合并内容，模考遮答案。M5《1000题》高数PDF1—356、线代357—454、概率455—541，未发现独立答案文件，不能默认靠本地完整解析订正。

当前28份数学MD：高数19份（18讲＋串联笔记）用于维护/查漏，线代6份用于已排复盘，公式完善版1份归M3，索引和交接2份为导航/制作说明；未发现概率MD包，概率用真实教材。

数学视频包：

| 课程包实际路径 | 文件数 | 总时长 | 主线与候选的关系 |
|---|---:|---:|---|
| `D:\study\数学一\08-视频讲解\01.基础30讲-高数` | 156 | 118.58小时 | 18讲主课133＋导学1归M12；零基础15归M2；核心计算7归M1 |
| `D:\study\数学一\08-视频讲解\02.线代9讲` | 32 | 14.46小时 | M10：31主课＋1导学，完整知识已排，视频只按需 |
| `D:\study\数学一\08-视频讲解\03.基础30讲-概率` | 38 | 21.91小时 | M11：六讲对应，首段统计课标题有误，按教材数理统计学习 |

线代基础六讲到强化九讲的对应：1→1/2，2→3/4，3→6，4→5，5→7/8，6→9。六讲知识完整复盘不等于按九讲标题强行全看视频。

## 408：原件、笔记和同内容版本

四本2027教材中，DS已完成，其他三本和对应26篇章节笔记已按当前进度安排。4篇章节导航只用于找材料。2009—2026的18套年度卷与18份答案全部已排，没有漏排年度。

34份分章讲义由26份章节、4份大题、4份背诵组成，都是单页大画布PDF，页数1不代表只有一页普通A4内容。

| 文件实际路径 | 用途 | 状态/选择编号 |
|---|---|---|
''')
for x in catalog:
    if x['subject']=='408' and x['choice']:
        out.append(f"| `{x['path']}` | {x['note']} | {x['status']}；{x['choice']} |\n")
out.append('''
C2的4份大题已渲染核：含旧真题截图、手写解法、知识/考频汇总，不当成4本新增独立题库。旧分章讲义出现“2023不考/删除”等注记，2027取舍以2027主教材及自己的要求核，不能只看旧讲义。

C6分章27篇真题MD条目合计753，按年份/题号去重为752（2009—2024每年47题），2011第7题跨两章重复。2025/2026不在分类MD，但年度PDF已排。部分Markdown公式/代码提取有失真，做题先核原年度PDF；不另要求再刷“753道新题”。分类题assets里的127张图是题目配图，不是127份额外教材。

C5图谱：17份XMind、16张PNG、1份数据结构PDF和15篇图谱MD；操作系统/网络图谱使用旧章节拆分，按知识匹配，不能拿图谱章号替代2027教材章号。

C1代码分组：

| 实际目录 | CPP文件数 | 选择方式 |
|---|---:|---|
''')
for x in co['code_packages']:
    out.append(f"| `{x['path']}` | {x['files']} | C1：闭卷写1个薄弱算法，核边界/复杂度 |\n")
out.append('\nC7视频包（仅读元数据，无观看完成判断）：\n\n| 实际目录 | 文件数 | 时长 | 覆盖限制 |\n|---|---:|---:|---|\n')
for x in co['video_packages']:
    note='仅1—7章，缺排序视频' if '数据结构' in x['path'] else ('仅1—4章，缺I/O课程' if '操作系统' in x['path'] else '计组对应章节按需补缺')
    out.append(f"| `{x['path']}` | {x['files']} | {x['duration_hours']}小时 | {note} |\n")
out.append('''
未发现计算机网络视频。既有网络教材和知识笔记已安排，不能把“未发现视频”理解为网络学习缺排。

## 英语：6份书、25年卷、各课程包与词表

语法扫描书289 PDF页，已核第三节起点印刷82/PDF92。其后第三—六章和附录已逐日列入英语任务；配套视频E1尚未固定加入。

| 六份教材/工具实际路径 | 现在怎样使用 |
|---|---|
''')
for x in catalog:
    if x['subject']=='英语' and '历年真题' not in x['path']:
        out.append(f"| `{x['path']}` | {x['status']} |\n")
out.append(r'''
年度25份：`F:\study English\英语一\01-历年真题`内2002—2026。2002—2008未排（E6）；2009—2017仅用部分题型补充，不算9套整卷已经排完（E7）；2018—2026九套已排。58篇混有英语二/完形素材，沿用训练单元编号，但不计成全英一模拟。

本地`02-真题解析`为空。2018—2021只有试卷，2023/2024不能凭旧索引断言客观题详解齐全；03资料索引保留已核网页入口，练前核年份题干。2022/2025/2026有本地题解文本，未逐题审校所有答案。

246个视频文件按原文件夹全量列包；同课不同文件不先认定为独立知识：

| 实际课程目录 | 文件数 | 元数据时长 | 候选编号 |
|---|---:|---:|---|
''')
for group,names in eng['video_groups'].items():
    v=[x for x in videos if x['group']==group]
    hours=sum(x['seconds'] or 0 for x in v)/3600
    dur=f'{hours:.2f}小时' if all(x['seconds'] is not None for x in v) else '时长未完整核实'
    if '语法长难句精讲' in group:code='E1'
    elif '语法补弱' in group or '语法点拨' in group:code='E2'
    elif '88长难句' in group:code='E3'
    elif '答疑' in group:code='E5'
    elif '语篇' in group:code='E4'
    else:code='E8'
    out.append(f'| `{group}` | {len(names)} | {dur} | {code} |\n')
out.append('''
E1第三章包含“写作里的应用”以及宾语、表语、同位语、主语从句长难句分析，可按每天语法主题找对应视频。E2语法补弱49文件中混有一条标为“词汇补弱”的文件，且有两条同名宾语课版本；没有据此算49节不同语法新课。

词汇2份PDF及下面11份词表继续由你自主选择，不排新增词数量、也不以词书背完为做真题前提：

''')
for x in eng['wordlists']:
    out.append(f"- `{x['FullName']}`\n")
out.append('''
## 政治：14份PDF逐一核对

| 实际路径 | 页数 | 已排与未排的边界 | 选择编号 |
|---|---:|---|---|
''')
for x in catalog:
    if x['subject']=='政治':
        out.append(f"| `{x['path']}` | {x['pages']} | {x['status']} | {x['choice']} |\n")
out.append('''
政治注意：

- 上册15页专题1—5片段是234页上册的便捷入口，非新增一轮教材。
- 下册自测的完整配套正文不齐，只对已有正文做局部自测，不能把62页全量默认背完。
- 2023卷PDF14—19有第34—38题材料/设问，未见分析题答案。选择解析齐全不代表分析解析齐全。
- 肖1000试题末尾思法材料至少一题截断；答案册不能补足缺的材料和设问。
- 2026卷6页缺多数选择题干/完整选项，仅参考考点，不能给它排独立完整模考。
- 政治专项旧计划和全科每日表知识主线/日期有冲突，以全科每日表为执行基准，选择自测后再落实来源；不能两套日程叠加。
- 肖八、肖四、完整2027下册、完整年度时政本地未发现。它们属于未持有资料，不列作已有候选；03索引已有11/20、12/4缺项检查日。

4份当前政治MD中，说明/开学资料索引为导航，政治专项03为待协调旧方案，首日学习单为planned任务。2份历史MD只作备份。不能因某篇有过去日期就算已完成。

## 不需要列入学习作业的文件

历史副本、制作过程、配置、导航、已编译程序不会自动加新学习任务。它们保留不动：

- D盘`.obsidian`及`未命名.base`：笔记库配置/视图；学习计划文件：执行和记录工具。
- 408的`.history`、`.model-cache`、`.planning`、`.vscode`、计组`视频转写记录`：备份、转写、抽帧/制作过程。7份EXE为49份CPP中的编译产物；真题JSON为分类制作数据。
- 数学的`06-视频解读`、`pdf_pages`、`_第十八讲定位`、`_讲义修订备份`：转写、抽帧/截图与旧讲义。68页公式PDF与完善版MD是同内容不同格式；导学/交接说明不算新课程。
- 政治`.计划历史`：2份旧方案/首日单备份。旧说明里提到某书，不证明本地已拥有。
- 英语`00-资料索引.md`与4份JSON：导航/词表制作配置；词表和两份词汇书是自主词汇来源。

没有删除、搬移或改写D/F盘任何原教材、题册、视频和词表。原全科计划修改前6份文件（含生成/验证脚本）已存于`.planning/20261009-material-audit/before-grammar-update`并记录哈希。

## 完整核对记录与读取边界

本文件列出全部资料组和215份PDF的分组对应；逐文件机器清单在`.planning/20261009-material-audit/materials-status.json`，包含215份PDF精确路径与状态，报告源及视频/笔记/词表清单可回查同目录。数学/408的独立教材、英语6本书、政治14本均有单独状态；年度卷按连续年份合并展示，不遗漏未安排的旧年份。

判断依据是当前文件目录、实际内容类型、目录/关键样页和全科日程；没有承诺每本资料逐题读完或各版本无错误。语法页范围来自本地目录，指定位入口；20分钟以内按代表例独立验收，不用翻页数量代替掌握。候选只有你明确选择后，才落实日期、页/题号和时间块。
''')

manifest={'date':'2026-10-09','scope':'D:\\study and F:\\study English\\英语一; live inventory; no full-read claim',
          'pdf_count':len(catalog),'video_count':720,'candidates':[dict(zip(['id','subject','label','status','method'],x)) for x in CANDIDATES],
          'pdfs':catalog,'english_videos':videos,'english_wordlists':eng['wordlists'],
          'math_notes':math['markdown_files'],'408_notes':co['markdown'],
          '408_code_packages':co['code_packages'],'408_diagram_packages':co['diagram_packages'],
          'source_reports':['math-report.json','408-report.json','politics-report.json','english-basic-inventory.json','all-files-inventory.json']}
(WORK/'materials-status.json').write_text(json.dumps(manifest,ensure_ascii=False,indent=2),encoding='utf-8')
target=FOLDER/'04-全部资料覆盖审查与待选清单.md'
target.write_text(''.join(out),encoding='utf-8')
print(json.dumps({'report':str(target),'pdf_count':len(catalog),'candidates':len(CANDIDATES),'english_videos':len(videos),'duration_errors':sum(x['seconds'] is None for x in videos),'chars':len(''.join(out))},ensure_ascii=False))
