from pathlib import Path
import json
import sys

base = Path(r'D:\study\数学一')
work = base / '06-视频解读' / '_线代第四至六讲'
work.mkdir(exist_ok=True)

def book():
    import pypdfium2 as pdfium
    from PIL import Image, ImageDraw
    doc = pdfium.PdfDocument(str(base / '01-基础讲义' / '27张宇基础30讲线代.pdf'))
    for number in range(121, len(doc) + 1):
        target = work / f'教材_{number:03d}.png'
        if not target.is_file():
            doc[number - 1].render(scale=2.2).to_pil().save(target)
    for start in range(121, len(doc) + 1, 12):
        canvas = Image.new('RGB', (1600, 1800), 'white')
        draw = ImageDraw.Draw(canvas)
        for i, number in enumerate(range(start, min(start + 12, len(doc) + 1))):
            im = Image.open(work / f'教材_{number:03d}.png')
            im.thumbnail((390, 550))
            x, y = i % 4 * 400, i // 4 * 600
            canvas.paste(im, (x, y))
            draw.text((x + 5, y + 552), f'PDF {number}', fill='black')
        canvas.save(work / f'目录索引_{start:03d}.jpg', quality=90)
    print(json.dumps({'PDF页数': len(doc), '已导出页序号': [121, len(doc)], '工作目录': str(work)}, ensure_ascii=False), flush=True)

def timeline(lectures):
    from PIL import Image, ImageDraw, ImageFont
    font = ImageFont.truetype(r'C:\Windows\Fonts\arial.ttf', 22)
    for lecture in lectures:
        folder = base / '06-视频解读' / f'_线代视频第{lecture}讲'
        destination = folder / '时间线复核'
        destination.mkdir(parents=True, exist_ok=True)
        index = folder / '画面索引.json'
        if not index.is_file():
            continue
        completed = {Path(rec['video']).name[:2] for rec in json.loads(index.read_text(encoding='utf-8'))}
        files = sorted(p for p in (folder / '教学画面').glob('*.jpg') if p.name[:2] in completed)
        for num in sorted(set(p.name[:2] for p in files)):
            group = [p for p in files if p.name[:2] == num]
            selected, last = [], -100
            for p in group:
                sec = int(p.stem.split('_')[1])
                if sec - last >= 40:
                    selected.append((p, sec))
                    last = sec
            if group and selected[-1][0] != group[-1]:
                selected.append((group[-1], int(group[-1].stem.split('_')[1])))
            for page, start in enumerate(range(0, len(selected), 12), 1):
                canvas = Image.new('RGB', (2560, 1530), 'white')
                draw = ImageDraw.Draw(canvas)
                for i, (p, sec) in enumerate(selected[start:start + 12]):
                    im = Image.open(p)
                    im.thumbnail((636, 475))
                    x, y = i % 4 * 640, i // 4 * 510
                    canvas.paste(im, (x, y))
                    draw.text((x + 4, y + 477), f'{lecture}/{num} {sec//60:02d}:{sec%60:02d}', font=font, fill='black')
                canvas.save(destination / f'{num}_{page:03d}.jpg', quality=90)
            print(f'TIMELINE {lecture}/{num}: {len(selected)}', flush=True)

if __name__ == '__main__':
    if sys.argv[1] == 'book':
        book()
    elif sys.argv[1] == 'timeline':
        timeline([int(n) for n in sys.argv[2:]])
