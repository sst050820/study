from pathlib import Path
base=Path(r'D:\study\408\.planning\co-all-detail\drafts')
for name in ['第4章-指令系统.md','第5章-中央处理器.md']:
    p=base/name
    t=p.read_text(encoding='utf-8')
    t=t.replace('[[408/02-计算机组成原理/','[[../')
    t=t.replace('[[408/90-复习资料/','[[../../90-复习资料/')
    t=t.replace('[[408/真题分类/','[[../../真题分类/')
    p.write_text(t,encoding='utf-8')
print('Normalized final-location source links in chapters 4 and 5')
