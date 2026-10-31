from pathlib import Path
import json, hashlib
from pypdf import PdfReader
import pypdfium2 as pdfium
from PIL import Image, ImageDraw

root=Path(r'D:\study\数学一')
out=Path(__file__).parent
records=[]
for yr in range(2009,2027):
    files=[root/'03-历年真题'/f'{yr}.pdf']
    files+=sorted((root/'04-真题解析').glob(f'{yr}*.pdf'))
    for path in files:
        pdf=PdfReader(path)
        samples=[]
        for i in sorted(set([0,min(1,len(pdf.pages)-1),len(pdf.pages)-1])):
            txt=pdf.pages[i].extract_text() or ''
            samples.append({'page':i+1,'characters':len(txt),'text':txt[:700]})
        records.append({'path':str(path),'pages':len(pdf.pages),'samples':samples,'sha256':hashlib.sha256(path.read_bytes()).hexdigest()})
(out/'truth-paper-audit.json').write_text(json.dumps(records,ensure_ascii=False,indent=2),encoding='utf-8')
for name,slug in [('27张宇基础30讲概率.pdf','probability'),('27张宇基础30讲线代.pdf','linear-algebra')]:
    path=root/'01-基础讲义'/name
    pdf=PdfReader(path)
    info={'path':str(path),'pages':len(pdf.pages),'first_pages':[]}
    for i in range(min(20,len(pdf.pages))):
        txt=pdf.pages[i].extract_text() or ''
        info['first_pages'].append({'page':i+1,'characters':len(txt),'text':txt[:1400]})
    (out/f'{slug}-pdf-info.json').write_text(json.dumps(info,ensure_ascii=False,indent=2),encoding='utf-8')
    doc=pdfium.PdfDocument(str(path))
    thumbs=[]
    for i in range(min(16,len(pdf.pages))):
        img=doc[i].render(scale=.7).to_pil().convert('RGB')
        img.thumbnail((380,510))
        canvas=Image.new('RGB',(400,550),'#e3e3e3')
        canvas.paste(img,((400-img.width)//2,30))
        ImageDraw.Draw(canvas).text((12,8),f'PDF {i+1}',fill='black')
        thumbs.append(canvas)
    sheet=Image.new('RGB',(4*400,4*550),'white')
    for i,img in enumerate(thumbs):
        sheet.paste(img,((i%4)*400,(i//4)*550))
    sheet.save(out/f'{slug}-first16.png')
    doc.close()
print('PDF inventory complete',len(records),'paper files')
print('Probability and linear algebra first 16 pages rendered')
