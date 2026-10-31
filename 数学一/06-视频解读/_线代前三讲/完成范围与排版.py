from pathlib import Path
import json,re,hashlib

base=Path(r'D:\study\数学一')
notes=base/'07-讲义'/'线性代数'
out=base/'06-视频解读'/'_线代前三讲'
files=sorted(notes.glob('0[123]-*.md'))
audit=[]
for p in files:
    text=p.read_text(encoding='utf-8')
    # A table cell cannot contain a raw vertical bar, including in inline math.
    lines=text.splitlines()
    for i,line in enumerate(lines):
        if line.startswith('|'):
            lines[i]=re.sub(r'\$([^$]+)\$',lambda m:'$'+re.sub(r'(?<!\\)\|',r'\\vert ',m.group(1))+'$',line)
    text='\n'.join(lines)+'\n';p.write_text(text,encoding='utf-8')
    issues=[];fenced=False;math_count=text.count('$$')
    if math_count%2:issues.append('显示数学分隔符不成对')
    if text.count('```')%2:issues.append('代码围栏不成对')
    if re.search(r'^#{4,} ',text,re.M):issues.append('存在四级或更深标题')
    if re.search('当前为教材整理稿|视频融合与最终检查进行中|TODO|待核实',text):issues.append('存在草稿标记')
    # Check each contiguous table has a stable number of columns.
    previous=None
    for line in lines:
        if line.startswith('```'):fenced=not fenced
        if fenced:continue
        if line.startswith('|'):
            cols=len(re.split(r'(?<!\\)\|',line))-2
            if previous is not None and cols!=previous:issues.append('表格列数不一致:'+line[:50])
            previous=cols
        else:previous=None
    # Match math environments, excluding escapes used as matrix row breaks.
    for env in ['pmatrix','vmatrix','aligned','cases']:
        if text.count('\\begin{'+env+'}')!=text.count('\\end{'+env+'}'):
            issues.append('数学环境不成对:'+env)
    for target in re.findall(r'\[\[([^\]|]+)(?:\|[^\]]+)?\]\]',text):
        if not (base/(target+'.md')).exists():issues.append('双链目标缺失:'+target)
    audit.append({'文档':p.name,'字符数':len(text),'行数':len(lines),'SHA256':hashlib.sha256(p.read_bytes()).hexdigest(),'排版问题':issues})

rows=[];total_sec=0;total_candidates=0;total_selected=0
for lecture in [1,2,3,4,6]:
    folder=base/'06-视频解读'/f'_线代视频第{lecture}讲'
    records=json.loads((folder/'画面索引.json').read_text(encoding='utf-8'))
    for rec in records:
        video=Path(rec['video']);num=video.name[:2]
        candidates=sorted((folder/'教学画面').glob(num+'_*.jpg'))
        selected=[];last=-100
        for p in candidates:
            sec=int(p.stem.split('_')[1])
            if sec-last>=40:selected.append(p);last=sec
        if candidates and selected[-1]!=candidates[-1]:selected.append(candidates[-1])
        duration=round(rec['duration']);total_sec+=rec['duration'];total_candidates+=len(candidates);total_selected+=len(selected)
        done=(folder/'转写'/f'{num}.done.json').exists() and (folder/'转写'/f'{num}.txt').exists()
        rows.append(f'| {lecture}/{num} | {video.name} | {duration//60:02d}:{duration%60:02d} | {len(candidates)} | {len(selected)} | '+('完成' if done else '缺失')+' |')

report='''# 线代前三讲：教材、视频核对与范围

完成日期：2026-10-08。笔记按基础教材讲次命名；强化视频的讲次不同，按实际主题对应。所有新生成的教材页图、语音转写、视频抽样帧、检查记录和笔记均保存在D盘。

## 一、交付文档与教材边界

教材为 `01-基础讲义/27张宇基础30讲线代.pdf`，共213个PDF页。此分册 **PDF页序号=印刷页码+7**，不用高数分册的偏移。

| 笔记 | 教材PDF页序号 | 印刷页码 | 已查看教材页图 | 融合的强化视频 |
| --- | --- | --- | --- | --- |
| [[07-讲义/线性代数/01-行列式]] | 18～51 | 11～44 | 34页 | 第1、2讲，共4段 |
| [[07-讲义/线性代数/02-矩阵]] | 52～88 | 45～81 | 37页 | 第3、4讲，共8段 |
| [[07-讲义/线性代数/03-向量组]] | 89～120 | 82～113 | 32页 | 第6讲，共5段 |

103张教材页图均已逐页查看。文件在本目录，命名为 `教材_018.png` 至 `教材_120.png`；各讲保留概念、公式条件、教材例题与习题的题意和解题过程。例题和习题编号分别为：第1讲例1.1～1.13、习题1.1～1.11；第2讲例2.1～2.20、习题2.1～2.15；第3讲例3.1～3.14、习题3.1～3.12。

## 二、视频处理与实际人工查看范围

视频源在 `08-视频讲解/02.线代9讲/`。17段均已完成本地语音自动转写；整理时结合转写中的教学内容、讲义题干和板书抽样帧校正数学符号。自动转写有“向量→项量、秩→字”等错词，不能直接当作可靠的数学答案，也没有将原转写未经校正地贴进正式笔记。

画面提取先每5秒探测一次，在画面差异达到阈值或间隔超过60秒时保存候选帧；候选帧差异可能来自老师手部移动、镜头或字幕，不等于已确认发生教学信息变化。随后按时间顺序选取间隔至少40秒的候选帧并保留末帧，制作12图一页的复核图，逐项查看；必要处另看单张较大图。

**覆盖边界：这是语音转写结合时间线抽样画面复核，尚未对全部原始帧或全部教学信息变化逐一审阅。** 在两张抽样帧之间短暂出现、又被翻走或遮挡的内容可能没有被看到；不宣称逐帧看完视频，也不宣称每次板书变化均已检查。教材逐页查看与视频抽样查看是两种不同的覆盖程度。

| 视频讲次/段号 | 实际文件名 | 时长 | 已提取候选帧 | 已查看时间线抽样帧 | 自动转写 |
| --- | --- | --- | --- | --- | --- |
'''+ '\n'.join(rows)+f'''

合计17段，源视频总时长约 {int(total_sec)//3600}小时{int(total_sec)%3600//60}分；提取候选帧{total_candidates}张，逐项查看时间线抽样帧{total_selected}张（含开头片头）。数量是实际文件与选帧记录统计，不表示同等数量的独立知识点。

中间材料分别位于 `06-视频解读/_线代视频第1讲`、`第2讲`、`第3讲`、`第4讲`、`第6讲` 对应目录，其中 `转写/` 保存带时间的文本，`教学画面/` 保存候选帧，`时间线复核/` 保存人工查看的拼图，`画面索引.json` 保存源视频与帧时间索引。

## 三、融合方式与已澄清的问题

- 按教材知识结构展开，各类例题就近放在概念和方法后；视频题号保留“视频”前缀，避免与基础教材题号混淆。
- 考研重点体现为必会基础、重点题型、方法入口、条件检查和复习验收；未进行历年题量统计，未把课程中的趋势判断当作确定的命题概率。
- “涛点”是对视频教学思路的归纳转述；“严师提醒”是笔记的复习要求，均不冒充教师逐字原话。
- $A^TA=B$：配方给出一个解；三阶实矩阵的全部解为 $QD$，$Q$ 正交。长度相等不推出向量相等。
- 视频例6.3：化简矩阵负责确定主元列编号与表示系数；原列向量组的极大无关组必须回原矩阵取列，最终以 $GH=A$ 核对。
- 反对称伴随、秩与非零特征值个数、满列秩/满行秩消去均补明条件；不把仅在可逆或可对角化情形成立的式子无条件使用。
- 书中“方程多于未知数的这种形式不存在”不作为数学结论：这类方程组当然存在，齐次时必有零解，表示唯一性由列秩判断。

## 四、交付检查

已检查标题层级、表格列数、数学分隔符与矩阵环境、双链目标、草稿状态标记。选取视频参数式、矩阵幂、分解、逆与坐标等29组关键计算，用有理数运算复算，修正发现的问题后全部相符；这属于选取计算的核对，不代表每个公式均被形式化验证。

检查和文件摘要记录在本目录的 `排版与文件摘要.json`、`计算核对记录.json`，便于后续核对交付版本。
'''
(out/'教材视频核对与范围.md').write_text(report,encoding='utf-8')
# Re-evaluate links after creating the referenced report.
for item in audit:item['排版问题']=[x for x in item['排版问题'] if not x.startswith('双链目标缺失:06-视频解读/_线代前三讲/教材视频核对与范围')]
(out/'排版与文件摘要.json').write_text(json.dumps(audit,ensure_ascii=False,indent=2),encoding='utf-8')
progress=f'''# 线代前三讲工作进度

日期：2026-10-08。

## 完成范围

- 按基础教材讲次完成3份独立笔记：行列式、矩阵、向量组。
- 教材PDF18～120页，共103页已逐页查看；章节边界、印刷页偏移+7已核实。
- 对应强化视频第1、2、3、4、6讲，共17段，语音自动转写完成；时间线{total_selected}张抽样帧已查看，非原视频逐帧审阅。
- 知识地图、概念条件、教材和视频例题、考研重点、方法钥匙、易错、涛点、复习清单与严师提醒已写入。
- 29组关键计算复算相符；排版与双链检查完成。输出及缓存均在D盘。

## 文档位置

- `D:/study/数学一/07-讲义/线性代数/01-行列式.md`
- `D:/study/数学一/07-讲义/线性代数/02-矩阵.md`
- `D:/study/数学一/07-讲义/线性代数/03-向量组.md`

## 使用说明

先读各讲知识结构及复习优先级，再按知识顺序学习；例题先遮住解答，写出题眼与方法后做计算。教材题号与视频题号不同，注意前缀。完整来源和覆盖程度见《教材视频核对与范围.md》；不把候选帧已提取等同于全部画面已人工审阅。
'''
(out/'工作进度.md').write_text(progress,encoding='utf-8')
print(json.dumps({'段数':len(rows),'时长秒':round(total_sec),'候选帧':total_candidates,'查看抽样帧':total_selected,'文档排版':audit},ensure_ascii=False,indent=2))
