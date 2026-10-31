from pathlib import Path
import re
import av
import numpy as np
from PIL import Image, ImageDraw, ImageFont

source = Path(r'D:\study\数学一\01.基础30讲-高数\17.第15讲')
root = Path(r'D:\study\数学一\_第十五讲视频解读')
frames = root / '教学画面'
sheets = root / '画面检查页'
frames.mkdir(exist_ok=True)
sheets.mkdir(exist_ok=True)
font = ImageFont.truetype(r'C:\Windows\Fonts\arial.ttf', 18)
files = sorted((p for p in source.glob('*.mp4') if re.match(r'^\d{2}\.',p.name) and int(p.name[:2]) <= 11), key=lambda p: int(p.name[:2]))
for video in files:
    num = int(video.name[:2])
    c = av.open(str(video))
    stream = c.streams.video[0]
    duration = float(c.duration / av.time_base)
    selected = []
    prev = None
    last_saved = -120
    for second in range(0, int(duration), 20):
        c.seek(int(second / stream.time_base), stream=stream, backward=True)
        frame = None
        for f in c.decode(stream):
            t = float(f.pts * stream.time_base) if f.pts is not None else 0
            if t >= second - 0.1:
                frame = f
                break
        if frame is None:
            continue
        img = frame.to_image().convert('RGB')
        small = np.asarray(img.resize((160,90))).astype(np.int16)
        change = float(np.abs(small-prev).mean()) if prev is not None else 999
        if second-last_saved >= 80 or (change >= 6 and second-last_saved >= 20):
            target = frames / f'{num:02d}_{second:05d}.jpg'
            img.save(target, quality=85)
            selected.append((target,second,change))
            last_saved = second
        prev = small
    c.close()
    for page,start in enumerate(range(0,len(selected),12),1):
        sheet = Image.new('RGB',(1600,960),'white')
        draw = ImageDraw.Draw(sheet)
        for i,(path,second,change) in enumerate(selected[start:start+12]):
            thumb=Image.open(path)
            thumb.thumbnail((392,270))
            x=(i%4)*400; y=(i//4)*320
            sheet.paste(thumb,(x+(392-thumb.width)//2,y))
            draw.text((x+8,y+274),f'{num:02d} {second//60:02d}:{second%60:02d} Δ{change:.0f}',fill='black',font=font)
        sheet.save(sheets/f'{num:02d}_page{page:02d}.jpg',quality=85)
    print(f'{num:02d}: {len(selected)} frames / {duration/60:.1f} min',flush=True)
