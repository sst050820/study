from pathlib import Path
import os, json, re, hashlib, shutil, subprocess, time
from collections import Counter,defaultdict
from concurrent.futures import ThreadPoolExecutor,as_completed
from pypdf import PdfReader

base=Path(r'D:\study\408')
out=Path(r'D:\study\学习计划\.planning\20261009-material-audit')
out.mkdir(parents=True,exist_ok=True)
excluded_names={'.history','.planning','.model-cache','.obsidian','.git','.vscode','视频转写记录','视频转写与画面核对'}
files=[]; excluded=[]
for root,dirs,names in os.walk(base):
    for name in list(dirs):
        if name in excluded_names or '备份' in name or name.lower() in {'backup','backups'}:
            excluded.append(str(Path(root)/name));dirs.remove(name)
    for name in names:
        files.append(Path(root)/name)
plans={p.name:p.read_text(encoding='utf-8-sig') for p in Path(r'D:\study\学习计划').glob('0[0-3]-*.md')}

pdfs=[];hash_groups=defaultdict(list)
for p in sorted(x for x in files if x.suffix.lower()=='.pdf'):
    rel=p.relative_to(base)
    item={'path':str(p),'relative_path':str(rel),'bytes':p.stat().st_size}
    reader=PdfReader(p)
    item['pages']=len(reader.pages)
    item['first_page_dimensions_pt']=[float(reader.pages[0].mediabox.width),float(reader.pages[0].mediabox.height)]
    item['native_text_samples']=[{'page':i+1,'text':(reader.pages[i].extract_text() or '')[:2500]} for i in range(min(2,len(reader.pages)))]
    item['sha256']=hashlib.sha256(p.read_bytes()).hexdigest()
    hash_groups[item['sha256']].append(str(p))
    if '02-历年真题' in p.parts:
        y=re.search(r'20\d\d',p.stem)
        item.update(category='1有明确日程',purpose=('年度答案订正' if '答案' in p.parts else '年度完整试卷'),schedule=f'{y[0]}年；11/4—12/8隔日开卷，翌日订正' if y else None)
    elif '03-分章讲义' in p.parts:
        item.update(category='3未纳入可选',purpose=('背诵知识点汇总' if p.stem=='背诵知识点' else '大题知识/方法汇总' if p.stem=='大题' else '分章知识/方法讲义'),schedule='无独立翻阅日程；对应知识已经主教材及笔记安排')
    elif '04-知识图谱' in p.parts:
        item.update(category='3未纳入可选',purpose='数据结构知识图谱PDF导出',schedule='没有独立阅读此PDF的日程；DS已经二轮完成')
    elif str(rel).startswith('01-数据结构'):
        item.update(category='4已学完成',purpose='王道2027数据结构主教材',schedule='二轮已完成；到期错题及真题失分按需回查')
    else:
        subject=('计组' if str(rel).startswith('02-') else 'OS' if str(rel).startswith('03-') else '网络')
        item.update(category='1有明确日程',purpose=f'王道2027{subject}主教材',schedule={'计组':'10/9—16第3章未完成部分及4—7章；1—2章已有进度完成后按需回查','OS':'10/17—25全五章','网络':'10/26—11/1全六章'}[subject])
    item['explicit_plan_filename_mentions']=[n for n,t in plans.items() if p.name in t]
    pdfs.append(item)

video_files=sorted(x for x in files if x.suffix.lower() in {'.mp4','.mkv','.mov','.avi','.flv','.webm','.m4v'})
ffprobe=shutil.which('ffprobe')
def probe_video(p):
    result={'path':str(p),'bytes':p.stat().st_size}
    if not ffprobe:return dict(result,duration_seconds=None,error='ffprobe unavailable')
    try:
        r=subprocess.run([ffprobe,'-v','error','-show_entries','format=duration','-of','json',str(p)],capture_output=True,text=True,encoding='utf-8',timeout=20)
        if r.returncode:raise RuntimeError(r.stderr[:200])
        result['duration_seconds']=float(json.loads(r.stdout)['format']['duration'])
    except Exception as e:result.update(duration_seconds=None,error=str(e))
    return result
video_results=[]
with ThreadPoolExecutor(max_workers=4) as pool:
    futures=[pool.submit(probe_video,p) for p in video_files]
    for f in as_completed(futures):video_results.append(f.result())
video_results.sort(key=lambda x:x['path'])
video_groups=defaultdict(list)
for v in video_results:
    parts=Path(v['path']).relative_to(base).parts
    video_groups['\\'.join(parts[:2])].append(v)
video_packages=[]
for group,items in sorted(video_groups.items()):
    ds=group.startswith('01-')
    video_packages.append({'path':str(base/group),'files':len(items),'duration_seconds':sum(v['duration_seconds'] or 0 for v in items),'duration_hours':round(sum(v['duration_seconds'] or 0 for v in items)/3600,2),'metadata_success':sum(v['duration_seconds'] is not None for v in items),'category':'4已学完成' if ds else '2仅按需查漏','plan_status':'DS二轮完成不重新看完整课程' if ds else '总计划允许知识不足时查看对应片段；未安排全课程观看','chapter_packages':dict(Counter(str(Path(v['path']).relative_to(base).parent) for v in items)),'items':items})

mds=[]
for p in sorted(x for x in files if x.suffix.lower()=='.md'):
    t=p.read_text(encoding='utf-8-sig',errors='replace');rel=p.relative_to(base)
    record={'path':str(p),'bytes':p.stat().st_size,'heading_samples':re.findall(r'^#{1,2}\s+(.+)$',t,re.M)[:20]}
    if '真题分类' in p.parts:
        record.update(category='2仅按需查漏',purpose='2009—2024分章真题题干；题型查漏，无独立全量刷此包日程',declared_question_count=re.search(r'题目数量：(\d+)道',t).group(1) if re.search(r'题目数量：(\d+)道',t) else None,years=sorted(set(re.findall(r'^## (20\d\d)年',t,re.M))))
    elif '02-知识结构文档' in p.parts:
        record.update(category='4已学完成' if str(rel).startswith('01-') else '1有明确日程',purpose='主教材/本地课程派生章节笔记，作为二轮复盘和验收入口，不是另一套教材',contains_lecture_material='视频' in t,derived_graph_lecture_references=[x for x in ['03-分章讲义','04-知识图谱'] if x in t])
    elif '04-知识图谱' in p.parts:
        record.update(category='2仅按需查漏',purpose='旧版知识图谱的文本导出，与现有章节笔记有大量同题内容；没有单独全读安排')
    else:
        record.update(category='5重复副本/过程记录',purpose='真题整理索引/补齐说明；查资料来源，不新增学习任务')
    mds.append(record)

code_files=[p for p in files if p.suffix.lower()=='.cpp']
code_groups=defaultdict(list)
for p in sorted(code_files):code_groups[str(p.parent)].append(str(p))
diagram_files=[p for p in files if p.suffix.lower() in {'.xmind','.png'}]
diagrams=defaultdict(list)
for p in diagram_files:
    rel=p.relative_to(base);group='\\'.join(rel.parts[:3])
    diagrams[group].append(str(p))
others=[{'path':str(p),'category':'5重复副本/过程记录' if p.suffix.lower() in {'.exe','.json'} else '需人工核对','extension':p.suffix.lower()} for p in files if p.suffix.lower() not in {'.pdf','.md','.mp4','.mkv','.mov','.avi','.flv','.webm','.m4v','.cpp','.png','.xmind'}]
payload={'scope':'All current 408 study files excluding hidden caches/history/planning, backup folders and transcript records; PDF structure and first two native text samples only; videos metadata only; no full-book/full-video claim. Existing four plans read only.','exclusions':excluded,'extension_counts':dict(Counter(p.suffix.lower() for p in files)),'pdfs':pdfs,'identical_pdf_duplicates':[v for v in hash_groups.values() if len(v)>1],'video_packages':video_packages,'markdown':mds,'code_packages':[{'path':k,'files':len(v),'items':v,'category':'3未纳入可选','purpose':'DS算法C++实现；尚无逐文件默写/运行安排；可替换既有DS错题维护'} for k,v in sorted(code_groups.items())],'diagram_packages':[{'path':str(base/k),'files':len(v),'items':sorted(v),'category':'2仅按需查漏','purpose':'章节笔记嵌入图/知识图谱可视化与XMind可编辑源；格式版本不当新增教材'} for k,v in sorted(diagrams.items())],'other_files':others,'true_paper_missing_years':[],'unscheduled_whole_papers':'2009—2026所有年度试卷已排，没有未排年份；分章题库2009—2024没有全量另刷日程，2025/2026不在分类包但在年度计划。'}
(out/'408-report.json').write_text(json.dumps(payload,ensure_ascii=False,indent=2),encoding='utf8')
print(json.dumps({'extension_counts':payload['extension_counts'],'pdf_category_counts':dict(Counter(p['category'] for p in pdfs)),'videos':[{k:v for k,v in x.items() if k!='items'} for x in video_packages],'pdf_exact_duplicates':payload['identical_pdf_duplicates'],'code_groups':[{k:v for k,v in x.items() if k!='items'} for x in payload['code_packages']],'other_files':others,'exclusions':excluded},ensure_ascii=False))
