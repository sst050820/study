from pathlib import Path
from PIL import Image,ImageDraw,ImageFont
import re
root=Path(r'D:\study\数学一\06-视频解读')
font=ImageFont.truetype(r'C:\Windows\Fonts\arial.ttf',22)
for lecture in [1,2,3,4,6]:
    base=root/f'_线代视频第{lecture}讲'
    out=base/'时间线复核';out.mkdir(exist_ok=True)
    files=sorted((base/'教学画面').glob('*.jpg'))
    for num in sorted(set(p.name[:2] for p in files)):
        group=[p for p in files if p.name[:2]==num]
        selected=[];last=-100
        for p in group:
            sec=int(p.stem.split('_')[1])
            if sec-last>=40:
                selected.append((p,sec));last=sec
        if group and selected[-1][0]!=group[-1]:
            selected.append((group[-1],int(group[-1].stem.split('_')[1])))
        for page,start in enumerate(range(0,len(selected),12),1):
            canvas=Image.new('RGB',(2560,1530),'white');draw=ImageDraw.Draw(canvas)
            for i,(p,sec) in enumerate(selected[start:start+12]):
                im=Image.open(p);im.thumbnail((636,475))
                x=i%4*640;y=i//4*510;canvas.paste(im,(x,y))
                draw.text((x+4,y+477),f'{lecture}/{num} {sec//60:02d}:{sec%60:02d}',font=font,fill='black')
            canvas.save(out/f'{num}_{page:03d}.jpg',quality=90)
        print(lecture,num,len(selected),flush=True)


