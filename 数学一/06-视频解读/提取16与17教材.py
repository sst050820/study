from pathlib import Path
import pypdfium2 as pdfium
from PIL import Image,ImageDraw
root=Path(r'D:\study\数学一\06-视频解读\_第16与17讲教材')
root.mkdir(exist_ok=True)
doc=pdfium.PdfDocument(r'D:\study\数学一\01-基础讲义\27张宇基础30讲（高数）.pdf')
for n in range(415,min(len(doc),560)+1):
    out=root/f'{n:03}.png'
    if not out.exists(): doc[n-1].render(scale=1.7).to_pil().save(out)
for start in range(415,560,20):
    sheet=Image.new('RGB',(1500,2100),'white'); d=ImageDraw.Draw(sheet)
    for i,n in enumerate(range(start,min(start+20,561))):
        im=Image.open(root/f'{n:03}.png'); im.thumbnail((295,490))
        x=i%5*300; y=i//5*525;sheet.paste(im,(x,y));d.text((x,y+492),str(n),fill='black')
    sheet.save(root/f'index_{start}.jpg')
print(root,flush=True)
