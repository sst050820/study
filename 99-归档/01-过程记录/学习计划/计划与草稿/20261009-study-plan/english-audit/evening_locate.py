from pathlib import Path
import json
import pypdfium2 as pdfium
root=Path(r'F:\study English\英语一\04-拓展资料')
out=Path(r'D:\study\学习计划\.planning\20261009-study-plan\english-audit')
out.mkdir(parents=True,exist_ok=True)
targets=[
('sentences','88句终结长难句.pdf',list(range(12))),
('test','考研英语58篇基础阅读-真题册（2）.pdf',list(range(5))+list(range(10,18))),
('explain','考研英语58篇基础阅读-精讲册（1）.pdf',list(range(8))),
]
report=[]
for label,filename,pages in targets:
    doc=pdfium.PdfDocument(root/filename)
    item={'label':label,'path':str(root/filename),'pages':len(doc),'sample':[]}
    for i in pages:
        if i>=len(doc):continue
        page=doc[i]
        textpage=page.get_textpage()
        text=textpage.get_text_range()
        item['sample'].append({'pdf_page':i+1,'text':text})
        textpage.close();page.close()
    report.append(item);doc.close()
(out/'evening-probe.json').write_text(json.dumps(report,ensure_ascii=False,indent=2),encoding='utf-8')
print('done',len(report))
