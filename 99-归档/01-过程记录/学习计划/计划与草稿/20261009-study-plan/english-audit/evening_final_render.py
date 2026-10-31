from pathlib import Path
import pypdfium2 as pdfium
root=Path(r'F:\study English\英语一\04-拓展资料')
out=Path(r'D:\study\学习计划\.planning\20261009-study-plan\english-audit')
for label,name,numbers in [('sentences','88句终结长难句.pdf',[33,34]),('explain','考研英语58篇基础阅读-精讲册（1）.pdf',[9,10,11,48,49])]:
 doc=pdfium.PdfDocument(root/name)
 for num in numbers:
  p=doc[num-1];b=p.render(scale=1.35);b.to_pil().convert('RGB').save(out/f'{label}-{num:03}.jpg',quality=88);b.close();p.close()
 doc.close()
print('done')
