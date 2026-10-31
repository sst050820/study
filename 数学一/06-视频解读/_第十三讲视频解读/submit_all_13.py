# -*- coding: utf-8 -*-
# 第13讲：批量提交8个视频 ASR（串行），task_id 存 task_ids_13.json
import subprocess, json, os, sys, time

DIR = r"D:\study\数学一\01.基础30讲-高数\15.第13讲"
OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "task_ids_13.json")

files = [
    "01.第13讲多云函数微分学基础知识结构01.mp4",
    "02.第13讲一、基础概念02.mp4",
    "03.第13讲一、基础概念03.mp4",
    "04.第13讲一、基础概念04.mp4",
    "05.第13讲二、多元函数微分法则05.mp4",
    "06.第13讲二、多元函数微分法则06 (1).mp4",
    "07.第13讲三、多元函数的极值与最值07.mp4",
    "08.第13讲三、多元函数的极值与最值08.mp4",
]

ids = {}
if os.path.exists(OUT):
    ids = json.load(open(OUT, encoding="utf-8"))

def parse_tid(out):
    import re
    m = re.search(r"amk-tool-asr-subtitles-[0-9]+", out)
    return m.group(0) if m else None

for f in files:
    if f in ids and ids[f]:
        print("已有", f, ids[f]); continue
    video = os.path.join(DIR, f)
    cmd = ["mediakit-cli", "video", "asr-subtitles", "--video-url", video,
           "--language", "cmn-Hans-CN"]
    print("提交", f, flush=True)
    try:
        p = subprocess.run(cmd, capture_output=True, text=True,
                           encoding="utf-8", errors="replace", timeout=1500)
        out = (p.stdout or "") + "\n" + (p.stderr or "")
        tid = parse_tid(out)
        if tid:
            ids[f] = tid
            json.dump(ids, open(OUT, "w", encoding="utf-8"), ensure_ascii=False, indent=1)
            print("  ->", tid, flush=True)
        else:
            print("  !! 未解析到task_id:", out[:400], flush=True)
    except subprocess.TimeoutExpired:
        print("  !! 超时(25min)", f, flush=True)
    except Exception as e:
        print("  !! 异常", repr(e), flush=True)
    time.sleep(3)

print("=== 完成 ===")
for f, t in ids.items():
    print(t, f)
