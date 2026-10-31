from pathlib import Path
import json
from pypdf import PdfReader

base = Path(r'D:\study\408')
out = Path(r'D:\study\学习计划\.planning\20261009-study-plan\408-audit')
out.mkdir(parents=True, exist_ok=True)
books = [base/'02-计算机组成原理'/'2027计算机组成原理_高清带书签版.pdf',base/'03-操作系统'/'王道2027操作系统-高清带书签.pdf',base/'04-计算机网络'/'2027计算机网络_高清带书签版.pdf']

def outline_list(reader, items, level=1):
    result=[]
    for item in items:
        if isinstance(item,list): result += outline_list(reader,item,level+1)
        else:
            try: page=reader.get_destination_page_number(item)+1
            except Exception: page=None
            result.append({'level':level,'title':str(item.title),'pdf_page':page})
    return result

book_results=[]
for path in books:
    reader=PdfReader(path)
    samples=[]
    for i in range(min(15,len(reader.pages))):
        txt=reader.pages[i].extract_text() or ''
        samples.append({'pdf_page':i+1,'characters':len(txt),'text':txt[:6000]})
    book_results.append({'path':str(path),'bytes':path.stat().st_size,'pages':len(reader.pages),'outline':outline_list(reader,reader.outline),'first15_samples':samples})

paper_results=[]
paper_dir=base/'90-复习资料'/'02-历年真题'
for year in range(2009,2027):
    item={'year':year}
    for label,folder,name in [('question','真题',f'{year}.pdf'),('answer','答案',f'{year}-answer.pdf')]:
        path=paper_dir/folder/name
        record={'path':str(path),'exists':path.exists()}
        if path.exists():
            try:
                reader=PdfReader(path)
                indexes=sorted(set([0, min(1,len(reader.pages)-1),len(reader.pages)-1]))
                texts=[{'pdf_page':i+1,'text':(reader.pages[i].extract_text() or '')[:4000]} for i in indexes]
                record.update(bytes=path.stat().st_size,pages=len(reader.pages),samples=texts)
            except Exception as exc: record['error']=str(exc)
        item[label]=record
    paper_results.append(item)

payload={'scope':'Book outlines and first 15 native-text samples; every 2009-2026 paper and answer parsed, first two and last page sampled. No full OCR or full video viewing.', 'books':book_results,'papers':paper_results}
(out/'pdf-audit.json').write_text(json.dumps(payload,ensure_ascii=False,indent=2),encoding='utf-8')
for book in book_results:
    print(json.dumps({'path':book['path'],'pages':book['pages'],'first15_characters':[p['characters'] for p in book['first15_samples']],'outline':book['outline']},ensure_ascii=False))
for paper in paper_results:
    print(json.dumps({'year':paper['year'],**{k:{'exists':paper[k]['exists'],'pages':paper[k].get('pages'),'bytes':paper[k].get('bytes'),'sample_characters':[len(s['text']) for s in paper[k].get('samples',[])],'first_excerpt':(paper[k].get('samples',[{'text':''}])[0]['text'][:160]),'error':paper[k].get('error')} for k in ['question','answer']}},ensure_ascii=False))
