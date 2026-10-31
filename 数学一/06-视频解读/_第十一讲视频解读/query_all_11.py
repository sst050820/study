# -*- coding: utf-8 -*-
# 批量查询第11讲3个ASR任务，completed 则落地为 asr_XX.txt
import subprocess, json, os, sys, io
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding="utf-8")

WORK = os.path.dirname(os.path.abspath(__file__))
IDFILE = os.path.join(WORK, "task_ids_11.json")
TASKS = json.load(open(IDFILE, encoding="utf-8"))

def query_one(tid):
    cmd = ["mediakit-cli", "shared", "query-task", "--task-id", tid, "--max-poll-attempts", "0"]
    p = subprocess.run(cmd, capture_output=True, text=True, encoding="utf-8", errors="replace")
    out = (p.stdout or "").strip()
    i, j = out.find("{"), out.rfind("}")
    if i < 0 or j < 0:
        return None, out[:300]
    try:
        return json.loads(out[i:j+1]), None
    except Exception as e:
        return None, f"JSON解析失败 {e}: {out[:300]}"

def main():
    done = 0
    for name, tid in TASKS.items():
        idx = name[:2]
        fp = os.path.join(WORK, f"asr_{idx}.txt")
        if os.path.exists(fp) and os.path.getsize(fp) > 0:
            print(f"[已落地] {name}"); done += 1; continue
        j, err = query_one(tid)
        if j is None:
            print(f"[查询异常] {name}: {err}"); continue
        st = j.get("status")
        if st == "completed":
            subs = j.get("subtitles") or []
            text = "".join(s.get("subtitle_text", "") for s in subs)
            with open(fp, "w", encoding="utf-8") as f:
                f.write(text)
            print(f"[完成] {name} 时长{j.get('duration')}s {len(subs)}条 {len(text)}字 -> {fp}")
            done += 1
        else:
            print(f"[进行中] {name} status={st}")
    print(f"==== 已落地 {done}/{len(TASKS)} ====")

if __name__ == "__main__":
    main()
