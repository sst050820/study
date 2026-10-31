from pathlib import Path
import pypdfium2 as pdfium

base=Path(r'D:\study\408')
out=Path(r'D:\study\学习计划\.planning\20261009-study-plan\408-audit')
requests=[
('co',base/'02-计算机组成原理'/'2027计算机组成原理_高清带书签版.pdf',[0,7,8,9,10]),
('os',base/'03-操作系统'/'王道2027操作系统-高清带书签.pdf',[0,7,8,9]),
('net',base/'04-计算机网络'/'2027计算机网络_高清带书签版.pdf',[0,7,8,9]),
('answer2019',base/'90-复习资料'/'02-历年真题'/'答案'/'2019-answer.pdf',[0,11]),
('answer2021',base/'90-复习资料'/'02-历年真题'/'答案'/'2021-answer.pdf',[0,10]),
('answer2024',base/'90-复习资料'/'02-历年真题'/'答案'/'2024-answer.pdf',[0,5]),
]
for label,path,pages in requests:
    doc=pdfium.PdfDocument(str(path))
    for i in pages:
        page=doc[i]
        image=page.render(scale=1.4).to_pil()
        name=out/f'{label}-page{i+1}.png'
        image.save(name)
        print(str(name))
        page.close()
    doc.close()
