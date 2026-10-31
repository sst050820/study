from pathlib import Path
import hashlib
import json
import re
import sys

sys.stdout.reconfigure(encoding="utf-8")
vault = Path(r"D:\study\数学一")
work = vault / "06-视频解读" / "_线代9讲强化融合"
backup = Path((work / "备份位置.txt").read_text(encoding="utf-8"))
manifest = json.loads((backup / "manifest-before.json").read_text(encoding="utf-8"))
notes = sorted((vault / "07-讲义" / "线性代数").glob("0[1-6]-*.md"))
errors = []
results = []
changed_originals = []

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

for original in manifest:
    if digest(Path(original["backup"])) != original["sha256"]:
        errors.append("备份哈希不匹配：" + original["name"])

if len(notes) != 6 or len(manifest) != 6:
    errors.append("讲义或备份数量不是6")

expected = [(1, 8), (2, 4), (3, 20), (4, 9), (5, 10), (6, 7), (7, 4), (8, 10), (9, 15)]
assigned = {1: 1, 2: 1, 3: 2, 4: 2, 5: 4, 6: 3, 7: 5, 8: 5, 9: 6}
for path in notes:
    text = path.read_text(encoding="utf-8")
    lines = text.splitlines()
    number = int(path.name[:2])
    fences = sum(line.startswith("```") for line in lines)
    math_fences = text.count("$$")
    if fences % 2 or math_fences % 2:
        errors.append("围栏不成对：" + path.name)
    if "\ufffd" in text or "\\n\\n" in text:
        errors.append("乱码或字面换行：" + path.name)
    if "线代9讲" not in text or "强化" not in text:
        errors.append("缺强化来源：" + path.name)
    if "强化教材九讲到六讲映射" not in text:
        errors.append("缺总映射链接：" + path.name)
    missing_exercises = []
    for lecture, count in expected:
        if assigned[lecture] == number:
            for example in range(1, count + 1):
                if not re.search(rf"(?<![0-9]){lecture}[.]{example}(?![0-9])", text):
                    missing_exercises.append(f"{lecture}.{example}")
    if missing_exercises:
        errors.append("缺例题编号：" + path.name + " " + ",".join(missing_exercises))
    links = re.findall(r"\[\[([^\]]+)\]\]", text)
    bad_links = []
    for link in links:
        target = link.split("|", 1)[0]
        relative, _, heading = target.partition("#")
        target_path = path if not relative else vault / relative
        if relative and "/" not in relative and "\\" not in relative:
            same_folder = path.parent / relative
            if not same_folder.suffix:
                same_folder = same_folder.with_suffix(".md")
            if same_folder.exists():
                target_path = same_folder
        if not target_path.suffix:
            target_path = target_path.with_suffix(".md")
        if not target_path.exists():
            bad_links.append(target)
        elif heading and target_path.suffix == ".md" and not heading.startswith("^"):
            headings = [re.sub(r"^#+\s+", "", line) for line in target_path.read_text(encoding="utf-8").splitlines() if re.match(r"^#+\s+", line)]
            if heading not in headings:
                bad_links.append(target)
    if bad_links:
        errors.append("链接或标题未定位：" + path.name + " " + "; ".join(bad_links))
    old = Path(next(item["backup"] for item in manifest if item["name"] == path.name)).read_text(encoding="utf-8")
    missing_lines = [line for line in old.splitlines() if line.strip() and line not in text]
    changed_originals.append({"file": path.name, "changed_original_nonempty_lines": missing_lines})
    results.append({"file": path.name, "lines": len(lines), "math_fences": math_fences, "code_fences": fences, "sha256": digest(path), "changed_original_lines": len(missing_lines), "links": len(links)})

report = {"backup": str(backup), "backup_hash_matches": not any("备份哈希" in item for item in errors), "results": results, "errors": errors}
(work / "融合读回检查.json").write_text(json.dumps(report, ensure_ascii=False, indent=2), encoding="utf-8")
(work / "原文改写逐行记录.json").write_text(json.dumps(changed_originals, ensure_ascii=False, indent=2), encoding="utf-8")
(backup / "manifest-after.json").write_text(json.dumps(results, ensure_ascii=False, indent=2), encoding="utf-8")
print(json.dumps(report, ensure_ascii=False, indent=2))
sys.exit(bool(errors))
