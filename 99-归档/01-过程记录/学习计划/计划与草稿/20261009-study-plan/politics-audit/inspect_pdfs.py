from pathlib import Path
import json
import pdfplumber

root = Path(r'D:\study\政治')
out = Path(r'D:\study\学习计划\.planning\20261009-study-plan\politics-audit')
out.mkdir(parents=True, exist_ok=True)
report = []
for path in sorted(root.rglob('*.pdf')):
    item = {'path': str(path), 'bytes': path.stat().st_size}
    try:
        with pdfplumber.open(path) as doc:
            item['pages'] = len(doc.pages)
            item['first_pages'] = []
            for i in range(min(12, len(doc.pages))):
                page = doc.pages[i]
                text = page.extract_text() or ''
                item['first_pages'].append({'page': i+1, 'chars': len(text), 'text': text})
    except Exception as error:
        item['error'] = repr(error)
    report.append(item)
    print(json.dumps({'path':str(path), 'pages':item.get('pages'), 'chars':[p['chars'] for p in item.get('first_pages',[])], 'error':item.get('error')}, ensure_ascii=False), flush=True)
(out/'pdf-probe.json').write_text(json.dumps(report, ensure_ascii=False, indent=2), encoding='utf-8')
