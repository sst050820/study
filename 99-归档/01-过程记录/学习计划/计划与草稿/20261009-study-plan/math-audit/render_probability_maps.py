from pathlib import Path
import pypdfium2 as pdfium
from PIL import Image,ImageDraw
out=Path(__file__).parent
doc=pdfium.PdfDocument(r'D:\study\数学一\01-基础讲义\27张宇基础30讲概率.pdf')
for ch,first in [(1,10),(2,42),(3,71),(4,112),(5,134),(6,142)]:
    for page_num in [first,first+1,first+2]:
        doc[page_num-1].render(scale=1.6).to_pil().save(out/f'probability-map-ch{ch}-pdf{page_num}.png')
doc.close()
print('18 probability mindmap/intro pages rendered')
