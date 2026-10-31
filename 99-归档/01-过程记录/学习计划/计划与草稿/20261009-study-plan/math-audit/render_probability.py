from pathlib import Path
import pypdfium2 as pdfium
from PIL import Image,ImageDraw
out=Path(__file__).parent
doc=pdfium.PdfDocument(r'D:\study\数学一\01-基础讲义\27张宇基础30讲概率.pdf')
print('Probability PDF pages',len(doc))
for i in [8,41,70,111,133,141]:
    doc[i].render(scale=1.6).to_pil().save(out/f'probability-pdf{i+1}.png')
# At each chapter boundary, inspect the last five pages for exercise numbering.
sections=[(1,10,41),(2,42,70),(3,71,111),(4,112,133),(5,134,141),(6,142,len(doc))]
for ch,start,end in sections:
    pages=range(max(start,end-5),end+1)
    sheet=Image.new('RGB',(3*520,2*720),'white')
    for j,page_num in enumerate(pages):
        img=doc[page_num-1].render(scale=.95).to_pil().convert('RGB')
        img.thumbnail((500,670))
        canvas=Image.new('RGB',(520,720),'#e3e3e3')
        canvas.paste(img,((520-img.width)//2,30))
        ImageDraw.Draw(canvas).text((10,8),f'Chapter {ch}, PDF {page_num}',fill='black')
        sheet.paste(canvas,((j%3)*520,(j//3)*720))
    sheet.save(out/f'probability-chapter{ch}-last6.png')
doc.close()
doc=pdfium.PdfDocument(r'D:\study\数学一\01-基础讲义\27张宇基础30讲线代.pdf')
doc[6].render(scale=1.6).to_pil().save(out/'linear-algebra-pdf7.png')
doc.close()
print('Chapter boundaries and exercise pages rendered')
