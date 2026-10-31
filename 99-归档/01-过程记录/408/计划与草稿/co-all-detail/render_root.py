from pathlib import Path
import pdfplumber
from PIL import Image, ImageDraw
out=Path(r'D:\study\408\.planning\co-all-detail\sources')
selection={1:[5,6,7,8,11,12,13,18,19],6:[274,275,276,277,278,283,284,285,290],7:[291,292,293,294,295,299,300,301,302,303,304,305,306,307,308,309,326]}
with pdfplumber.open(r'D:\study\408\02-计算机组成原理\2027计算机组成原理_高清带书签版.pdf') as pdf:
    for chapter,pages in selection.items():
        dest=out/f'ch{chapter}'
        dest.mkdir(exist_ok=True)
        for start in range(0,len(pages),4):
            sheet=Image.new('RGB',(1800,2600),'white')
            draw=ImageDraw.Draw(sheet)
            for i,p in enumerate(pages[start:start+4]):
                im=pdf.pages[p+11].to_image(resolution=130).original.convert('RGB')
                im.save(dest/f'book-{p+12:03d}.jpg',quality=93)
                im.thumbnail((900,1260))
                x=i%2*900;y=i//2*1300
                sheet.paste(im,(x,y))
                draw.text((x+8,y+1270),f'Printed {p} / PDF {p+12}',fill='black')
            sheet.save(dest/f'sheet-{start//4+1}.jpg',quality=93)
print('Root chapter source pages rendered')
