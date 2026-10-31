from pathlib import Path
import os, sys, json, time
BASE=Path(r'D:\study\数学一')
ROOT=BASE/'06-视频解读'
os.environ['HF_HOME']=r'D:\study\408\.model-cache\huggingface'
os.environ['HF_HUB_OFFLINE']='1'
os.environ['TEMP']=str(ROOT)
os.environ['TMP']=str(ROOT)

def sources(n):
    return sorted((BASE/'08-视频讲解'/'01.基础30讲-高数'/f'{n+2:02d}.第{n}讲').glob('*.mp4'))

def transcribe(n):
    from faster_whisper import WhisperModel
    out=ROOT/f'_第{n}讲视频解读'/'转写'
    out.mkdir(parents=True,exist_ok=True)
    modelpath=next(Path(r'D:\study\408\.model-cache\huggingface\hub\models--Systran--faster-whisper-base\snapshots').glob('*/model.bin')).parent
    model=WhisperModel(str(modelpath),device='cpu',compute_type='int8',cpu_threads=4,num_workers=1)
    for video in sources(n):
        target=out/(video.name[:2]+'.txt')
        done=target.with_suffix('.done.json')
        if done.exists(): continue
        start=time.time()
        print(f'START {n} {video.name}',flush=True)
        segs,info=model.transcribe(str(video),language='zh',beam_size=3,vad_filter=True)
        with target.open('w',encoding='utf8') as f:
            for s in segs:
                if s.text.strip():
                    f.write(f'[{s.start:.2f}-{s.end:.2f}] {s.text.strip()}\n')
                    f.flush()
        done.write_text(json.dumps({'video':str(video),'duration':info.duration,'wall':time.time()-start},ensure_ascii=False),encoding='utf8')
        print(f'DONE {n} {video.name[:2]} {info.duration:.1f}s audio {time.time()-start:.1f}s wall',flush=True)

def frames(n):
    import av,numpy as np
    from PIL import Image,ImageDraw,ImageFont
    root=ROOT/f'_第{n}讲视频解读'
    out=root/'教学画面'; sheets=root/'画面检查页'
    out.mkdir(parents=True,exist_ok=True); sheets.mkdir(parents=True,exist_ok=True)
    font=ImageFont.truetype(r'C:\Windows\Fonts\arial.ttf',22)
    manifest=[]
    for video in sources(n):
        num=video.name[:2]; c=av.open(str(video)); stream=c.streams.video[0]
        dur=float(c.duration/av.time_base); selected=[]; prev=None; last=-100
        for sec in range(0,int(dur),5):
            c.seek(int(sec/stream.time_base),stream=stream,backward=True)
            frame=None
            for f in c.decode(stream):
                if f.time is not None and f.time>=sec-0.1: frame=f; break
            if frame is None: continue
            im=frame.to_image().convert('RGB')
            a=np.asarray(im.resize((256,144))).astype(np.int16)
            delta=float(np.abs(a-prev).mean()) if prev is not None else 999
            if delta>=4 or sec-last>=60:
                path=out/f'{num}_{sec:05d}.jpg'; im.save(path,quality=88)
                selected.append((str(path),sec,delta)); prev=a; last=sec
        c.close()
        for p,start in enumerate(range(0,len(selected),12),1):
            sheet=Image.new('RGB',(2000,1200),'white'); draw=ImageDraw.Draw(sheet)
            for i,(path,sec,delta) in enumerate(selected[start:start+12]):
                im=Image.open(path); im.thumbnail((495,355))
                x=(i%4)*500; y=(i//4)*400; sheet.paste(im,(x,y))
                draw.text((x+6,y+362),f'{n}/{num} {sec//60:02d}:{sec%60:02d}',font=font,fill='black')
            sheet.save(sheets/f'{num}_{p:03d}.jpg',quality=88)
        manifest.append({'video':str(video),'duration':dur,'frames':selected})
        (root/'画面索引.json').write_text(json.dumps(manifest,ensure_ascii=False),encoding='utf8')
        print(f'FRAMES {n}/{num}: {len(selected)} duration {dur/60:.1f}min',flush=True)

if __name__=='__main__':
    globals()[sys.argv[1]](int(sys.argv[2]))
