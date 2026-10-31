from pathlib import Path
from collections import Counter, defaultdict
from datetime import date, timedelta, datetime
import json, re

out = Path(r'D:\study\学习计划\.planning\20261009-material-audit')
rp = out / '408-report.json'
r = json.loads(rp.read_text(encoding='utf-8'))
base = Path(r'D:\study\408')
r['audited_at_local'] = datetime.now().isoformat(timespec='seconds')
r['classification_legend'] = {
    '1有明确日程': '知识/刷题任务明确排到日期；并不等于每一个衍生文件均须全文阅读',
    '2仅按需查漏': '同主题已安排，原文件作为查漏/另一种呈现；无全量独立日程',
    '3未纳入可选': '没有单独排到日期的专项使用方式，用户可选择替换既有任务',
    '4已学完成': '用户确认的第二轮知识进度；不代表相应视频/文件逐个看完',
    '5重复副本/过程记录': '格式/索引/构建/提取等派生记录，不新增学习量；不等于字节相同',
}

for p in r['pdfs']:
    path = Path(p['path'])
    if '02-历年真题' in path.parts:
        year = int(re.search(r'20\d\d', path.stem)[0])
        practice = date(2026, 11, 4) + timedelta(days=2*(year-2009))
        correction = practice + timedelta(days=1)
        p['year'] = year
        p['practice_date'] = str(practice)
        p['correction_date'] = str(correction)
        p['schedule'] = f'{practice:%m/%d}做{year}卷，{correction:%m/%d}订正'
    elif '03-分章讲义' in path.parts:
        if path.stem == '大题':
            p.update(category='3未纳入可选', purpose='旧年综合题截图、手写解题、考频表及方法汇总；不是新增一套年度卷', schedule='没有单独全刷此PDF的日程；可按弱专题替换当天一道综合题')
            p['visual_probe'] = '实际渲染并查看单页大画布概览；考频表截至2022，未逐题审校或证明所有年份完整'
        elif path.stem == '背诵知识点':
            p.update(category='3未纳入可选', purpose='常考结论/公式/知识联系的旧版速记提纲；同主题知识已有主教材日程', schedule='没有单独背完该PDF的日程；可替换当日知识图回忆的10分钟')
        else:
            p.update(category='2仅按需查漏', purpose='旧版单页大画布分章讲义；同主题已有主教材/笔记日程，作为查漏的另一种呈现', schedule='未逐个排此PDF阅读全文；有疑问时回查本章，优先以2027主教材为准')
        p['version_boundary'] = '旧讲义出现2022/2023时间或删考备注，不能以其删考标记决定2027复习取舍；1页是大画布，不能按普通1页估时'
    elif '04-知识图谱' in path.parts:
        p.update(category='2仅按需查漏', purpose='数据结构知识图谱PDF导出；与PNG/XMind为不同呈现，不是独立教材', schedule='DS二轮知识已完成；图谱可作闭卷回忆后的核对工具')

for m in r['markdown']:
    p = Path(m['path'])
    if '02-知识结构文档' in p.parts:
        if '目录' in p.stem:
            m['category'] = '2仅按需查漏'
            m['purpose'] = '章节导航和材料映射，不是单独学习章节'
        elif '02-计算机组成原理' in p.parts and (p.stem.startswith('第1章') or p.stem.startswith('第2章')):
            m['category'] = '4已学完成'
            m['progress_boundary'] = '依据用户计组第3章进行中的进度；有真题失分时仍可回查1—2章'
    elif p.name == '408历年真题分类整理.md':
        m['category'] = '2仅按需查漏'
        m['purpose'] = '分章真题索引和范围说明，不是另一套题库'

for v in r['video_packages']:
    v['viewing_boundary'] = '只用ffprobe读取每条媒体时长；没有重新完整观看；科目二轮完成不能推出每条视频看过'
    v['full_playback_scheduled'] = False
    if '01-数据结构' in Path(v['path']).parts:
        v['coverage_gap'] = '现有章节包1—7，没有第8章排序视频；第8章有当前笔记/代码/2027教材'
    elif '03-操作系统' in Path(v['path']).parts:
        v['coverage_gap'] = '仅第1—4章课程包，没有第5章I/O视频；有当前第5章笔记/2027教材'
    else:
        v['coverage_gap'] = '文件在同一目录；本次不逐讲检查内容是否覆盖全部小节'

# Exact year/question inventory of categorized Markdown, not a full question correctness review.
questions = defaultdict(list)
classified = [m for m in r['markdown'] if '真题分类' in Path(m['path']).parts]
for m in classified:
    text = Path(m['path']).read_text(encoding='utf-8-sig')
    for year, section in re.findall(r'^## (20\d\d)年\s*\n(.*?)(?=^## 20\d\d年|\Z)', text, re.M | re.S):
        for number in re.findall(r'^### 第(\d+)题', section, re.M):
            questions[(year, int(number))].append(m['path'])
    m['actual_question_heading_count'] = len(re.findall(r'^### 第\d+题', text, re.M))
r['categorized_true_papers'] = {
    'root': str(base / '真题分类'),
    'category': '2仅按需查漏',
    'chapter_files': len(classified),
    'declared_total': sum(int(m['declared_question_count']) for m in classified),
    'distinct_year_question_count': len(questions),
    'choice_questions': sum(q <= 40 for y, q in questions),
    'comprehensive_questions': sum(41 <= q <= 47 for y, q in questions),
    'missing_year_question_numbers_2009_2024': [(str(y), q) for y in range(2009, 2025) for q in range(1, 48) if (str(y), q) not in questions],
    'cross_chapter_duplicates': [{'year': y, 'question_number': q, 'paths': paths} for (y, q), paths in questions.items() if len(paths) > 1],
    'unscheduled_full_rebrush_scope': '2009—2024各年1—40选择题、41—47综合题，全部有题号入口；没有再次全量刷完27份分类文件的独立日期',
    'not_in_categorized_package': [2025, 2026],
    'boundary': '分类版题干/公式/代码可见提取失真；以年度原始PDF为准。索引/题号齐全不证明每题内容与解答完整准确；机构答案未逐题校审。OS分类6章/旧图谱7章须映射当前教材5章。',
}

# Collapse diagram groups into meaningful packages, retaining every actual file path.
diagrams = [p for d in r['diagram_packages'] for p in d['items']]
groups = defaultdict(list)
for p in diagrams:
    parts = Path(p).relative_to(base).parts
    key = '\\'.join(parts[:3]) if parts[0] == '90-复习资料' else '\\'.join(parts[:2])
    groups[key].append(p)
r['diagram_packages'] = [{'path': str(base / k), 'files': len(ps), 'extension_counts': dict(Counter(Path(p).suffix for p in ps)), 'items': sorted(ps), 'category': '2仅按需查漏', 'purpose': '题干配图' if k.startswith('真题分类') else '知识图谱PNG及XMind可编辑源；与相应PDF/MD是不同呈现，没有单独全量学习日程'} for k, ps in sorted(groups.items())]

for c in r['code_packages']:
    c['source_probe'] = {'path': c['items'][0], 'first_650_chars': Path(c['items'][0]).read_text(encoding='utf-8-sig', errors='replace')[:650]}
    c['boundary'] = '读取各章一个代表代码的注释/开头确认用途，没有编译或逐算法正确性审校'

r['optional_recommendations'] = [
    {'priority': '较高', 'material': str(base / '01-数据结构' / '03-代码'), 'task': '每周选一个真题失分或DS薄弱算法，遮代码手写核心函数，手推空/单元素/重复值等适用边界，然后核代码', 'minutes': '20分钟/次', 'replace': '现有DS旧错题维护20分钟；若本周DS标记题未完成，则先完成标记题', 'why': '主动输出可补代码写不出的缺口；不重刷已完成二轮的整科', 'not_added': True},
    {'priority': '有相应失分才选', 'material': str(base / '90-复习资料' / '03-分章讲义') + '\\四科\\大题.pdf', 'task': '选PV、分页/Cache、指令过程、路由/TCP或DS应用的一个题型；遮手写答案独立完成一题，再比较方法', 'minutes': '20—30分钟/次', 'replace': '当日独立刷题100分钟中的一道综合题，不增加题量/总时间', 'why': '集中看同题型的手写步骤；原题年份已在年度卷计划内，提前见题须标熟悉', 'not_added': True},
    {'priority': '可选速记', 'material': str(base / '90-复习资料' / '03-分章讲义') + '\\四科\\背诵知识点.pdf', 'task': '闭卷写5条当前章常考结论/条件，再核对旧速记提纲与2027主教材', 'minutes': '10分钟/次', 'replace': '知识图回忆与补缺30分钟的前10分钟', 'why': '作为记忆核对工具；不能代替全章知识清单，也不能采信旧删考备注', 'not_added': True},
    {'priority': '按需', 'material': '26份分章讲义、旧知识图谱、248段课程视频', 'task': '同一知识点连续错两次时查看对应图/讲义或视频片段，再立即独立重做', 'minutes': '片段10—20分钟，最多在当天定向补缺30分钟内', 'replace': '现有定向补缺，不并列增加另一套阅读/视频课程', 'why': '三个视频包原始时长共80.94小时；全量新增会挤占二轮独立做题与订正', 'not_added': True},
]
for i, recommendation in enumerate(r['optional_recommendations'], 1):
    recommendation['id'] = f'408-A{i}'
    if i in (2, 3):
        stem = '大题' if i == 2 else '背诵知识点'
        recommendation['material'] = str(base / '90-复习资料' / '03-分章讲义')
        recommendation['files'] = [p['path'] for p in r['pdfs'] if Path(p['path']).stem == stem]
r['coverage_findings'] = {
    'independent_core_books': 4,
    'other_independent_expanded_textbooks_found': [],
    'all_annual_question_answer_pairs': '2009—2026共18对，年度做卷和翌日订正已全部安排',
    'supplementary_single_canvas_pdfs': {'chapter_handouts': 26, 'big_question_summaries': 4, 'recitation_outlines': 4, 'DS_mindmap_pdf': 1},
    'current_chapter_note_packages': {'DS': '8章+1目录；二轮已完成；3—8章为视频讲解整理', 'CO': '7章+1目录；1—2章已有完成进度，3—7章按剩余二轮安排', 'OS': '5章+1目录；笔记含旧分章讲义/图谱的重组引用', 'network': '6章+1目录；笔记含旧分章讲义/图谱的重组引用'},
    'missing_local_video_coverage': ['DS第8章排序', 'OS第5章I/O', '计算机网络整个课程包'],
    'answer_boundaries': ['2024年度答案6页：选择题只有答案字母，综合题有要点；无完整选择题解释', '2025/2026说明标机构回忆/解析版，未逐题权威审校', '2019/2021年度答案为扫描页，本轮不新增全文OCR，已有上轮首末页视觉核验记录'],
    'exact_pdf_duplicates': '75个PDF的SHA256互不相同；不同格式/衍生笔记有内容重叠，不据此声称重复文件可删',
}
r['read_boundaries'] = [
    '枚举所有未排除目录中的文件，75个PDF读取结构/页数和前2页原生文字样本；未逐页阅读全文',
    '34份单页分章PDF是大画布；4份大题额外渲染并查看整体版面，未逐题审校；没有把原生文字未提取的图像题当成已读',
    '248条视频仅metadata时长/标题和目录检查；本轮没有观看课程内容',
    '74份MD读取文本以核标题/来源引用/年份题号；知识正文未逐点与2027教材校订',
    '49份CPP枚举，7个章节各抽1份代码开头；未编译/跑程序/审校全部实现',
    '图谱及截图按包枚举并与MD引用区分用途；没有逐张读全部143PNG或展开17XMind',
    '未修改原资料与四份计划；只写独立审查产物。',
]
rp.write_text(json.dumps(r, ensure_ascii=False, indent=2), encoding='utf-8')

lines = [
    '408资料逐项审查（只读；未自动加计划）',
    f"有效文件：{sum(r['extension_counts'].values())}；扩展名计数：{r['extension_counts']}",
    '分类：1有明确日程；2仅按需查漏；3未纳入可选；4已学完成（知识进度，非逐文件看完）；5重复副本/过程记录。',
    '本次目录中独立主教材4本；没有找到额外拓展教材。90-复习资料当前仅02-历年真题、03-分章讲义、04-知识图谱。',
    '不同文件呈现同一知识不另计新增课程；全部75个PDF没有SHA256完全相同副本。',
    '未固定排期资料分组：8份专项PDF（4大题+4背诵，供选择）；26份分章PDF+1份DS图谱PDF（仅查漏）；27份分类真题MD（仅查漏）；248视频（无全看日程）；49CPP（可选默写）；15份旧图谱MD+17XMind/16图谱PNG（替代呈现）；127题干PNG（依附分类题）；2索引/说明MD、7EXE/1JSON（导航或过程）。',
    '34份分章PDF细分：DS10=8章+大题+背诵；计组9=7章+大题+背诵；OS7=5章+大题+背诵；网络8=6章+大题+背诵。',
    '17XMind来源：DS2、计组1、OS8、网络6；143PNG来源：知识图谱16（DS1/计组1/OS8/网络6）+真题分类assets题干配图127。',
    '建议编号：408-A1 算法闭卷写20分钟替换DS维护；408-A2 专题大题20—30分钟替换一道综合题；408-A3 速记提纲10分钟替换知识图回忆；408-A4 难点讲义/视频10—20分钟用既有定向补缺。均未自动加入。',
    '', '一、全部75个PDF（实际路径逐项；页数为PDF页，不是书页）',
]
for p in r['pdfs']:
    lines.extend([f"[{p['category']}] {p['path']} | {p['pages']} PDF页", f"  用途：{p['purpose']}；安排：{p['schedule']}"])
lines.extend(['', '二、课程视频包（248条均成功读取时长；无全量重看日程）'])
for v in r['video_packages']:
    lines.append(f"[{v['category']}] {v['path']} | {v['files']}条 | {v['duration_hours']:.2f}小时")
    lines.append('  '+v['plan_status']+'；'+v['coverage_gap'])
    for chapter, n in v['chapter_packages'].items():
        lines.append(f'  {base/chapter} | {n}条')
lines.append('合计80.94小时；未找到网络视频。DS二轮完成不能证明85条视频逐条看过。')
lines.extend(['', '三、笔记/分类真题/知识图谱/代码包'])
for subj, info in r['coverage_findings']['current_chapter_note_packages'].items():
    folder={'DS':'01-数据结构','CO':'02-计算机组成原理','OS':'03-操作系统','network':'04-计算机网络'}[subj]
    lines.append(f'{base/folder/"02-知识结构文档"} | {info}；主教材/课程派生笔记，不是另一套教材。')
lines.append('真题分类27份：DS8、计组7、OS6、网6；2009—2024各年1—47题，752独立题=640选择+112综合。无独立全量再次刷此包日程；2025/2026只有年度卷入口。')
for m in classified:
    lines.append(f"[2仅按需查漏] {m['path']} | {m['actual_question_heading_count']}条 | 出现年份：{','.join(m['years'])}")
lines.append('2011年第7题在DS图/查找两章重复收录，使条数合计753；不作为753道新题。分类版代码/公式有提取失真，核原始年度PDF。')
for d in r['diagram_packages']:
    lines.append(f"[{d['category']}] {d['path']} | {d['files']}文件 {d['extension_counts']} | {d['purpose']}")
lines.append('旧OS图谱按7章组织（引论、进程控制、调度死锁、存储管理、虚拟存储、输入输出、文件）；现教材5章。网络旧图谱6章，章节目录按2027主教材。')
for c in r['code_packages']:
    lines.append(f"[{c['category']}] {c['path']} | {c['files']}CPP；无逐文件运行/默写日程；可替换DS维护")
    lines.append('  '+ '；'.join(Path(p).name for p in c['items']))
for m in r['markdown']:
    if '02-知识结构文档' not in Path(m['path']).parts and '真题分类' not in Path(m['path']).parts:
        lines.append(f"[{m['category']}] {m['path']} | {m['purpose']}")
for x in r['other_files']:
    lines.append(f"[{x['category']}] {x['path']} | 编译/提取产物，不新增学习任务")
lines.extend(['', '四、可供用户选择的替换任务（不增加每天408 240分钟）'])
for a in r['optional_recommendations']:
    lines.append(f"{a['id']} {a['priority']}：{a['material']}；{a['task']}；{a['minutes']}；替换：{a['replace']}。")
lines.extend(['', '五、资料限制'])
lines.extend(r['coverage_findings']['answer_boundaries'])
lines.append('旧分章讲义出现2023不考/删除备注，不能用于2027取舍；大题中引用的仓库“其他资源/一休学长总结/强化班PDF”不等于本地确有该独立书，当前未找到。')
lines.extend(['', '六、读取与排除范围'])
lines.extend(r['read_boundaries'])
lines.extend('排除：'+p for p in r['exclusions'])
lines.append('排除对象为历史、缓存、规划、编辑器配置、转写记录；本次未删除它们，也未把历史副本算成当前教材。')
(out/'408-report.txt').write_text('\n'.join(lines)+'\n', encoding='utf-8')
print(json.dumps({'pdfs':len(r['pdfs']),'categorized_distinct':len(questions),'categorized_records':sum(len(v) for v in questions.values()),'missing_categorized':r['categorized_true_papers']['missing_year_question_numbers_2009_2024'],'txt_lines':len(lines),'outputs':[str(rp),str(out/'408-report.txt')]},ensure_ascii=False))
