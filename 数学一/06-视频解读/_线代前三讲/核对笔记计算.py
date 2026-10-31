from fractions import Fraction as F
from pathlib import Path
import json, re

def mat(a): return [[F(x) for x in row] for row in a]
def eye(n): return [[F(i==j) for j in range(n)] for i in range(n)]
def mul(a,b): return [[sum(a[i][k]*b[k][j] for k in range(len(b))) for j in range(len(b[0]))] for i in range(len(a))]
def sub(a,b): return [[x-y for x,y in zip(ar,br)] for ar,br in zip(a,b)]
def trans(a): return [list(x) for x in zip(*a)]
def det(a):
    a=[r[:] for r in a]; value=F(1);n=len(a)
    for j in range(n):
        p=next((i for i in range(j,n) if a[i][j]),None)
        if p is None:return F(0)
        if p!=j:a[p],a[j]=a[j],a[p];value=-value
        pivot=a[j][j];value*=pivot
        for i in range(j+1,n):
            factor=a[i][j]/pivot
            for k in range(j+1,n):a[i][k]-=factor*a[j][k]
    return value
def inverse(a):
    n=len(a);a=[row[:]+idrow for row,idrow in zip(a,eye(n))]
    for j in range(n):
        p=next(i for i in range(j,n) if a[i][j]);a[j],a[p]=a[p],a[j]
        q=a[j][j];a[j]=[x/q for x in a[j]]
        for i in range(n):
            if i!=j:
                q=a[i][j];a[i]=[x-q*y for x,y in zip(a[i],a[j])]
    return [r[n:] for r in a]
def power(a,n):
    if n<0:a=inverse(a);n=-n
    r=eye(len(a))
    while n:
        if n%2:r=mul(r,a)
        a=mul(a,a);n//=2
    return r
def rank(a):
    a=[r[:] for r in a];i=0
    for j in range(len(a[0])):
        p=next((p for p in range(i,len(a)) if a[p][j]),None)
        if p is None:continue
        a[i],a[p]=a[p],a[i];q=a[i][j];a[i]=[x/q for x in a[i]]
        for p in range(i+1,len(a)):
            q=a[p][j];a[p]=[x-q*y for x,y in zip(a[p],a[i])]
        i+=1
        if i==len(a):break
    return i
checks=[]
def check(name,actual,expected):
    checks.append({'项目':name,'相符':actual==expected})
    if actual!=expected: print('不符',name,'实际=',actual,'笔记=',expected)

for x in [-2,F(-1,4),0,1,2,3]:
    g=mat([[2*x+1,1,2*x+1,3],[5*x+1,-2,4*x,-3],[0,1,2*x+1,2],[2*x,-2,4*x,-4]])
    check(f'视频1.1 g({x})',det(g),-x*(8*x+2))
A=mat([[2,6,-4],[-1,-3,2],[3,9,-6]])
check('视频3.2外积幂',power(A,10),[[(-7)**9*x for x in r] for r in A])
check('视频3.5幂零展开',power(mat([[1,1,-1],[0,1,1],[0,0,1]]),10),mat([[1,10,35],[0,1,10],[0,0,1]]))
check('视频3.6行列变换',mul(mul(power(mat([[1,0],[-1,1]]),3),mat([[1,2],[-1,3]])),power(mat([[0,1],[1,0]]),-5)),mat([[2,1],[-3,-4]]))
B=mat([[1,1,1],[0,1,1],[0,0,1]]);C=mat([[0,1,1],[0,0,1],[0,0,1]])
check('视频3.7还原A',mul(mul(B,sub(B,C)),inverse(B)),mat([[1,0,-1],[0,1,-1],[0,0,0]]))
D=mat([[1,-1,1],[0,1,-2],[0,0,1]])
check('跨章9.13 Gram因子',mul(trans(D),D),mat([[1,-1,1],[-1,2,-3],[1,-3,6]]))
P=mat([[1,1,3],[1,2,2],[0,0,2]])
check('视频3.16 A99',mul(mul(P,mat([[-1,0,0],[0,-2**99,0],[0,0,0]])),inverse(P)),mat([[2**99-2,1-2**99,2-2**98],[2**100-2,1-2**100,2-2**99],[0,0,0]]))
A=mat([[1,1,1,1],[1,1,1,1],[2,1,0,-1],[-1,0,1,2]]);B=mat([[1,1],[1,1],[0,1],[1,0]]);C=mat([[-1,0,1,2],[2,1,0,-1]])
check('视频3.17满秩分解',mul(B,C),A)
t=3**9;check('视频3.17 A10',power(A,10),mat([[t,t,t,t],[t,t,t,t],[t+1,t,t-1,t-2],[-1,0,1,2]]))
A=mat([[1,2,2],[1,3,0],[2,7,-2]]);B=mat([[1,2,2],[0,1,1],[-1,1,1]])
for k1,k2,k3 in [(0,0,1),(1,2,3),(-1,4,-2)]:
    P=mat([[3-6*k1,4-6*k2,4-6*k3],[-1+2*k1,-1+2*k2,-1+2*k3],[k1,k2,k3]])
    check('视频3.19参数式AP',mul(A,P),B);check('视频3.19行列式',det(P),F(k3-k2))
A=mat([[0,1,2],[0,0,3],[0,0,0]]);P=mat([[1,0,0],[0,1,F(-2,3)],[0,0,F(1,3)]]);J=mat([[0,1,0],[0,0,1],[0,0,0]])
check('视频3.20列链',mul(A,P),mul(P,J))
M=mat([[8,0,-4],[F(-3,2),9,-6],[-2,0,1]])
check('视频4.3低次关系',power(M,2),[[9*x for x in r] for r in M]);check('视频4.3秩',rank(M),2)
check('视频4.8反例秩',rank(mat([[0,1,0,0],[0,0,0,1],[0,0,1,0],[0,0,0,0]])),3)
K=mat([[1,2,2,1],[2,1,5,0],[3,1,9,1]])
check('视频6.1核向量',mul(K,mat([[3],[-1],[-1],[1]])),mat([[0],[0],[0]]))
check('视频6.2三列和',mul(mat([[-2,1,1],[1,1,-2],[-1,2,-1],[1,-2,1]]),mat([[1],[1],[1]])),mat([[0],[0],[0],[0]]))
A=mat([[1,-1,3,0,-1],[-1,0,-2,-1,-1],[1,1,1,2,3]])
G=mat([[1,-1],[-1,0],[1,1]]);H=mat([[1,0,2,1,1],[0,1,-1,1,2]])
check('视频6.3回原列分解',mul(G,H),A)
A=mat([[1,0,1],[1,1,0],[0,1,1]]);B=mat([[1,1,1],[0,1,1],[0,0,1]]);C=mat([[0,-1,1],[1,0,-1],[0,1,1]])
check('视频6.7基关系',mul(B,C),A);check('视频6.7新坐标',mul(inverse(C),mat([[1],[0],[2]])),mat([[F(3,2)],[F(1,2)],[F(3,2)]]))
root=Path(r'D:\study\数学一\07-讲义\线性代数')
for p in sorted(root.glob('0[123]-*.md')):
    text=p.read_text(encoding='utf-8'); chapter=int(p.name[:2])
    print(p.name,len(text),'字',len(text.splitlines()),'行','显示数学分隔符',text.count('$$'),'最高标题',max(len(m.group(1)) for m in re.finditer(r'^(#+) ',text,re.M)))
print('计算核对',len(checks),'条；相符',sum(c['相符'] for c in checks))
Path(__file__).with_name('计算核对记录.json').write_text(json.dumps(checks,ensure_ascii=False,indent=2),encoding='utf-8')
