from pathlib import Path
import pypdfium2 as pdfium
from PIL import Image,ImageDraw
out=Path(__file__).parent
doc=pdfium.PdfDocument(r'D:\study\数学一\01-基础讲义\27张宇基础30讲概率.pdf')
for batch,start in enumerate(range(144,168,6),1):
    sheet=Image.new('RGB',(3*520,2*720),'white')
    for j,page_num in enumerate(range(start,min(start+6,168))):
        img=doc[page_num-1].render(scale=.95).to_pil().convert('RGB')
        img.thumbnail((500,670))
        canvas=Image.new('RGB',(520,720),'#e3e3e3')
        canvas.paste(img,((520-img.width)//2,30))
        ImageDraw.Draw(canvas).text((10,8),f'PDF {page_num}',fill='black')
        sheet.paste(canvas,((j%3)*520,(j//3)*720))
    sheet.save(out/f'statistics-part{batch}.png')
doc.close()
print('Statistics section pages 144-167 rendered')
