from pathlib import Path
import pypdfium2 as pdfium
from PIL import Image, ImageDraw
root=Path(r'F:\study English\英语一\04-拓展资料')
out=Path(r'D:\study\学习计划\.planning\20261009-study-plan\english-audit')
groups=[
('sentences','88句终结长难句.pdf',[1,2,3,4,5,6,26,27,28,29,30,31,32]),
('test','考研英语58篇基础阅读-真题册（2）.pdf',[1,2,3,4,5,11,12,13,14,15,16,17]),
('explain','考研英语58篇基础阅读-精讲册（1）.pdf',[1,2,3,4,5,6,7,8]),
]
for label,name,numbers in groups:
    doc=pdfium.PdfDocument(root/name)
    thumbs=[]
    for num in numbers:
        page=doc[num-1]
        bitmap=page.render(scale=1.35)
        im=bitmap.to_pil().convert('RGB')
        im.save(out/f'{label}-{num:03}.jpg',quality=88)
        thumb=im.copy();thumb.thumbnail((420,595))
        tile=Image.new('RGB',(430,625),'white')
        tile.paste(thumb,((430-thumb.width)//2,25))
        ImageDraw.Draw(tile).text((5,5),f'{label} PDF page {num}',fill='red')
        thumbs.append(tile)
        bitmap.close();page.close()
    for batch in range((len(thumbs)+5)//6):
        sheet=Image.new('RGB',(1290,1250),'#cccccc')
        for pos,tile in enumerate(thumbs[batch*6:(batch+1)*6]):sheet.paste(tile,((pos%3)*430,(pos//3)*625))
        sheet.save(out/f'{label}-montage-{batch+1}.jpg',quality=90)
    doc.close()
print('rendered')
