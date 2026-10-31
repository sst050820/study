from pathlib import Path
import os
import time
from faster_whisper import WhisperModel

os.environ['HF_HOME'] = r'D:\study\408\.model-cache\huggingface'
source = Path(r'D:\study\数学一\01.基础30讲-高数\17.第15讲')
out = Path(r'D:\study\数学一\_第十五讲视频解读\转写')
out.mkdir(parents=True, exist_ok=True)
model = WhisperModel('base', device='cpu', compute_type='int8', cpu_threads=6, num_workers=1)
for num in range(8,12):
    video = next(source.glob(f'{num:02d}.*.mp4'))
    target = out / f'{num:02d}.txt'
    if target.exists() and target.stat().st_size:
        print(f'{num:02d} SKIP', flush=True)
        continue
    start=time.time()
    print(f'{num:02d} START',flush=True)
    segments, info=model.transcribe(str(video),language='zh',beam_size=3,vad_filter=True)
    with target.open('w',encoding='utf-8') as f:
        for s in segments:
            if s.text.strip():
                f.write(f'[{s.start:07.2f}-{s.end:07.2f}] {s.text.strip()}\n')
    print(f'{num:02d} DONE {info.duration:.0f}s audio {time.time()-start:.1f}s wall',flush=True)
