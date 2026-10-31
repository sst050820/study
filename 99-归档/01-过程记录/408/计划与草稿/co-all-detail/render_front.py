from pathlib import Path
from PIL import Image, ImageDraw
import pdfplumber
root=Path(r'D:\study\408\.planning\co-all-detail\sources')
book=Path(r'D:\study\408\02-计算机组成原理\2027计算机组成原理_高清带书签版.pdf')
with pdfplumber.open(book) as pdf:
    for start in range(0,16,4):
        sheet=Image.new('RGB',(1600,2300),'white')
        draw=ImageDraw.Draw(sheet)
        for offset in range(4):
            idx=start+offset
            im=pdf.pages[idx].to_image(resolution=110).original.convert('RGB')
            im.save(root/f'book-{idx+1:03d}.jpg',quality=92)
            im.thumbnail((800,1120))
            x=offset%2*800;y=offset//2*1150
            sheet.paste(im,(x,y))
            draw.text((x+8,y+1125),f'PDF {idx+1}',fill='black')
        sheet.save(root/f'front-sheet-{start//4+1}.jpg',quality=92)
print('Rendered PDF pages 1-16')
