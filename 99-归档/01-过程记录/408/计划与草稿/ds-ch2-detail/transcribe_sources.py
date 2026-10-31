from pathlib import Path
import json, time, av
from faster_whisper import WhisperModel
from PIL import Image, ImageDraw, ImageFont

src = Path(r'D:\study\408\01-数据结构\01-数据结构王道课程\02 第二章 线性表')
out = Path(r'D:\study\408\.planning\ds-ch2-detail\sources\video')
out.mkdir(parents=True, exist_ok=True)
model_path = r'D:\study\408\.model-cache\huggingface\hub\models--Systran--faster-whisper-base\snapshots\ebe41f70d5b6dfa9166e2c581c45c9c0cfc57b66'
model = WhisperModel(model_path, device='cpu', compute_type='int8', cpu_threads=8, num_workers=1)
manifest = []
for video in sorted(src.glob('*.mp4')):
    number = video.name.split()[0]
    target = out / (number + '.txt')
    container = av.open(str(video))
    stream = container.streams.video[0]
    duration = float(container.duration / av.time_base)
    entry = dict(file=video.name, duration_seconds=duration, subtitles=[s.type for s in container.streams if s.type=='subtitle'])
    thumbs = []
    for fraction in [0.18,0.43,0.70,0.92]:
        seconds=duration*fraction
        container.seek(int(seconds / float(stream.time_base)), stream=stream)
        for frame in container.decode(stream):
            if frame.time is not None and frame.time >= seconds:
                image=frame.to_image()
                image.save(out / f'{number}_{int(seconds):04d}.jpg', quality=90)
                image.thumbnail((768,440))
                thumbs.append((image.copy(),seconds))
                break
    container.close()
    sheet=Image.new('RGB',(1536,960),'white')
    draw=ImageDraw.Draw(sheet)
    font=ImageFont.truetype('C:/Windows/Fonts/msyh.ttc',20)
    for i,(im,t) in enumerate(thumbs):
        x=(i%2)*768;y=(i//2)*480
        sheet.paste(im,(x,y))
        draw.text((x+8,y+442),f'{video.stem}  {int(t)//60}:{int(t)%60:02d}',font=font,fill='black')
    sheet.save(out / f'{number}_sheet.jpg',quality=88)
    if not target.exists():
        start=time.time()
        print(f'START {number}: {video.name}, {duration/60:.1f} min',flush=True)
        segments,info=model.transcribe(str(video),language='zh',beam_size=3,vad_filter=True,initial_prompt='数据结构，线性表，顺序表，单链表，双链表，循环链表，静态链表，头结点，头指针，首元结点，位序，下标，前驱，后继，指针，malloc，free，LinkList，LNode。')
        with target.open('w',encoding='utf-8') as f:
            count=0
            for s in segments:
                if s.text.strip():
                    f.write(f'[{s.start:.2f}-{s.end:.2f}] {s.text.strip()}\n')
                    f.flush();count+=1
        print(f'DONE {number}: {count} segments, {time.time()-start:.0f}s',flush=True)
        entry['segments']=count
    manifest.append(entry)
    (out/'manifest.json').write_text(json.dumps(manifest,ensure_ascii=False,indent=2),encoding='utf-8')
print('ALL COMPLETE',flush=True)
