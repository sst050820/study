# -*- coding: utf-8 -*-
# 第12讲：批量提交4个视频 ASR（串行），task_id 存 task_ids_12.json
import subprocess, json, os, sys, time

DIR = r"D:\study\数学一\01.基础30讲-高数\14.第12讲"
OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "task_ids_12.json")

files = [
    "01.第12讲一元函数积分学的应用（三）基础知识结构01.mp4",
    "02.第12讲物理应用（仅数学一、数学二）02.mp4",
    "03.第12讲二、古鲁金第一定理及其应用&第14讲三、古鲁金第二定理03.mp4",
    "04.第12讲经济应用（仅数学三）04.mp4",
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
