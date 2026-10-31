import sys; sys.stdout.reconfigure(encoding='utf-8',errors='replace')
import json
from pathlib import Path
from pypdf import PdfReader
root=Path(r'F:\study English\英语一')
records=json.loads(Path(r'D:\study\学习计划\.planning\20261009-study-plan\english_pdf_audit.json').read_text(encoding='utf-8'))
for x in records[2:]:
    print(Path(x['file']).name, 'pages', x['pages'], 'answer_pages', x['answer_pages'])
    r=PdfReader(x['file'])
    for i,p in enumerate(r.pages):
        t=p.extract_text() or ''
        if any(w in t for w in ['参考答案','答案解析','参考范文','Section III','Section Ⅲ','英语（二）','英语(二)','英语（一）试题及参考答案']):
            print('PAGE',i+1,t[:260].replace('\n',' '))
