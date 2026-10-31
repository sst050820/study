import sys,json,subprocess
from pathlib import Path
from pypdf import PdfReader
from PIL import Image,ImageOps,ImageDraw
sys.stdout.reconfigure(encoding='utf-8',errors='replace')
work=Path(r'D:\study\学习计划\.planning\20261009-material-audit')
book=Path(r'F:\study English\英语一\04-拓展资料\不就是语法和长难句吗.pdf')
r=PdfReader(str(book))
texts=[p.extract_text() or '' for p in r.pages[:12]]
record={'path':str(book),'pages':len(r.pages),'first12chars':sum(map(len,texts)),'metadata':{str(k):str(v) for k,v in (r.metadata or {}).items()},'samples':texts}
(work/'grammar-probe.json').write_text(json.dumps(record,ensure_ascii=False,indent=2),encoding='utf-8')
print(json.dumps({k:v for k,v in record.items() if k!='samples'},ensure_ascii=False));print('\n'.join(texts)[:3000])
exe=r'C:\Users\13939\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\poppler\Library\bin\pdftoppm.exe'
subprocess.run([exe,'-f','1','-l','8','-scale-to','1100','-png',str(book),str(work/'grammar')],check=True,stdout=subprocess.DEVNULL)
images=sorted(work.glob('grammar-*.png'))
for batch,block in enumerate([images[:4],images[4:8]],1):
    tiles=[]
    for p in block:
        im=Image.open(p).convert('RGB');im.thumbnail((700,1000))
        tile=Image.new('RGB',(720,1040),'white');tile.paste(im,((720-im.width)//2,35));ImageDraw.Draw(tile).text((10,8),p.stem,fill='black');tiles.append(tile)
    canvas=Image.new('RGB',(1440,2080),'#ddd')
    for i,t in enumerate(tiles):canvas.paste(t,((i%2)*720,(i//2)*1040))
    canvas.save(work/f'grammar-contact-{batch}.png')
    print(str(work/f'grammar-contact-{batch}.png'))
