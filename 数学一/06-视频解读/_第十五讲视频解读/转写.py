from pathlib import Path
import os
import re
import time
from faster_whisper import WhisperModel

os.environ['HF_HOME'] = r'D:\study\408\.model-cache\huggingface'
source = Path(r'D:\study\数学一\01.基础30讲-高数\17.第15讲')
out = Path(r'D:\study\数学一\_第十五讲视频解读\转写')
out.mkdir(parents=True, exist_ok=True)
files = sorted((p for p in source.glob('*.mp4') if re.match(r'^\d{2}\.', p.name) and int(p.name[:2]) <= 11), key=lambda p: int(p.name[:2]))
model = WhisperModel('base', device='cpu', compute_type='int8', cpu_threads=8, num_workers=1)
for i, video in enumerate(files, 1):
    target = out / f'{video.name[:2]}.txt'
    if target.exists() and target.stat().st_size:
        print(f'{i}/{len(files)} SKIP {target.name}', flush=True)
        continue
    start = time.time()
    print(f'{i}/{len(files)} START {video.name}', flush=True)
    segments, info = model.transcribe(str(video), language='zh', beam_size=3, vad_filter=True)
    with target.open('w', encoding='utf-8') as f:
        for s in segments:
            if s.text.strip():
                f.write(f'[{s.start:07.2f}-{s.end:07.2f}] {s.text.strip()}\n')
    print(f'{i}/{len(files)} DONE {info.duration:.0f}s audio {time.time()-start:.1f}s wall', flush=True)
