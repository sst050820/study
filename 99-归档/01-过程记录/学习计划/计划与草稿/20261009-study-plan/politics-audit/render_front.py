from pathlib import Path
import pypdfium2 as pdfium
from PIL import Image, ImageDraw
root = Path(r'D:\study\政治')
out = Path(r'D:\study\学习计划\.planning\20261009-study-plan\politics-audit')
targets = [
('500', '2027考研政治--核心重点必背500句-赠品-腿姐陆寓丰.pdf', 4),
('syllabus', '2027考研政治--考点清单--腿姐陆寓丰.pdf', 10),
('answers', '27版肖1000答案解析册.pdf', 5),
('questions', '27版肖1000试题分册.pdf', 5),
('handbook_full', '27腿姐《冲刺背诵手册》完整版.pdf', 8),
('self_test', '27腿姐《冲刺背诵手册》自测本.pdf', 6),
('handbook_up', '冲刺背诵手册(上册专题1-5及自测).pdf', 5),
('handbook_down', '冲刺背诵手册(下册重点会议线索&马原唯物论辩证法大题原理).pdf', 5),
('daily_plan', '腿姐背诵手册逐日计划.pdf', 1),
]
for short, filename, count in targets:
    doc = pdfium.PdfDocument(root / filename)
    images = []
    for number in range(min(count, len(doc))):
        page = doc[number]
        im = page.render(scale=1.35).to_pil().convert('RGB')
        im.thumbnail((760, 1070))
        pane = Image.new('RGB', (780, 1110), 'white')
        pane.paste(im, ((780-im.width)//2, 30))
        ImageDraw.Draw(pane).text((10, 8), f'{short} PDF page {number+1}', fill='black')
        images.append(pane)
        page.close()
    for start in range(0, len(images), 4):
        chunk = images[start:start+4]
        canvas = Image.new('RGB', (1560, 1110*((len(chunk)+1)//2)), 'white')
        for i, im in enumerate(chunk):
            canvas.paste(im, ((i%2)*780, (i//2)*1110))
        canvas.save(out/f'{short}_{start+1}.jpg', quality=88)
    doc.close()
    print(short, len(images), flush=True)
