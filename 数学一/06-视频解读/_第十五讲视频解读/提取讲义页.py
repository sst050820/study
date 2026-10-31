from pathlib import Path
import pypdfium2 as pdfium

pdf = pdfium.PdfDocument(r'D:\study\数学一\01-基础讲义\27张宇基础30讲（高数）.pdf')
out = Path(r'D:\study\数学一\_第十五讲视频解读\讲义页')
out.mkdir(parents=True, exist_ok=True)
for num in range(383, 446):
    target = out / f'{num:03d}.png'
    if not target.exists():
        bitmap = pdf[num - 1].render(scale=1.5)
        bitmap.to_pil().save(target)
    print(num, flush=True)
