from pathlib import Path
from fractions import Fraction as F
from itertools import permutations
import json

out = Path(__file__).parent
exec((out / '核对笔记计算.py').read_text(encoding='utf-8').split('checks=[]', 1)[0])
results = []

def verify(name, actual, expected):
    results.append({'项目': name, '相符': actual == expected,
                    '实际': str(actual), '期望': str(expected)})

def trim(p):
    p = list(p)
    while len(p) > 1 and p[-1] == 0:
        p.pop()
    return tuple(p)

def addp(a, b):
    return trim([(a[i] if i < len(a) else 0) +
                 (b[i] if i < len(b) else 0) for i in range(max(len(a), len(b)))])

def mulp(a, b):
    c = [F(0)] * (len(a) + len(b) - 1)
    for i, x in enumerate(a):
        for j, y in enumerate(b):
            c[i+j] += x*y
    return trim(c)

def detp(a):
    n = len(a)
    value = (F(0),)
    for p in permutations(range(n)):
        invs = sum(p[i] > p[j] for i in range(n) for j in range(i+1, n))
        term = (F((-1)**invs),)
        for i, j in enumerate(p):
            x = a[i][j]
            term = mulp(term, tuple(F(v) for v in x) if isinstance(x, list) else (F(x),))
        value = addp(value, term)
    return value

def cofactor(a, i, j):
    return (-1)**(i+j) * det([[x for c, x in enumerate(row) if c != j]
                              for r, row in enumerate(a) if r != i])

verify('教材例1.1', det(mat([[2,-1,0,0],[0,2,-1,0],[0,0,2,-1],[-1,0,0,2]])), F(15))
verify('教材例1.2', det(mat([[1,1,1,1],[1,2,0,0],[1,0,3,0],[1,0,0,4]])), F(-2))
verify('教材例1.3恒等多项式', detp([[1,-1,1,[-1,1]],[1,-1,[1,1],-1],[1,[-1,1],1,-1],[[1,1],-1,1,-1]]), (0,0,0,0,1))
for a, b, c in [(1,2,3),(1,1,-2),(0,2,4)]:
    verify(f'教材例1.6取值{a,b,c}', det(mat([[a,b,c],[a*a,b*b,c*c],[b+c,a+c,a+b]])), F((a+b+c)*(b-a)*(c-a)*(c-b)))
verify('教材例1.7恒等多项式', detp([[[1,-1],[0,1],0,0],[-1,[1,-1],[0,1],0],[0,-1,[1,-1],[0,1]],[0,0,-1,[1,-1]]]), (1,-1,1,-1,1))
verify('教材例1.8差式恒等多项式', detp([[1,0,1],[1,2,[1,2]],[1,3,[1,3,3]]]), (0,0,6))
for a in [F(2),F(2,3)]:
    verify(f'教材例1.9参数{a}', detp([[[-1,1],-2,3],[1,[-4,1],3],[-1,a,[-5,1]]]), (-36+6*a,34-3*a,-10,1))
verify('教材例1.11系数行列式', det(mat([[0,2,1],[-1,2,2],[2,-5,-2]])), F(5))
A=mat([[2,-1,2,3],[0,1,-1,0],[2,3,4,5],[1,1,1,1]])
verify('教材例1.12混合余子式', sum(cofactor(A,2,j) for j in range(3))-cofactor(A,2,3), F(6))
verify('教材例1.13及习题1.11参数恒等式', detp([[1,2,-2],[2,-1,[0,1]],[3,1,-1]]), (-5,5))
verify('习题1.1恒等多项式', detp([[[-2,1],[-1,1],[-2,1],[-3,1]],[[-2,2],[-1,2],[-2,2],[-3,2]],[[-3,3],[-2,3],[-5,4],[-5,3]],[[0,4],[-3,4],[-7,5],[-3,4]]]), (0,-5,5))
A=mat([[1,-1,0,1],[-2,1,-1,1],[3,-2,2,-1],[0,0,3,4]])
verify('习题1.2反向展开', cofactor(A,0,0)-cofactor(A,0,1), F(-4))
verify('习题1.3三次项系数', detp([[[0,1],[0,1],1,[0,2]],[1,[0,1],2,-1],[2,1,[0,1],1],[2,-1,1,[0,1]]])[3], F(-5))
verify('习题1.4恒等多项式', detp([[1,[0,1],0,0],[0,1,[0,1],0],[0,0,1,[0,1]],[[0,1],0,0,1]]), (1,0,0,0,-1))
verify('习题1.5', det(mat([[4,1,2,3],[3,4,1,2],[2,3,4,1],[1,2,3,4]])), F(160))
for a,b,c in [(0,0,0),(2,3,4),(-5,9,2)]:
    verify(f'习题1.6取值{a,b,c}',det(mat([[1,a,0,0],[-1,2-a,b,0],[0,-2,3-b,c],[0,0,-3,4-c]])),F(24))
verify('习题1.7恒等多项式', detp([[[0,1],2,2],[-2,[-2,1],2],[2,2,[-2,1]]]), (0,0,-4,1))
verify('习题1.9', det(mat([[0,1,2],[1,-1,1],[-2,2,0]])), F(-2))
verify('习题1.10恒等多项式', detp([[[0,1],1,1],[1,[0,1],1],[1,1,1]]), (1,-2,1))
for b in [0,7]:
    verify(f'视频例2.1参数a恒等式b={b}', detp([[2,1,0,-1],[-1,2,-5,3],[3,0,[0,1],b],[0,-2,4,0]]), (-30,10))
A=mat([[0,0,0,5,6],[0,0,0,7,8],[1,2,3,0,0],[0,1,4,0,0],[0,0,1,0,0]])
verify('视频例2.2全部代数余子式之和', sum(cofactor(A,i,j) for i in range(5) for j in range(5)), F(-4))
verify('秩一加边结论算例',det(mat([[2,2,3],[2,5,6],[3,6,10]])),F(15))
verify('施密特正交例内积', mul(mat([[1,0,1]]),mat([[-1],[2],[1]])),mat([[0]]))
verify('幂公式不能扩展至零次反例', rank(mat([[1,1,1,1],[1,1,1,1],[2,1,0,-1],[-1,0,1,2]])), 2)

(out/'审查计算记录.json').write_text(json.dumps(results,ensure_ascii=False,indent=2),encoding='utf-8')
failed = [x for x in results if not x['相符']]
print(json.dumps({'条数':len(results),'相符':len(results)-len(failed),'不符':failed},ensure_ascii=False,indent=2))
if failed:
    raise SystemExit(1)
