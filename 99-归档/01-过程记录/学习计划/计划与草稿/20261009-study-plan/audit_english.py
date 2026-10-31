import sys; sys.stdout.reconfigure(encoding="utf-8", errors="replace")
from pathlib import Path
from pypdf import PdfReader
import json
root=Path(r'F:\study English\英语一')
out=Path(r'D:\study\学习计划\.planning\20261009-study-plan')
files=[root/'04-拓展资料'/'考研英语58篇基础阅读-真题册（2）.pdf', root/'04-拓展资料'/'88句终结长难句.pdf']+[root/'01-历年真题'/f'{y}.pdf' for y in range(2018,2027)]
results=[]
for p in files:
    r=PdfReader(str(p))
    texts=[x.extract_text() or '' for x in r.pages]
    record={'file':str(p),'pages':len(texts),'textchars':sum(map(len,texts)), 'first_excerpt':'\n'.join(texts[:4])[:3500], 'last_excerpt':'\n'.join(texts[-2:])[:1300], 'answer_pages':[i+1 for i,t in enumerate(texts) if any(s in t for s in ['参考答案','答案解析','答案与解析','参考范文','参考译文'])]}
    results.append(record)
    print(json.dumps(record,ensure_ascii=False))
(out/'english_pdf_audit.json').write_text(json.dumps(results,ensure_ascii=False,indent=2),encoding='utf-8')

