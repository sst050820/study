from pathlib import Path
import json,hashlib,re
base=Path(r'D:\study\408\.planning\co-all-detail')
target=Path(r'D:\study\408\02-计算机组成原理\02-知识结构文档')
old=json.loads((base/'original-manifest.json').read_text(encoding='utf-8'))
checks=json.loads((base/'document-checks.json').read_text(encoding='utf-8'))
assert all(not row['issues'] for row in checks)
result=[]
for entry in old:
    name=entry['file']; final=(target/name).read_bytes(); draft=(base/'drafts'/name).read_bytes()
    assert final==draft
    assert hashlib.sha256((base/'originals'/name).read_bytes()).hexdigest()==entry['sha256']
    result.append({'file':name,'bytes':len(final),'sha256':hashlib.sha256(final).hexdigest(),'written_equals_draft':True,'original_backup_hash_ok':True})
assert len(list(target.glob('*.md')))==8
assert len(list((base/'sources'/'video').glob('*.done')))==15
(base/'published-manifest.json').write_text(json.dumps(result,ensure_ascii=False,indent=2),encoding='utf-8')
p=base/'task_plan.md'; t=p.read_text(encoding='utf-8').replace('- [ ]','- [x]'); p.write_text(t,encoding='utf-8')
with (base/'progress.md').open('a',encoding='utf-8') as f:
    f.write('\n2026-10-04 完成：七章和目录全部写回原文件，主目录仍为8份MD，没有新建同章学习副本。每章文字树状地图与教学部分对应；补齐详细过程、逐轮/逐步例题、自测和资料入口。已检查表格列数、代码/展示公式围栏、资料链接、原稿备份哈希及最终文件与草稿一致性。全部15份补充重点视频转写完成；已有35份转写保留。\n')
with (base/'findings.md').open('a',encoding='utf-8') as f:
    f.write('\n## 终稿核对要点\n\n- 原稿独有的例题、方法、自测和扩展内容按章保留；未经本次重统计的真题数量和年份不沿用。\n- 第2章完整乘除逐轮、IEEE特殊值与舍入；第3章芯片/Cache/TLB逐步演算；第4章扩展码与栈帧；第5章完整控制与流水时序已展开。\n- 第6章按2027教材区分I/O总线与通信总线，事务三个基本阶段与课程四阶段的划分，工作频率为有效数据拍频率；握手请求方与数据发送方不混同。\n- 第5章ALU标志在结果有效拍锁存；第4章参数区例题明确初始ESP=EBP。\n- 第7章将定时查询的开销预算与不停忙等区分；DMA按块计CPU开销、按拍计总线占用。\n- 95视频已生成重点抽帧，实际查看范围与教材重点扫描页分别记录在章末；没有声称完整观看全部课程或逐题读取全部教材。\n- originals目录为8份原稿可恢复备份；original-manifest.json与published-manifest.json分别记录整理前和整理后哈希。\n')
print('Final files match all eight drafts; all original backups verified; source transcriptions complete; planning record closed.')
