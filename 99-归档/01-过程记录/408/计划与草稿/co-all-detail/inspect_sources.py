from pathlib import Path
import json, re, hashlib
import pdfplumber
from pypdf import PdfReader

root = Path(r'D:\study\408\02-计算机组成原理')
work = Path(r'D:\study\408\.planning\co-all-detail')
backup = work / 'originals'
sources = work / 'sources'
backup.mkdir(parents=True, exist_ok=True)
sources.mkdir(parents=True, exist_ok=True)
files = []
for path in (root / '02-知识结构文档').glob('*.md'):
    data = path.read_bytes()
    dest = backup / path.name
    if not dest.exists():
        dest.write_bytes(data)
    if hashlib.sha256(dest.read_bytes()).hexdigest() != hashlib.sha256(data).hexdigest():
        raise RuntimeError(f'Backup differs from current source: {path.name}')
    files.append({'file':path.name,'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest()})
(work/'original-manifest.json').write_text(json.dumps(files,ensure_ascii=False,indent=2),encoding='utf-8')

videos = []
for path in sorted((root/'01-计算机组成原理王道课程').glob('*.mp4')):
    m = re.match(r'(\S+)\s+(\d+)\.',path.name)
    if not m: continue
    number, chapter = m.group(1),int(m.group(2))
    videos.append({'file':path.name,'number':number,'chapter':chapter,
        'existing_transcript':str(root/'视频转写记录'/(number+'.txt')) if (root/'视频转写记录'/(number+'.txt')).exists() else None})
(sources/'video-inventory.json').write_text(json.dumps(videos,ensure_ascii=False,indent=2),encoding='utf-8')

book = root/'2027计算机组成原理_高清带书签版.pdf'
reader = PdfReader(str(book))
outline = []
def visit(items):
    for item in items:
        if isinstance(item,list): visit(item)
        else:
            try: outline.append({'title':str(item.title),'pdf_page':reader.get_destination_page_number(item)+1})
            except Exception: pass
try: visit(reader.outline)
except Exception as e: outline.append({'error':str(e)})
(sources/'book-outline.json').write_text(json.dumps(outline,ensure_ascii=False,indent=2),encoding='utf-8')
with pdfplumber.open(book) as pdf:
    early = []
    for idx in range(min(16,len(pdf.pages))):
        txt=pdf.pages[idx].extract_text() or ''
        early.append(f'--- PDF {idx+1} ---\n{txt}')
    (sources/'book-front-text.txt').write_text('\n'.join(early),encoding='utf-8')
    print(json.dumps({'book_pages':len(pdf.pages),'outline_entries':len(outline),'outline_preview':outline[:14],
        'front_text_characters':sum(len(x) for x in early),'backups':len(files),
        'videos_by_chapter':{str(c):len([v for v in videos if v['chapter']==c]) for c in range(1,8)}},ensure_ascii=False,indent=2))
