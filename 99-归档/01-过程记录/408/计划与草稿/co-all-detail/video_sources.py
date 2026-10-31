from pathlib import Path
import json, time, av
from PIL import Image, ImageDraw, ImageFont
from faster_whisper import WhisperModel

root=Path(r'D:\study\408\02-计算机组成原理')
work=Path(r'D:\study\408\.planning\co-all-detail\sources')
out=work/'video'
out.mkdir(parents=True,exist_ok=True)
inventory=json.loads((work/'video-inventory.json').read_text(encoding='utf-8'))
font=ImageFont.truetype('C:/Windows/Fonts/msyh.ttc',17)
manifest=[]
for item in inventory:
    number=item['number']
    video=root/'01-计算机组成原理王道课程'/item['file']
    container=av.open(str(video))
    duration=float(container.duration/av.time_base)
    stream=container.streams.video[0]
    entry=dict(item,duration_seconds=duration)
    if not (out/f'{number}_sheet.jpg').exists():
        thumbs=[]
        for fraction in [.18,.43,.70,.92]:
            seconds=duration*fraction
            container.seek(int(seconds/float(stream.time_base)),stream=stream)
            for frame in container.decode(stream):
                if frame.time is not None and frame.time >= seconds:
                    im=frame.to_image()
                    im.save(out/f'{number}_{int(seconds):04d}.jpg',quality=90)
                    im.thumbnail((768,440))
                    thumbs.append((im.copy(),seconds))
                    break
        sheet=Image.new('RGB',(1536,960),'white')
        draw=ImageDraw.Draw(sheet)
        for i,(im,t) in enumerate(thumbs):
            x=i%2*768;y=i//2*480
            sheet.paste(im,(x,y))
            draw.text((x+6,y+442),f'{video.stem}  {int(t)//60}:{int(t)%60:02d}',font=font,fill='black')
        sheet.save(out/f'{number}_sheet.jpg',quality=90)
    container.close()
    manifest.append(entry)
    (out/'manifest.json').write_text(json.dumps(manifest,ensure_ascii=False,indent=2),encoding='utf-8')
    print(f'FRAMES {number}',flush=True)

selected={'38','40','43','47','50','54','63','66','68','71','74','80','82','89','91'}
model=WhisperModel(r'D:\study\408\.model-cache\huggingface\hub\models--Systran--faster-whisper-base\snapshots\ebe41f70d5b6dfa9166e2c581c45c9c0cfc57b66',device='cpu',compute_type='int8',cpu_threads=8,num_workers=1)
for item in inventory:
    number=item['number']
    if number not in selected: continue
    target=out/f'{number}.txt'
    done=out/f'{number}.done'
    if done.exists(): continue
    video=root/'01-计算机组成原理王道课程'/item['file']
    start=time.time()
    print(f'TRANSCRIBE {number}: {item["file"]}',flush=True)
    segments,_=model.transcribe(str(video),language='zh',beam_size=3,vad_filter=True,initial_prompt='计算机组成原理，主存，SRAM，DRAM，Cache，组相联，标记，指令寻址，基址，变址，相对寻址，栈帧，CPU，PC，MAR，MDR，微操作，数据通路，微程序，流水线，中断屏蔽，DMA，总线，同步，异步。')
    count=0
    with target.open('w',encoding='utf-8') as f:
        for s in segments:
            if s.text.strip():
                f.write(f'[{s.start:.2f}-{s.end:.2f}] {s.text.strip()}\n')
                f.flush();count+=1
    done.write_text(json.dumps({'segments':count,'elapsed_seconds':round(time.time()-start)},ensure_ascii=False),encoding='utf-8')
    print(f'DONE {number}: {count} segments, {time.time()-start:.0f}s',flush=True)
print('SOURCE EXTRACTION COMPLETE',flush=True)
