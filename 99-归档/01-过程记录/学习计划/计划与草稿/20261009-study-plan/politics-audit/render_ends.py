from pathlib import Path
import json
import pypdfium2 as pdfium
from PIL import Image, ImageDraw
root = Path(r'D:\study\政治')
out = Path(r'D:\study\学习计划\.planning\20261009-study-plan\politics-audit')
targets = [
('500', '2027考研政治--核心重点必背500句-赠品-腿姐陆寓丰.pdf'),
('syllabus', '2027考研政治--考点清单--腿姐陆寓丰.pdf'),
('questions', '27版肖1000试题分册.pdf'),
('handbook_full', '27腿姐《冲刺背诵手册》完整版.pdf'),
('handbook_up', '冲刺背诵手册(上册专题1-5及自测).pdf'),
('handbook_down', '冲刺背诵手册(下册重点会议线索&马原唯物论辩证法大题原理).pdf'),
('daily_plan', '腿姐背诵手册逐日计划.pdf'),
]
for short, filename in targets:
    doc = pdfium.PdfDocument(root / filename)
    n = len(doc)
    if short == 'daily_plan':
        chosen = list(range(n))
    else:
        chosen = list(range(max(0, n-2),n))
    images = []
    for number in chosen:
        page=doc[number]
        im=page.render(scale=1.6).to_pil().convert('RGB')
        im.thumbnail((760,1070))
        pane=Image.new('RGB',(780,1110),'white')
        pane.paste(im,((780-im.width)//2,30))
        ImageDraw.Draw(pane).text((10,8),f'{short} PDF page {number+1} of {n}',fill='black')
        images.append(pane)
        page.close()
    canvas=Image.new('RGB',(1560,1110*((len(images)+1)//2)),'white')
    for i,im in enumerate(images):
        canvas.paste(im,((i%2)*780,(i//2)*1110))
    canvas.save(out/f'{short}_ends.jpg',quality=90)
    print(short,n,flush=True)
    doc.close()
