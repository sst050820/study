---
title: 考研数学公式手册（Obsidian完善版）
aliases:
  - 考研数学公式汇总
  - 高等数学线性代数概率论公式手册
tags:
  - 考研数学
  - 高等数学
  - 线性代数
  - 概率论与数理统计
  - 公式手册
status: 复习资料
source: 依据《张宇数学公式手册》整理并补充
---

# 考研数学公式手册

> [!abstract] 使用说明
> 本笔记按照原公式手册的总体顺序，整理为四部分：
>
> 1. **高等数学**
> 2. **线性代数**
> 3. **概率论与数理统计**
> 4. **初等数学常用公式**
>
> 内容以考研常用定义、定理、公式、结论和易错点为主，并补充了原手册中较简略但考试经常使用的公式。本文为独立笔记，不与其他讲义建立双向链接。

> [!warning] 符号约定
> - $C$ 表示任意常数；
> - $o(\cdot)$ 表示高阶无穷小；
> - $A^*$ 表示矩阵 $A$ 的伴随矩阵；
> - $r(A)$ 表示矩阵 $A$ 的秩；
> - $E(X)$、$D(X)$ 分别表示数学期望和方差；
> - 未特别说明时，积分曲线、曲面均分段光滑。

---

# 第一部分 高等数学

# 第一章 函数、极限与连续

## 1. 函数的基本性质

### 1.1 奇偶性

在关于原点对称的定义域上：

$$
f(-x)=f(x)
\Longleftrightarrow
f(x)\text{ 为偶函数},
$$

$$
f(-x)=-f(x)
\Longleftrightarrow
f(x)\text{ 为奇函数}.
$$

常用结论：

- 奇函数 $\times$ 奇函数 = 偶函数；
- 偶函数 $\times$ 偶函数 = 偶函数；
- 奇函数 $\times$ 偶函数 = 奇函数；
- 可积奇函数在对称区间上的积分为 $0$；
- 可积偶函数满足

$$
\int_{-a}^{a}f(x)\,dx
=
2\int_0^af(x)\,dx.
$$

---

### 1.2 周期性

若存在 $T>0$，使得对定义域内任意 $x$：

$$
f(x+T)=f(x),
$$

则 $f$ 为周期函数。

若 $T_1,T_2$ 是两个函数的周期，且 $\dfrac{T_1}{T_2}\in\mathbb Q$，则它们的和与积通常仍有公共周期。

---

### 1.3 单调性与有界性

- $x_1<x_2\Rightarrow f(x_1)\leq f(x_2)$：单调不减；
- $x_1<x_2\Rightarrow f(x_1)<f(x_2)$：严格单调增加；
- 若存在 $M$，使 $f(x)\leq M$，则有上界；
- 若存在 $m$，使 $f(x)\geq m$，则有下界。

---

### 1.4 反函数

严格单调函数在其值域上存在反函数。

若 $y=f(x)$ 可导、严格单调，且 $f'(x)\neq0$，则

$$
\boxed{
\left(f^{-1}\right)'(y)
=
\frac{1}{f'(x)},
\qquad y=f(x)
}
$$

---

## 2. 常用基本函数关系

### 2.1 指数与对数

$$
a^{x+y}=a^xa^y,
\qquad
a^{x-y}=\frac{a^x}{a^y},
$$

$$
\log_a(xy)=\log_ax+\log_ay,
$$

$$
\log_a\frac{x}{y}=\log_ax-\log_ay,
$$

$$
\log_ax^r=r\log_ax,
$$

$$
\log_ab=\frac{\ln b}{\ln a}.
$$

---

### 2.2 反三角函数

主值范围：

$$
\arcsin x\in\left[-\frac{\pi}{2},\frac{\pi}{2}\right],
\qquad |x|\leq1,
$$

$$
\arccos x\in[0,\pi],
\qquad |x|\leq1,
$$

$$
\arctan x\in\left(-\frac{\pi}{2},\frac{\pi}{2}\right).
$$

常用关系：

$$
\arcsin x+\arccos x=\frac{\pi}{2},
$$

$$
\arctan x+\operatorname{arccot}x=\frac{\pi}{2}.
$$

---

## 3. 极限的基本结论

### 3.1 极限运算法则

若

$$
\lim f(x)=A,\qquad \lim g(x)=B,
$$

则

$$
\lim[f(x)\pm g(x)]=A\pm B,
$$

$$
\lim f(x)g(x)=AB,
$$

$$
\lim\frac{f(x)}{g(x)}=\frac AB
\qquad (B\neq0).
$$

---

### 3.2 夹逼准则

若在某去心邻域内：

$$
g(x)\leq f(x)\leq h(x),
$$

且

$$
\lim g(x)=\lim h(x)=A,
$$

则

$$
\boxed{\lim f(x)=A}
$$

---

### 3.3 单调有界准则

单调有界数列必收敛：

$$
\boxed{
\text{单调增加且有上界}
\Rightarrow
\text{收敛}
}
$$

$$
\boxed{
\text{单调减少且有下界}
\Rightarrow
\text{收敛}
}
$$

---

## 4. 两个重要极限

$$
\boxed{
\lim_{x\to0}\frac{\sin x}{x}=1
}
$$

$$
\boxed{
\lim_{x\to0}(1+x)^{1/x}=e
}
$$

等价形式：

$$
\lim_{n\to\infty}\left(1+\frac1n\right)^n=e,
$$

$$
\lim_{x\to\infty}\left(1+\frac ax\right)^x=e^a,
$$

若 $u(x)\to0$，则

$$
\boxed{
(1+u)^{1/u}\to e
}
$$

---

## 5. 常用等价无穷小

当 $x\to0$ 时：

$$
\boxed{
\sin x\sim x,\quad
\tan x\sim x,\quad
\arcsin x\sim x,\quad
\arctan x\sim x
}
$$

$$
\boxed{
1-\cos x\sim\frac{x^2}{2}
}
$$

$$
\boxed{
e^x-1\sim x
}
$$

$$
\boxed{
a^x-1\sim x\ln a
}
$$

$$
\boxed{
\ln(1+x)\sim x
}
$$

$$
\boxed{
(1+x)^\alpha-1\sim\alpha x
}
$$

常用高阶等价：

$$
x-\sin x\sim\frac{x^3}{6},
$$

$$
\tan x-x\sim\frac{x^3}{3},
$$

$$
\tan x-\sin x\sim\frac{x^3}{2},
$$

$$
\arcsin x-x\sim\frac{x^3}{6},
$$

$$
x-\arctan x\sim\frac{x^3}{3},
$$

$$
e^x-1-x\sim\frac{x^2}{2},
$$

$$
\ln(1+x)-x\sim-\frac{x^2}{2}.
$$

> [!warning] 等价无穷小替换
> 等价无穷小通常可在**乘除结构**中直接替换；在加减结构中，应先提取主项或使用泰勒展开，不能机械替换。

---

## 6. 无穷小阶的比较

若 $\alpha,\beta\to0$：

$$
\lim\frac{\alpha}{\beta}=0
\Rightarrow
\alpha=o(\beta),
$$

$$
\lim\frac{\alpha}{\beta}=C\neq0
\Rightarrow
\alpha,\beta\text{ 同阶},
$$

$$
\lim\frac{\alpha}{\beta}=1
\Rightarrow
\alpha\sim\beta.
$$

若

$$
\lim\frac{\alpha}{\beta^k}=C\neq0,
$$

则 $\alpha$ 是 $\beta$ 的 $k$ 阶无穷小。

---

## 7. 连续与间断点

函数在 $x_0$ 连续：

$$
\boxed{
\lim_{x\to x_0}f(x)=f(x_0)
}
$$

连续的充要条件：

$$
\Delta x\to0
\Rightarrow
\Delta y=f(x_0+\Delta x)-f(x_0)\to0.
$$

间断点分类：

- **可去间断点**：左右极限存在且相等，但函数值不存在或不等于极限；
- **跳跃间断点**：左右极限存在但不相等；
- **无穷间断点**：至少一个单侧极限为无穷；
- **振荡间断点**：极限因振荡不存在。

---

## 8. 闭区间连续函数的定理

若 $f$ 在 $[a,b]$ 连续，则：

1. **有界性定理**：$f$ 在 $[a,b]$ 有界；
2. **最值定理**：$f$ 在 $[a,b]$ 取得最大值和最小值；
3. **介值定理**：$f$ 取得最小值与最大值之间的一切值；
4. **零点定理**：若 $f(a)f(b)<0$，则存在 $\xi\in(a,b)$，使

$$
f(\xi)=0.
$$

---

# 第二章 一元函数微分学

## 1. 导数与微分

导数定义：

$$
\boxed{
f'(x_0)
=
\lim_{\Delta x\to0}
\frac{f(x_0+\Delta x)-f(x_0)}{\Delta x}
}
$$

可微定义：

$$
\Delta y=A\Delta x+o(\Delta x).
$$

若可微，则

$$
A=f'(x),
\qquad
dy=f'(x)\,dx.
$$

关系：

$$
\boxed{
\text{可导}
\Longleftrightarrow
\text{可微}
\Rightarrow
\text{连续}
}
$$

反向“连续 $\Rightarrow$ 可导”一般不成立。

---

## 2. 基本导数公式

$$
(C)'=0,
\qquad
(x^\alpha)'=\alpha x^{\alpha-1},
$$

$$
(e^x)'=e^x,
\qquad
(a^x)'=a^x\ln a,
$$

$$
(\ln x)'=\frac1x,
\qquad
(\log_ax)'=\frac{1}{x\ln a},
$$

$$
(\sin x)'=\cos x,
\qquad
(\cos x)'=-\sin x,
$$

$$
(\tan x)'=\sec^2x,
\qquad
(\cot x)'=-\csc^2x,
$$

$$
(\sec x)'=\sec x\tan x,
\qquad
(\csc x)'=-\csc x\cot x,
$$

$$
(\arcsin x)'=\frac1{\sqrt{1-x^2}},
$$

$$
(\arccos x)'=-\frac1{\sqrt{1-x^2}},
$$

$$
(\arctan x)'=\frac1{1+x^2},
$$

$$
(\operatorname{arccot}x)'=-\frac1{1+x^2}.
$$

---

## 3. 求导法则

$$
(u\pm v)'=u'\pm v',
$$

$$
(uv)'=u'v+uv',
$$

$$
\left(\frac uv\right)'
=
\frac{u'v-uv'}{v^2},
$$

复合函数：

$$
\boxed{
\frac{dy}{dx}
=
\frac{dy}{du}\frac{du}{dx}
}
$$

对数求导：

若

$$
y=u(x)^{v(x)},
$$

则

$$
\ln y=v\ln u,
$$

$$
\boxed{
\frac{y'}y
=
v'\ln u+v\frac{u'}u
}
$$

---

## 4. 隐函数与参数方程求导

若

$$
F(x,y)=0,
$$

且 $F_y\neq0$，则

$$
\boxed{
y'=-\frac{F_x}{F_y}
}
$$

二阶导数：

$$
\boxed{
y''
=
-
\frac{F_{xx}+2F_{xy}y'+F_{yy}(y')^2}{F_y}
}
$$

参数方程：

$$
x=\varphi(t),\qquad y=\psi(t),
$$

则

$$
\boxed{
\frac{dy}{dx}
=
\frac{\psi'(t)}{\varphi'(t)}
}
$$

$$
\boxed{
\frac{d^2y}{dx^2}
=
\frac{
\psi''(t)\varphi'(t)-\psi'(t)\varphi''(t)
}{
[\varphi'(t)]^3
}
}
$$

---

## 5. 高阶导数

莱布尼茨公式：

$$
\boxed{
(uv)^{(n)}
=
\sum_{k=0}^{n}
\binom nk
u^{(k)}v^{(n-k)}
}
$$

常用：

$$
(e^{ax})^{(n)}=a^ne^{ax},
$$

$$
[\sin(ax+b)]^{(n)}
=
a^n\sin\left(ax+b+\frac{n\pi}{2}\right),
$$

$$
[\cos(ax+b)]^{(n)}
=
a^n\cos\left(ax+b+\frac{n\pi}{2}\right),
$$

$$
\left(\frac1{ax+b}\right)^{(n)}
=
\frac{(-1)^na^nn!}{(ax+b)^{n+1}},
$$

$$
[\ln(ax+b)]^{(n)}
=
\frac{(-1)^{n-1}a^n(n-1)!}{(ax+b)^n}
\qquad(n\geq1).
$$

---

## 6. 微分中值定理

### 6.1 罗尔定理

若 $f$ 在 $[a,b]$ 连续，在 $(a,b)$ 可导，且

$$
f(a)=f(b),
$$

则存在 $\xi\in(a,b)$，使

$$
\boxed{f'(\xi)=0}
$$

---

### 6.2 拉格朗日中值定理

$$
\boxed{
f(b)-f(a)=f'(\xi)(b-a)
}
$$

等价形式：

$$
\frac{f(b)-f(a)}{b-a}=f'(\xi).
$$

---

### 6.3 柯西中值定理

若 $f,g$ 满足相应条件且 $g'(x)\neq0$，则存在 $\xi\in(a,b)$：

$$
\boxed{
\frac{f(b)-f(a)}{g(b)-g(a)}
=
\frac{f'(\xi)}{g'(\xi)}
}
$$

---

## 7. 泰勒公式

带拉格朗日余项：

$$
\boxed{
f(x)
=
\sum_{k=0}^{n}
\frac{f^{(k)}(x_0)}{k!}(x-x_0)^k
+
\frac{f^{(n+1)}(\xi)}{(n+1)!}(x-x_0)^{n+1}
}
$$

带佩亚诺余项：

$$
\boxed{
f(x)
=
\sum_{k=0}^{n}
\frac{f^{(k)}(x_0)}{k!}(x-x_0)^k
+
o((x-x_0)^n)
}
$$

常用麦克劳林展开：

$$
e^x
=
1+x+\frac{x^2}{2!}+\frac{x^3}{3!}+\cdots,
$$

$$
\sin x
=
x-\frac{x^3}{3!}+\frac{x^5}{5!}-\cdots,
$$

$$
\cos x
=
1-\frac{x^2}{2!}+\frac{x^4}{4!}-\cdots,
$$

$$
\ln(1+x)
=
x-\frac{x^2}{2}+\frac{x^3}{3}-\cdots,
$$

$$
(1+x)^\alpha
=
1+\alpha x+\frac{\alpha(\alpha-1)}{2!}x^2+\cdots.
$$

---

## 8. 洛必达法则

适用于：

$$
\frac00,\qquad \frac{\infty}{\infty}
$$

型未定式。

在满足可导、分母导数不为零等条件下：

$$
\boxed{
\lim\frac{f(x)}{g(x)}
=
\lim\frac{f'(x)}{g'(x)}
}
$$

其他未定式应先转化：

- $0\cdot\infty$：化为商；
- $\infty-\infty$：通分、根式有理化；
- $1^\infty,0^0,\infty^0$：取对数。

若

$$
y=f(x)^{g(x)},
$$

则

$$
\ln y=g(x)\ln f(x).
$$

---

## 9. 单调性、极值、最值

若在区间内：

$$
f'(x)>0
\Rightarrow
f\text{ 单调增加},
$$

$$
f'(x)<0
\Rightarrow
f\text{ 单调减少}.
$$

可导函数在内部极值点的必要条件：

$$
\boxed{
f'(x_0)=0
}
$$

第一充分条件：

- 左侧 $f'>0$，右侧 $f'<0$：极大值；
- 左侧 $f'<0$，右侧 $f'>0$：极小值。

第二充分条件：

若

$$
f'(x_0)=0,\qquad f''(x_0)\neq0,
$$

则

$$
f''(x_0)>0
\Rightarrow
x_0\text{ 为极小值点},
$$

$$
f''(x_0)<0
\Rightarrow
x_0\text{ 为极大值点}.
$$

---

## 10. 凹凸性与拐点

常用约定：

$$
f''(x)>0
\Rightarrow
\text{曲线凹（向上弯）},
$$

$$
f''(x)<0
\Rightarrow
\text{曲线凸（向下弯）}.
$$

拐点要求曲线两侧凹凸性改变。

$$
f''(x_0)=0
$$

只是可疑点，不是充分条件。

---

## 11. 渐近线

水平渐近线：

$$
\boxed{
\lim_{x\to\pm\infty}f(x)=A
\Rightarrow
y=A
}
$$

垂直渐近线：

$$
\boxed{
\lim_{x\to x_0^\pm}f(x)=\infty
\Rightarrow
x=x_0
}
$$

斜渐近线：

$$
\boxed{
y=ax+b
}
$$

其中

$$
a=\lim_{x\to\infty}\frac{f(x)}x,
$$

$$
b=\lim_{x\to\infty}[f(x)-ax].
$$

---

## 12. 弧长、曲率与曲率半径

显式曲线 $y=f(x)$：

$$
\boxed{
ds=\sqrt{1+[f'(x)]^2}\,dx
}
$$

参数曲线：

$$
\boxed{
ds=\sqrt{[x'(t)]^2+[y'(t)]^2}\,dt
}
$$

曲率：

$$
\boxed{
K
=
\frac{|y''|}{[1+(y')^2]^{3/2}}
}
$$

参数形式：

$$
\boxed{
K
=
\frac{|x'y''-y'x''|}
{[(x')^2+(y')^2]^{3/2}}
}
$$

曲率半径：

$$
\boxed{R=\frac1K}
$$

---

# 第三章 一元函数积分学

## 1. 不定积分基本公式

$$
\int x^\alpha\,dx
=
\frac{x^{\alpha+1}}{\alpha+1}+C
\qquad(\alpha\neq-1),
$$

$$
\int\frac{dx}{x}=\ln|x|+C,
$$

$$
\int e^x\,dx=e^x+C,
$$

$$
\int a^x\,dx=\frac{a^x}{\ln a}+C,
$$

$$
\int\sin x\,dx=-\cos x+C,
$$

$$
\int\cos x\,dx=\sin x+C,
$$

$$
\int\sec^2x\,dx=\tan x+C,
$$

$$
\int\csc^2x\,dx=-\cot x+C,
$$

$$
\int\sec x\tan x\,dx=\sec x+C,
$$

$$
\int\csc x\cot x\,dx=-\csc x+C,
$$

$$
\int\sec x\,dx
=
\ln|\sec x+\tan x|+C,
$$

$$
\int\csc x\,dx
=
\ln|\csc x-\cot x|+C,
$$

$$
\int\frac{dx}{a^2+x^2}
=
\frac1a\arctan\frac xa+C
\qquad(a>0),
$$

$$
\int\frac{dx}{\sqrt{a^2-x^2}}
=
\arcsin\frac xa+C,
$$

$$
\int\frac{dx}{a^2-x^2}
=
\frac1{2a}
\ln\left|\frac{a+x}{a-x}\right|+C,
$$

$$
\int\frac{dx}{\sqrt{x^2+a^2}}
=
\ln|x+\sqrt{x^2+a^2}|+C,
$$

$$
\int\frac{dx}{\sqrt{x^2-a^2}}
=
\ln|x+\sqrt{x^2-a^2}|+C.
$$

---

## 2. 换元法与分部积分

第一换元法：

$$
\boxed{
\int f(\varphi(x))\varphi'(x)\,dx
=
\int f(u)\,du
}
$$

分部积分：

$$
\boxed{
\int u\,dv=uv-\int v\,du
}
$$

常用优先顺序：

> 反三角函数、对数函数、幂函数、三角函数、指数函数。

---

## 3. 定积分基本性质

$$
\int_a^bf(x)\,dx
=
-\int_b^af(x)\,dx,
$$

$$
\int_a^cf(x)\,dx+\int_c^bf(x)\,dx
=
\int_a^bf(x)\,dx,
$$

若 $f\leq g$：

$$
\int_a^bf(x)\,dx
\leq
\int_a^bg(x)\,dx.
$$

估值：

若 $m\leq f(x)\leq M$，则

$$
\boxed{
m(b-a)
\leq
\int_a^bf(x)\,dx
\leq
M(b-a)
}
$$

积分中值定理：

若 $f$ 连续，则存在 $\xi\in[a,b]$：

$$
\boxed{
\int_a^bf(x)\,dx
=
f(\xi)(b-a)
}
$$

---

## 4. 微积分基本定理

变上限积分：

$$
F(x)=\int_a^xf(t)\,dt
$$

若 $f$ 连续，则

$$
\boxed{F'(x)=f(x)}
$$

复合上、下限：

$$
\boxed{
\frac{d}{dx}
\int_{\alpha(x)}^{\beta(x)}f(t)\,dt
=
f(\beta(x))\beta'(x)
-
f(\alpha(x))\alpha'(x)
}
$$

牛顿-莱布尼茨公式：

$$
\boxed{
\int_a^bf(x)\,dx
=
F(b)-F(a)
}
$$

---

## 5. 定积分换元与分部积分

换元：

若 $x=\varphi(t)$，且端点对应，则

$$
\boxed{
\int_a^bf(x)\,dx
=
\int_\alpha^\beta
f(\varphi(t))\varphi'(t)\,dt
}
$$

分部积分：

$$
\boxed{
\int_a^bu\,dv
=
[uv]_a^b-\int_a^bv\,du
}
$$

---

## 6. 定积分对称性与周期性

偶函数：

$$
\int_{-a}^{a}f(x)\,dx
=
2\int_0^af(x)\,dx.
$$

奇函数：

$$
\int_{-a}^{a}f(x)\,dx=0.
$$

周期函数，周期为 $T$：

$$
\boxed{
\int_a^{a+T}f(x)\,dx
=
\int_0^Tf(x)\,dx
}
$$

若 $f$ 在 $[0,a]$ 连续：

$$
\boxed{
\int_0^af(x)\,dx
=
\int_0^af(a-x)\,dx
}
$$

故

$$
\boxed{
\int_0^af(x)\,dx
=
\frac12\int_0^a[f(x)+f(a-x)]\,dx
}
$$

---

## 7. 华里士公式

$$
I_n=\int_0^{\pi/2}\sin^nx\,dx
=
\int_0^{\pi/2}\cos^nx\,dx.
$$

递推：

$$
\boxed{
I_n=\frac{n-1}{n}I_{n-2}
}
$$

其中

$$
I_0=\frac{\pi}{2},
\qquad
I_1=1.
$$

因此：

$$
I_{2n}
=
\frac{1\cdot3\cdots(2n-1)}
{2\cdot4\cdots2n}
\cdot\frac{\pi}{2},
$$

$$
I_{2n+1}
=
\frac{2\cdot4\cdots2n}
{3\cdot5\cdots(2n+1)}.
$$

---

## 8. 平面图形面积

直角坐标：

$$
\boxed{
S
=
\int_a^b|f(x)-g(x)|\,dx
}
$$

极坐标：

$$
\boxed{
S
=
\frac12\int_\alpha^\beta r^2(\theta)\,d\theta
}
$$

两条极坐标曲线之间：

$$
\boxed{
S
=
\frac12
\int_\alpha^\beta
[r_2^2(\theta)-r_1^2(\theta)]
\,d\theta
}
$$

参数曲线围成面积：

$$
\boxed{
S
=
\left|\int_\alpha^\beta y(t)x'(t)\,dt\right|
}
$$

闭曲线也可写为：

$$
S
=
\frac12\left|
\oint(x\,dy-y\,dx)
\right|.
$$

---

## 9. 旋转体体积

绕 $x$ 轴，圆盘法：

$$
\boxed{
V=\pi\int_a^bf^2(x)\,dx
}
$$

垫片法：

$$
\boxed{
V
=
\pi\int_a^b
[R^2(x)-r^2(x)]\,dx
}
$$

圆柱壳法，绕 $y$ 轴：

$$
\boxed{
V
=
2\pi\int_a^b
x\,|f(x)-g(x)|\,dx
}
$$

---

## 10. 旋转曲面面积

曲线 $y=f(x)$ 绕 $x$ 轴：

$$
\boxed{
S
=
2\pi\int_a^b
|f(x)|\sqrt{1+[f'(x)]^2}\,dx
}
$$

绕 $y$ 轴：

$$
\boxed{
S
=
2\pi\int_a^b
|x|\sqrt{1+[f'(x)]^2}\,dx
}
$$

---

## 11. 反常积分

无穷区间：

$$
\int_a^{+\infty}f(x)\,dx
=
\lim_{b\to+\infty}
\int_a^bf(x)\,dx.
$$

无界函数：

$$
\int_a^bf(x)\,dx
=
\lim_{\varepsilon\to0^+}
\int_{a+\varepsilon}^bf(x)\,dx
$$

或在奇点处分段。

基本判敛公式：

$$
\boxed{
\int_1^\infty\frac{dx}{x^p}
\begin{cases}
\text{收敛},&p>1,\\
\text{发散},&p\leq1,
\end{cases}
}
$$

$$
\boxed{
\int_0^1\frac{dx}{x^p}
\begin{cases}
\text{收敛},&p<1,\\
\text{发散},&p\geq1.
\end{cases}
}
$$

对数型：

$$
\boxed{
\int_e^\infty
\frac{dx}{x(\ln x)^p}
\begin{cases}
\text{收敛},&p>1,\\
\text{发散},&p\leq1.
\end{cases}
}
$$

比较判别：

若 $0\leq f\leq g$：

- $\int g$ 收敛 $\Rightarrow\int f$ 收敛；
- $\int f$ 发散 $\Rightarrow\int g$ 发散。

极限比较：

若

$$
\lim\frac{f(x)}{g(x)}=C\in(0,\infty),
$$

则同敛散。

---

# 第四章 向量代数与空间解析几何

## 1. 向量运算

设

$$
\boldsymbol a=(a_1,a_2,a_3),
\quad
\boldsymbol b=(b_1,b_2,b_3).
$$

模：

$$
|\boldsymbol a|
=
\sqrt{a_1^2+a_2^2+a_3^2}.
$$

点积：

$$
\boxed{
\boldsymbol a\cdot\boldsymbol b
=
a_1b_1+a_2b_2+a_3b_3
=
|\boldsymbol a||\boldsymbol b|\cos\theta
}
$$

垂直：

$$
\boldsymbol a\perp\boldsymbol b
\Longleftrightarrow
\boldsymbol a\cdot\boldsymbol b=0.
$$

叉积：

$$
\boxed{
\boldsymbol a\times\boldsymbol b
=
\begin{vmatrix}
\boldsymbol i&\boldsymbol j&\boldsymbol k\\
a_1&a_2&a_3\\
b_1&b_2&b_3
\end{vmatrix}
}
$$

平行：

$$
\boldsymbol a\parallel\boldsymbol b
\Longleftrightarrow
\boldsymbol a\times\boldsymbol b=\boldsymbol0.
$$

混合积：

$$
\boxed{
[\boldsymbol a,\boldsymbol b,\boldsymbol c]
=
(\boldsymbol a\times\boldsymbol b)\cdot\boldsymbol c
}
$$

三向量共面：

$$
[\boldsymbol a,\boldsymbol b,\boldsymbol c]=0.
$$

---

## 2. 平面方程

一般式：

$$
\boxed{
Ax+By+Cz+D=0
}
$$

法向量：

$$
\boldsymbol n=(A,B,C).
$$

点法式：

$$
\boxed{
A(x-x_0)+B(y-y_0)+C(z-z_0)=0
}
$$

截距式：

$$
\boxed{
\frac xa+\frac yb+\frac zc=1
}
$$

平面束：

若 $\Pi_1=0,\Pi_2=0$ 的交线固定，则过该交线的平面：

$$
\boxed{
\Pi_1+\lambda\Pi_2=0
}
$$

---

## 3. 空间直线

点向式：

$$
\boxed{
\frac{x-x_0}{l}
=
\frac{y-y_0}{m}
=
\frac{z-z_0}{n}
}
$$

参数式：

$$
\boxed{
x=x_0+lt,\quad
y=y_0+mt,\quad
z=z_0+nt
}
$$

两平面交线方向向量：

$$
\boxed{
\boldsymbol s
=
\boldsymbol n_1\times\boldsymbol n_2
}
$$

---

## 4. 距离公式

点到平面：

$$
\boxed{
d
=
\frac{|Ax_0+By_0+Cz_0+D|}
{\sqrt{A^2+B^2+C^2}}
}
$$

点到直线：

$$
\boxed{
d
=
\frac{
|\overrightarrow{P_1P_0}\times\boldsymbol s|
}{
|\boldsymbol s|
}
}
$$

平行平面：

$$
\boxed{
d
=
\frac{|D_1-D_2|}
{\sqrt{A^2+B^2+C^2}}
}
$$

异面直线：

$$
\boxed{
d
=
\frac{
|\overrightarrow{P_1P_2}\cdot
(\boldsymbol s_1\times\boldsymbol s_2)|
}{
|\boldsymbol s_1\times\boldsymbol s_2|
}
}
$$

---

## 5. 夹角与位置关系

线线夹角：

$$
\cos\theta
=
\frac{
|\boldsymbol s_1\cdot\boldsymbol s_2|
}{
|\boldsymbol s_1||\boldsymbol s_2|
}.
$$

面面夹角：

$$
\cos\theta
=
\frac{
|\boldsymbol n_1\cdot\boldsymbol n_2|
}{
|\boldsymbol n_1||\boldsymbol n_2|
}.
$$

线面夹角：

$$
\boxed{
\sin\theta
=
\frac{
|\boldsymbol s\cdot\boldsymbol n|
}{
|\boldsymbol s||\boldsymbol n|
}
}
$$

---

## 6. 常见曲面

球面：

$$
(x-a)^2+(y-b)^2+(z-c)^2=R^2.
$$

椭球面：

$$
\frac{x^2}{a^2}
+
\frac{y^2}{b^2}
+
\frac{z^2}{c^2}
=1.
$$

单叶双曲面：

$$
\frac{x^2}{a^2}
+
\frac{y^2}{b^2}
-
\frac{z^2}{c^2}
=1.
$$

双叶双曲面：

$$
-\frac{x^2}{a^2}
-
\frac{y^2}{b^2}
+
\frac{z^2}{c^2}
=1.
$$

椭圆抛物面：

$$
z=\frac{x^2}{a^2}+\frac{y^2}{b^2}.
$$

双曲抛物面：

$$
z=\frac{x^2}{a^2}-\frac{y^2}{b^2}.
$$

锥面：

$$
\frac{x^2}{a^2}
+
\frac{y^2}{b^2}
-
\frac{z^2}{c^2}
=0.
$$

---

# 第五章 多元函数微分学

## 1. 偏导数与全微分

$$
f_x(x_0,y_0)
=
\lim_{\Delta x\to0}
\frac{
f(x_0+\Delta x,y_0)-f(x_0,y_0)
}{
\Delta x
}.
$$

若

$$
\Delta z
=
A\Delta x+B\Delta y+o(\rho),
\quad
\rho=\sqrt{(\Delta x)^2+(\Delta y)^2},
$$

则函数可微，且

$$
A=f_x,\qquad B=f_y.
$$

全微分：

$$
\boxed{
dz=f_x\,dx+f_y\,dy
}
$$

重要关系：

$$
\boxed{
\text{偏导数连续}
\Rightarrow
\text{可微}
\Rightarrow
\text{连续且偏导数存在}
}
$$

---

## 2. 多元复合函数求导

若

$$
z=f(u,v),
\quad
u=u(x,y),\quad
v=v(x,y),
$$

则

$$
\boxed{
z_x=f_uu_x+f_vv_x
}
$$

$$
\boxed{
z_y=f_uu_y+f_vv_y
}
$$

全微分形式不变：

$$
\boxed{
dz=f_u\,du+f_v\,dv
}
$$

---

## 3. 隐函数求导

若

$$
F(x,y,z)=0
$$

确定 $z=z(x,y)$，且 $F_z\neq0$，则

$$
\boxed{
z_x=-\frac{F_x}{F_z},
\qquad
z_y=-\frac{F_y}{F_z}
}
$$

---

## 4. 方向导数与梯度

单位方向向量：

$$
\boldsymbol l=(l_1,l_2,l_3),
\quad |\boldsymbol l|=1.
$$

方向导数：

$$
\boxed{
D_{\boldsymbol l}f
=
\nabla f\cdot\boldsymbol l
}
$$

梯度：

$$
\boxed{
\nabla f=(f_x,f_y,f_z)
}
$$

最大方向导数：

$$
\boxed{
\max D_{\boldsymbol l}f
=
|\nabla f|
}
$$

最大增长方向为 $\nabla f$ 的方向。

---

## 5. 切线、法平面、切平面与法线

参数曲线：

$$
\boldsymbol r(t)
=
(x(t),y(t),z(t)).
$$

切向量：

$$
\boxed{
\boldsymbol T=\boldsymbol r'(t_0)
}
$$

法平面：

$$
\boxed{
\boldsymbol r'(t_0)\cdot
(\boldsymbol r-\boldsymbol r_0)=0
}
$$

隐式曲面：

$$
F(x,y,z)=0.
$$

法向量：

$$
\boxed{
\boldsymbol n=\nabla F(P_0)
}
$$

切平面：

$$
\boxed{
F_x(P_0)(x-x_0)
+
F_y(P_0)(y-y_0)
+
F_z(P_0)(z-z_0)=0
}
$$

---

## 6. 二元函数极值

必要条件：

若 $f$ 可微且在内部取得极值，则

$$
\boxed{
f_x=f_y=0
}
$$

设驻点处：

$$
A=f_{xx},\quad B=f_{xy},\quad C=f_{yy},
$$

$$
D=AC-B^2.
$$

判别：

$$
\boxed{
\begin{array}{c|c}
D>0,\ A>0 & 极小值\\
D>0,\ A<0 & 极大值\\
D<0 & 鞍点\\
D=0 & 判别失效
\end{array}
}
$$

---

## 7. 条件极值

约束：

$$
g(x,y)=0.
$$

构造：

$$
L=f+\lambda g.
$$

求解：

$$
\boxed{
\begin{cases}
f_x+\lambda g_x=0,\\
f_y+\lambda g_y=0,\\
g(x,y)=0.
\end{cases}
}
$$

几何意义：

$$
\boxed{
\nabla f\parallel\nabla g
}
$$

---

## 8. 二元函数二阶泰勒公式

$$
\boxed{
\begin{aligned}
f(x_0+h,y_0+k)
={}&
f_0+f_xh+f_yk\\
&+
\frac12
(f_{xx}h^2+2f_{xy}hk+f_{yy}k^2)
+o(h^2+k^2)
\end{aligned}
}
$$

各偏导数均在 $(x_0,y_0)$ 处取值。

---

# 第六章 多元函数积分学

## 1. 二重积分

直角坐标：

若

$$
D=\{(x,y)\mid a\leq x\leq b,\ 
\varphi_1(x)\leq y\leq\varphi_2(x)\},
$$

则

$$
\boxed{
\iint_Df(x,y)\,dA
=
\int_a^bdx
\int_{\varphi_1(x)}^{\varphi_2(x)}
f(x,y)\,dy
}
$$

极坐标：

$$
x=r\cos\theta,\quad y=r\sin\theta,
$$

$$
\boxed{
dA=r\,dr\,d\theta
}
$$

$$
\boxed{
\iint_Df\,dA
=
\int_\alpha^\beta d\theta
\int_{r_1(\theta)}^{r_2(\theta)}
f(r\cos\theta,r\sin\theta)r\,dr
}
$$

---

## 2. 三重积分

直角坐标：

$$
dV=dx\,dy\,dz.
$$

柱面坐标：

$$
x=r\cos\theta,\quad
y=r\sin\theta,
$$

$$
\boxed{
dV=r\,dr\,d\theta\,dz
}
$$

球面坐标：

$$
x=\rho\sin\varphi\cos\theta,
$$

$$
y=\rho\sin\varphi\sin\theta,
$$

$$
z=\rho\cos\varphi,
$$

$$
\boxed{
dV=\rho^2\sin\varphi
\,d\rho\,d\varphi\,d\theta
}
$$

---

## 3. 重积分的几何应用

面积：

$$
S=\iint_D1\,dA.
$$

体积：

$$
V=\iiint_\Omega1\,dV.
$$

曲面 $z=f(x,y)\geq0$ 下方体积：

$$
V=\iint_Df(x,y)\,dA.
$$

质量：

$$
M=\iint_D\rho\,dA
\quad\text{或}\quad
M=\iiint_\Omega\rho\,dV.
$$

质心：

$$
\bar x
=
\frac1M\iint_Dx\rho\,dA,
\qquad
\bar y
=
\frac1M\iint_Dy\rho\,dA.
$$

空间情形类似。

---

## 4. 第一型曲线积分

$$
\boxed{
\int_Lf(x,y,z)\,ds
}
$$

参数曲线：

$$
x=x(t),\ y=y(t),\ z=z(t),
$$

$$
\boxed{
ds
=
\sqrt{x'^2+y'^2+z'^2}\,dt
}
$$

第一型曲线积分与方向无关。

---

## 5. 第二型曲线积分

$$
\boxed{
\int_LP\,dx+Q\,dy+R\,dz
}
$$

参数化：

$$
\boxed{
\int_\alpha^\beta
(Px'+Qy'+Rz')\,dt
}
$$

第二型曲线积分与方向有关，曲线反向后积分变号。

---

## 6. 格林公式

正向闭曲线 $L=\partial D$：

$$
\boxed{
\oint_LP\,dx+Q\,dy
=
\iint_D
(Q_x-P_y)\,dx\,dy
}
$$

面积公式：

$$
\boxed{
S_D
=
\frac12
\oint_L(x\,dy-y\,dx)
}
$$

路径无关的等价条件（单连通区域）：

$$
\boxed{
P_y=Q_x
}
$$

存在势函数 $u$：

$$
du=P\,dx+Q\,dy,
$$

则

$$
\boxed{
\int_A^BP\,dx+Q\,dy
=
u(B)-u(A)
}
$$

---

## 7. 第一型曲面积分

$$
\boxed{
\iint_\Sigma f(x,y,z)\,dS
}
$$

若

$$
z=z(x,y),
$$

则

$$
\boxed{
dS
=
\sqrt{1+z_x^2+z_y^2}\,dx\,dy
}
$$

参数曲面 $\boldsymbol r(u,v)$：

$$
\boxed{
dS
=
|\boldsymbol r_u\times\boldsymbol r_v|
\,du\,dv
}
$$

第一型曲面积分与取向无关。

---

## 8. 第二型曲面积分

通量形式：

$$
\boxed{
\iint_\Sigma
\boldsymbol F\cdot\boldsymbol n\,dS
}
$$

坐标形式：

$$
\boxed{
\iint_\Sigma
P\,dy\,dz+Q\,dz\,dx+R\,dx\,dy
}
$$

曲面反向后积分变号。

---

## 9. 高斯公式

封闭曲面 $\Sigma=\partial\Omega$ 取外侧：

$$
\boxed{
\iint_\Sigma
\boldsymbol F\cdot\boldsymbol n\,dS
=
\iiint_\Omega
\nabla\cdot\boldsymbol F\,dV
}
$$

其中

$$
\boxed{
\nabla\cdot\boldsymbol F
=
P_x+Q_y+R_z
}
$$

---

## 10. 斯托克斯公式

若 $L=\partial\Sigma$，方向满足右手规则：

$$
\boxed{
\oint_L\boldsymbol F\cdot d\boldsymbol r
=
\iint_\Sigma
(\nabla\times\boldsymbol F)
\cdot\boldsymbol n\,dS
}
$$

旋度：

$$
\boxed{
\nabla\times\boldsymbol F
=
(R_y-Q_z,\ P_z-R_x,\ Q_x-P_y)
}
$$

---

# 第七章 无穷级数

## 1. 数项级数

部分和：

$$
S_n=\sum_{k=1}^{n}u_k.
$$

级数收敛：

$$
\boxed{
\sum_{n=1}^\infty u_n\text{ 收敛}
\Longleftrightarrow
\lim_{n\to\infty}S_n\text{ 存在}
}
$$

必要条件：

$$
\boxed{
\sum u_n\text{ 收敛}
\Rightarrow
u_n\to0
}
$$

反之不成立。

---

## 2. 重要参照级数

等比级数：

$$
\boxed{
\sum_{n=0}^{\infty}q^n
=
\frac1{1-q},
\quad |q|<1
}
$$

$p$ 级数：

$$
\boxed{
\sum_{n=1}^{\infty}\frac1{n^p}
\begin{cases}
\text{收敛},&p>1,\\
\text{发散},&p\leq1.
\end{cases}
}
$$

对数型：

$$
\boxed{
\sum_{n=2}^{\infty}
\frac1{n(\ln n)^p}
\begin{cases}
\text{收敛},&p>1,\\
\text{发散},&p\leq1.
\end{cases}
}
$$

---

## 3. 正项级数判别法

比较判别：

若

$$
0\leq u_n\leq v_n,
$$

则：

- $\sum v_n$ 收敛 $\Rightarrow\sum u_n$ 收敛；
- $\sum u_n$ 发散 $\Rightarrow\sum v_n$ 发散。

极限比较：

若

$$
\lim_{n\to\infty}\frac{u_n}{v_n}
=C\in(0,\infty),
$$

则同敛散。

比值判别：

$$
\rho
=
\lim\frac{u_{n+1}}{u_n}.
$$

$$
\boxed{
\rho<1\Rightarrow\text{收敛},
\quad
\rho>1\Rightarrow\text{发散},
\quad
\rho=1\Rightarrow\text{失效}
}
$$

根值判别：

$$
\rho
=
\lim\sqrt[n]{u_n}.
$$

结论同上。

---

## 4. 交错级数

莱布尼茨判别：

若

$$
u_n\downarrow0,
$$

则

$$
\boxed{
\sum_{n=1}^{\infty}
(-1)^{n-1}u_n
\text{ 收敛}
}
$$

余项估计：

$$
\boxed{
|R_n|\leq u_{n+1}
}
$$

---

## 5. 绝对收敛与条件收敛

$$
\boxed{
\sum|u_n|\text{ 收敛}
\Rightarrow
\sum u_n\text{ 收敛}
}
$$

若 $\sum u_n$ 收敛而 $\sum|u_n|$ 发散，则条件收敛。

---

## 6. 幂级数

$$
\sum_{n=0}^{\infty}a_n(x-x_0)^n.
$$

收敛半径：

若极限存在，

$$
\boxed{
R
=
\lim_{n\to\infty}
\left|
\frac{a_n}{a_{n+1}}
\right|
}
$$

或

$$
\boxed{
R
=
\frac1{
\limsup\sqrt[n]{|a_n|}
}
}
$$

求收敛区间步骤：

1. 求 $R$；
2. 得到 $|x-x_0|<R$；
3. 左、右端点分别检验。

---

## 7. 幂级数运算

在收敛区间内部：

$$
\left(
\sum_{n=0}^{\infty}a_nx^n
\right)'
=
\sum_{n=1}^{\infty}na_nx^{n-1},
$$

$$
\int
\sum_{n=0}^{\infty}a_nx^n\,dx
=
C+
\sum_{n=0}^{\infty}
\frac{a_n}{n+1}x^{n+1}.
$$

逐项求导、逐项积分后收敛半径不变。

---

## 8. 常用幂级数

$$
\boxed{
\frac1{1-x}
=
\sum_{n=0}^{\infty}x^n,
\quad |x|<1
}
$$

$$
\boxed{
e^x
=
\sum_{n=0}^{\infty}\frac{x^n}{n!}
}
$$

$$
\boxed{
\sin x
=
\sum_{n=0}^{\infty}
(-1)^n\frac{x^{2n+1}}{(2n+1)!}
}
$$

$$
\boxed{
\cos x
=
\sum_{n=0}^{\infty}
(-1)^n\frac{x^{2n}}{(2n)!}
}
$$

$$
\boxed{
\ln(1+x)
=
\sum_{n=1}^{\infty}
(-1)^{n-1}\frac{x^n}{n},
\quad -1<x\leq1
}
$$

$$
\boxed{
\arctan x
=
\sum_{n=0}^{\infty}
(-1)^n\frac{x^{2n+1}}{2n+1},
\quad |x|\leq1
}
$$

$$
\boxed{
(1+x)^\alpha
=
\sum_{n=0}^{\infty}
\binom{\alpha}{n}x^n,
\quad |x|<1
}
$$

---

## 9. 傅里叶级数

周期为 $2l$：

$$
\boxed{
f(x)
\sim
\frac{a_0}{2}
+
\sum_{n=1}^{\infty}
\left[
a_n\cos\frac{n\pi x}{l}
+
b_n\sin\frac{n\pi x}{l}
\right]
}
$$

$$
a_n
=
\frac1l\int_{-l}^{l}
f(x)\cos\frac{n\pi x}{l}\,dx,
$$

$$
b_n
=
\frac1l\int_{-l}^{l}
f(x)\sin\frac{n\pi x}{l}\,dx.
$$

偶函数只含余弦项：

$$
b_n=0.
$$

奇函数只含正弦项：

$$
a_0=a_n=0.
$$

狄利克雷收敛结论：

$$
\boxed{
S(x)
=
\frac{f(x-0)+f(x+0)}{2}
}
$$

连续点处 $S(x)=f(x)$。

---

# 第八章 常微分方程与差分方程

## 1. 可分离变量方程

$$
\frac{dy}{dx}=f(x)g(y).
$$

分离：

$$
\boxed{
\frac{dy}{g(y)}=f(x)\,dx
}
$$

---

## 2. 齐次微分方程

$$
\frac{dy}{dx}
=
F\left(\frac yx\right).
$$

令

$$
\boxed{
y=ux,\qquad y'=u+xu'
}
$$

化为可分离变量方程。

---

## 3. 一阶线性微分方程

$$
\boxed{
y'+p(x)y=q(x)
}
$$

积分因子：

$$
\mu(x)=e^{\int p(x)\,dx}.
$$

通解：

$$
\boxed{
y
=
e^{-\int p\,dx}
\left[
\int q(x)e^{\int p\,dx}\,dx+C
\right]
}
$$

---

## 4. 伯努利方程

$$
y'+p(x)y=q(x)y^n
\qquad(n\neq0,1).
$$

令

$$
\boxed{
z=y^{1-n}
}
$$

化为一阶线性方程。

---

## 5. 全微分方程

$$
M(x,y)\,dx+N(x,y)\,dy=0.
$$

若

$$
\boxed{
M_y=N_x
}
$$

则存在 $u(x,y)$：

$$
du=M\,dx+N\,dy,
$$

解为

$$
\boxed{u(x,y)=C}
$$

---

## 6. 可降阶高阶方程

若

$$
y''=f(x),
$$

直接积分两次。

若方程不显含 $y$，令

$$
p=y',
\qquad
y''=p'.
$$

若方程不显含 $x$，令

$$
p=y',
\qquad
\boxed{
y''=p\frac{dp}{dy}
}
$$

---

## 7. 二阶常系数齐次线性方程

$$
y''+py'+qy=0.
$$

特征方程：

$$
\boxed{
r^2+pr+q=0
}
$$

两不等实根：

$$
y=C_1e^{r_1x}+C_2e^{r_2x}.
$$

二重根：

$$
y=(C_1+C_2x)e^{rx}.
$$

共轭复根 $\alpha\pm\beta i$：

$$
\boxed{
y=e^{\alpha x}
(C_1\cos\beta x+C_2\sin\beta x)
}
$$

---

## 8. 二阶常系数非齐次方程

$$
y''+py'+qy=f(x).
$$

通解：

$$
\boxed{
y=y_h+y_p
}
$$

待定系数法：

若

$$
f(x)=e^{\alpha x}
[P_m(x)\cos\beta x+Q_n(x)\sin\beta x],
$$

设特解为

$$
\boxed{
y_p
=
x^s e^{\alpha x}
[R_k(x)\cos\beta x+S_k(x)\sin\beta x]
}
$$

其中：

- $k=\max(m,n)$；
- $s$ 是 $\alpha+i\beta$ 作为特征根的重数。

---

## 9. 欧拉方程

$$
x^2y''+pxy'+qy=f(x).
$$

齐次方程可令：

$$
\boxed{y=x^m}
$$

或令

$$
x=e^t
$$

化为常系数方程。

二重根 $m$ 对应：

$$
\boxed{
y=x^m(C_1+C_2\ln x)
}
$$

---

## 10. 一阶常系数线性差分方程

$$
x_{n+1}=ax_n+b.
$$

当 $a\neq1$：

$$
\boxed{
x_n
=
C a^n+\frac{b}{1-a}
}
$$

若给定 $x_0$：

$$
\boxed{
x_n
=
a^nx_0
+
b\frac{1-a^n}{1-a}
}
$$

当 $a=1$：

$$
x_n=x_0+nb.
$$

---

# 第二部分 线性代数

# 第一章 行列式

## 1. 基本性质

$$
\det(A^T)=\det(A).
$$

交换两行（列），行列式变号。

某行（列）乘 $k$，行列式乘 $k$。

某行（列）加上另一行（列）的 $k$ 倍，行列式不变。

若两行（列）成比例，行列式为 $0$。

三角行列式：

$$
\boxed{
\det A
=
a_{11}a_{22}\cdots a_{nn}
}
$$

乘法：

$$
\boxed{
|AB|=|A||B|
}
$$

逆矩阵：

$$
\boxed{
|A^{-1}|=\frac1{|A|}
}
$$

数乘：

$$
\boxed{
|kA|=k^n|A|
}
$$

---

## 2. 余子式与代数余子式

删去第 $i$ 行、第 $j$ 列所得余子式：

$$
M_{ij}.
$$

代数余子式：

$$
\boxed{
A_{ij}=(-1)^{i+j}M_{ij}
}
$$

拉普拉斯展开：

$$
\boxed{
|A|
=
\sum_{j=1}^{n}a_{ij}A_{ij}
}
$$

任一行元素与另一行对应代数余子式乘积之和为 $0$：

$$
\sum_{j=1}^{n}a_{kj}A_{ij}=0
\qquad(k\neq i).
$$

---

## 3. 常用特殊行列式

范德蒙德行列式：

$$
\boxed{
\begin{vmatrix}
1&1&\cdots&1\\
x_1&x_2&\cdots&x_n\\
\vdots&\vdots&&\vdots\\
x_1^{n-1}&x_2^{n-1}&\cdots&x_n^{n-1}
\end{vmatrix}
=
\prod_{1\leq j<i\leq n}(x_i-x_j)
}
$$

二阶：

$$
\begin{vmatrix}
a&b\\c&d
\end{vmatrix}
=ad-bc.
$$

三阶可用按行列展开，考试中不建议机械套“对角线法”处理高阶行列式。

---

# 第二章 矩阵

## 1. 矩阵运算

$$
(A+B)^T=A^T+B^T,
$$

$$
(AB)^T=B^TA^T,
$$

$$
(ABC)^T=C^TB^TA^T.
$$

矩阵乘法一般不满足交换律：

$$
AB\neq BA.
$$

---

## 2. 逆矩阵

$$
AA^{-1}=A^{-1}A=I.
$$

可逆充要条件：

$$
\boxed{
A\text{ 可逆}
\Longleftrightarrow
|A|\neq0
\Longleftrightarrow
r(A)=n
}
$$

伴随矩阵公式：

$$
\boxed{
A^{-1}=\frac1{|A|}A^*
}
$$

乘积的逆：

$$
\boxed{
(AB)^{-1}=B^{-1}A^{-1}
}
$$

转置的逆：

$$
(A^T)^{-1}=(A^{-1})^T.
$$

---

## 3. 伴随矩阵的重要结论

$$
\boxed{
AA^*=A^*A=|A|I
}
$$

$$
\boxed{
|A^*|=|A|^{n-1}
}
$$

若 $A$ 可逆：

$$
\boxed{
A^*=|A|A^{-1}
}
$$

伴随矩阵的秩：

$$
\boxed{
r(A^*)
=
\begin{cases}
n,&r(A)=n,\\
1,&r(A)=n-1,\\
0,&r(A)\leq n-2.
\end{cases}
}
$$

---

## 4. 初等变换与初等矩阵

对矩阵作一次初等行变换，相当于左乘一个初等矩阵：

$$
B=EA.
$$

作一次初等列变换，相当于右乘：

$$
B=AE.
$$

可逆矩阵可以写成有限个初等矩阵的乘积。

---

## 5. 矩阵的秩

$$
\boxed{
r(A)=
\text{非零子式的最高阶数}
}
$$

基本性质：

$$
r(A)=r(A^T),
$$

$$
r(AB)\leq\min\{r(A),r(B)\},
$$

$$
r(A+B)\leq r(A)+r(B),
$$

若 $A$ 为 $m\times n$，$B$ 为 $n\times s$：

$$
\boxed{
r(A)+r(B)-n
\leq
r(AB)
}
$$

可逆矩阵不改变秩：

$$
r(PAQ)=r(A)
$$

其中 $P,Q$ 可逆。

重要结论：

$$
\boxed{
r(A^TA)=r(A)
}
$$

---

## 6. 分块矩阵

分块对角矩阵：

$$
A=
\begin{pmatrix}
A_1&0\\
0&A_2
\end{pmatrix}.
$$

则

$$
|A|=|A_1||A_2|.
$$

若 $A_1,A_2$ 可逆：

$$
A^{-1}
=
\begin{pmatrix}
A_1^{-1}&0\\
0&A_2^{-1}
\end{pmatrix}.
$$

---

# 第三章 向量

## 1. 线性组合与线性表示

若

$$
\boldsymbol\beta
=
k_1\boldsymbol\alpha_1+\cdots+
k_s\boldsymbol\alpha_s,
$$

则称 $\boldsymbol\beta$ 可由 $\boldsymbol\alpha_1,\ldots,\boldsymbol\alpha_s$ 线性表示。

判定：

$$
\boxed{
\boldsymbol\beta\text{ 可由向量组 }A\text{ 表示}
\Longleftrightarrow
r(A)=r(A,\boldsymbol\beta)
}
$$

---

## 2. 线性相关与无关

向量组 $\boldsymbol\alpha_1,\ldots,\boldsymbol\alpha_s$ 线性相关：

存在不全为零的 $k_i$：

$$
k_1\boldsymbol\alpha_1+\cdots+k_s\boldsymbol\alpha_s=\boldsymbol0.
$$

无关则只有零解。

判定：

$$
\boxed{
\boldsymbol\alpha_1,\ldots,\boldsymbol\alpha_s
\text{ 线性无关}
\Longleftrightarrow
r(\boldsymbol\alpha_1,\ldots,\boldsymbol\alpha_s)=s
}
$$

常用结论：

- 含零向量的向量组必相关；
- 部分组相关 $\Rightarrow$ 整体组相关；
- 整体组无关 $\Rightarrow$ 任一部分组无关；
- $n$ 维空间中超过 $n$ 个向量必相关；
- 两个向量相关等价于成比例。

---

## 3. 极大无关组与向量组的秩

极大线性无关组所含向量个数等于向量组的秩。

任意两个极大无关组所含向量个数相同。

---

## 4. 等价向量组

两个向量组可以互相线性表示，则等价。

等价向量组的秩相同。

若两个向量组等价且均线性无关，则向量个数相同。

---

## 5. 基、维数与坐标

$n$ 维向量空间的一组基：

$$
\boldsymbol e_1,\ldots,\boldsymbol e_n
$$

线性无关且能表示空间中任意向量。

若

$$
\boldsymbol x
=
x_1\boldsymbol e_1+\cdots+x_n\boldsymbol e_n,
$$

则 $(x_1,\ldots,x_n)^T$ 是 $\boldsymbol x$ 在该基下的坐标。

---

## 6. 内积、正交与施密特正交化

标准内积：

$$
(\boldsymbol x,\boldsymbol y)
=
\boldsymbol x^T\boldsymbol y.
$$

正交：

$$
\boldsymbol x^T\boldsymbol y=0.
$$

单位向量：

$$
\boldsymbol e
=
\frac{\boldsymbol x}{\|\boldsymbol x\|}.
$$

施密特正交化：

$$
\boldsymbol\beta_1=\boldsymbol\alpha_1,
$$

$$
\boldsymbol\beta_k
=
\boldsymbol\alpha_k
-
\sum_{j=1}^{k-1}
\frac{
(\boldsymbol\alpha_k,\boldsymbol\beta_j)
}{
(\boldsymbol\beta_j,\boldsymbol\beta_j)
}
\boldsymbol\beta_j.
$$

再单位化：

$$
\boldsymbol e_k
=
\frac{\boldsymbol\beta_k}{\|\boldsymbol\beta_k\|}.
$$

---

# 第四章 线性方程组

## 1. 解的判定

方程组：

$$
A\boldsymbol x=\boldsymbol b.
$$

有解充要条件：

$$
\boxed{
r(A)=r(A,\boldsymbol b)
}
$$

唯一解：

$$
\boxed{
r(A)=r(A,\boldsymbol b)=n
}
$$

无穷多解：

$$
\boxed{
r(A)=r(A,\boldsymbol b)<n
}
$$

无解：

$$
\boxed{
r(A)<r(A,\boldsymbol b)
}
$$

---

## 2. 齐次线性方程组

$$
A\boldsymbol x=\boldsymbol0.
$$

必有零解。

有非零解充要条件：

$$
\boxed{
r(A)<n
}
$$

若 $A$ 为 $n$ 阶方阵：

$$
\boxed{
|A|=0
\Longleftrightarrow
\text{有非零解}
}
$$

基础解系含向量个数：

$$
\boxed{
n-r(A)
}
$$

通解：

$$
\boxed{
\boldsymbol x
=
k_1\boldsymbol\xi_1+\cdots+
k_{n-r}\boldsymbol\xi_{n-r}
}
$$

---

## 3. 非齐次线性方程组解的结构

若 $\boldsymbol\eta^*$ 是一个特解，$\boldsymbol\xi$ 是对应齐次方程通解，则：

$$
\boxed{
\boldsymbol x
=
\boldsymbol\eta^*
+
\boldsymbol\xi
}
$$

任意两个非齐次解之差，是对应齐次方程的解。

---

## 4. 克拉默法则

若 $A$ 为 $n$ 阶且 $|A|\neq0$：

$$
\boxed{
x_i=\frac{D_i}{D}
}
$$

其中 $D=|A|$，$D_i$ 是将第 $i$ 列换成常数列所得行列式。

---

# 第五章 特征值与特征向量

## 1. 定义

若存在非零向量 $\boldsymbol\xi$：

$$
A\boldsymbol\xi
=
\lambda\boldsymbol\xi,
$$

则 $\lambda$ 为特征值，$\boldsymbol\xi$ 为对应特征向量。

特征方程：

$$
\boxed{
|\lambda I-A|=0
}
$$

对应特征向量满足：

$$
\boxed{
(\lambda I-A)\boldsymbol x=\boldsymbol0
}
$$

---

## 2. 特征值的重要性质

若 $\lambda_i$ 为全部特征值：

$$
\boxed{
\sum_{i=1}^{n}\lambda_i
=
\operatorname{tr}(A)
}
$$

$$
\boxed{
\prod_{i=1}^{n}\lambda_i
=
|A|
}
$$

若 $A$ 可逆，则 $A^{-1}$ 的特征值为：

$$
\frac1{\lambda_i}.
$$

$A^k$ 的特征值为：

$$
\lambda_i^k.
$$

多项式矩阵 $f(A)$ 的特征值为：

$$
f(\lambda_i).
$$

不同特征值对应的特征向量线性无关。

---

## 3. 相似矩阵

若存在可逆矩阵 $P$：

$$
B=P^{-1}AP,
$$

则 $A,B$ 相似。

相似矩阵具有相同的：

- 特征多项式；
- 特征值；
- 行列式；
- 迹；
- 秩。

但这些量相同不一定相似。

---

## 4. 矩阵可对角化

$$
\boxed{
A\text{ 可对角化}
\Longleftrightarrow
A\text{ 有 }n\text{ 个线性无关特征向量}
}
$$

若 $A$ 有 $n$ 个互不相同的特征值，则必可对角化。

对每个特征值：

$$
\boxed{
\text{几何重数}
=
\text{代数重数}
}
$$

是可对角化的等价条件。

若：

$$
A=P\Lambda P^{-1},
$$

则：

$$
\boxed{
A^k=P\Lambda^kP^{-1}
}
$$

---

## 5. 实对称矩阵

实对称矩阵：

$$
A^T=A.
$$

性质：

1. 特征值均为实数；
2. 不同特征值对应特征向量正交；
3. 必可正交对角化：

$$
\boxed{
Q^TAQ=\Lambda,
\qquad Q^TQ=I
}
$$

---

# 第六章 二次型

## 1. 二次型矩阵表示

$$
f(x_1,\ldots,x_n)
=
\boldsymbol x^TA\boldsymbol x,
$$

其中 $A$ 取实对称矩阵。

交叉项 $2a_{ij}x_ix_j$ 对应：

$$
a_{ij}=a_{ji}.
$$

---

## 2. 合同变换

令：

$$
\boldsymbol x=C\boldsymbol y,
\qquad |C|\neq0.
$$

则：

$$
\boldsymbol x^TA\boldsymbol x
=
\boldsymbol y^T(C^TAC)\boldsymbol y.
$$

称 $A$ 与 $C^TAC$ 合同。

合同变换保持正、负惯性指数不变。

---

## 3. 标准形与规范形

标准形：

$$
\lambda_1y_1^2+\cdots+\lambda_ny_n^2.
$$

规范形：

$$
z_1^2+\cdots+z_p^2
-
z_{p+1}^2-\cdots-z_{p+q}^2.
$$

惯性定理：

$$
\boxed{
p,q\text{ 在可逆线性变换下不变}
}
$$

---

## 4. 正定二次型

实对称矩阵 $A$ 正定的等价条件：

$$
\boxed{
\boldsymbol x^TA\boldsymbol x>0
\quad(\boldsymbol x\neq0)
}
$$

$$
\boxed{
A\text{ 的全部特征值}>0
}
$$

$$
\boxed{
A\text{ 的各阶顺序主子式}>0
}
$$

即：

$$
\Delta_1>0,\quad
\Delta_2>0,\quad\cdots,\quad
\Delta_n>0.
$$

负定条件：

$$
\boxed{
(-1)^k\Delta_k>0
\qquad(k=1,\ldots,n)
}
$$

即顺序主子式符号交替：

$$
\Delta_1<0,\quad
\Delta_2>0,\quad
\Delta_3<0,\ldots
$$

---

# 第三部分 概率论与数理统计

# 第一章 随机事件与概率

## 1. 事件运算

$$
A\cup B：A\text{ 或 }B,
$$

$$
A\cap B=AB：A\text{ 与 }B,
$$

$$
\bar A：A\text{ 的对立事件},
$$

$$
A-B=A\bar B.
$$

德摩根律：

$$
\boxed{
\overline{A\cup B}
=
\bar A\cap\bar B
}
$$

$$
\boxed{
\overline{A\cap B}
=
\bar A\cup\bar B
}
$$

---

## 2. 概率公式

$$
P(\Omega)=1,
\qquad
P(\varnothing)=0.
$$

加法公式：

$$
\boxed{
P(A\cup B)
=
P(A)+P(B)-P(AB)
}
$$

若互斥：

$$
P(A\cup B)=P(A)+P(B).
$$

差事件：

$$
P(A-B)=P(A)-P(AB).
$$

---

## 3. 条件概率与乘法公式

$$
\boxed{
P(A|B)=\frac{P(AB)}{P(B)}
}
\qquad P(B)>0.
$$

乘法公式：

$$
\boxed{
P(AB)=P(B)P(A|B)
}
$$

多事件：

$$
P(A_1\cdots A_n)
=
P(A_1)
P(A_2|A_1)\cdots
P(A_n|A_1\cdots A_{n-1}).
$$

---

## 4. 独立性

$A,B$ 独立：

$$
\boxed{
P(AB)=P(A)P(B)
}
$$

若 $A,B$ 独立，则以下事件也相互独立：

$$
A,\bar B;\quad
\bar A,B;\quad
\bar A,\bar B.
$$

两两独立不一定相互独立。

---

## 5. 全概率公式与贝叶斯公式

若 $B_1,\ldots,B_n$ 构成完备事件组：

$$
B_iB_j=\varnothing,
\quad
\bigcup B_i=\Omega,
$$

则

$$
\boxed{
P(A)
=
\sum_{i=1}^{n}
P(B_i)P(A|B_i)
}
$$

贝叶斯公式：

$$
\boxed{
P(B_i|A)
=
\frac{
P(B_i)P(A|B_i)
}{
\sum_{j=1}^{n}
P(B_j)P(A|B_j)
}
}
$$

---

## 6. 古典概型与几何概型

古典概型：

$$
\boxed{
P(A)=\frac{\text{有利基本事件数}}{\text{基本事件总数}}
}
$$

几何概型：

$$
\boxed{
P(A)
=
\frac{\text{有利区域测度}}{\text{总体区域测度}}
}
$$

测度可以是长度、面积或体积。

---

# 第二章 一维随机变量及其分布

## 1. 分布函数

$$
\boxed{
F(x)=P(X\leq x)
}
$$

性质：

1. $F(x)$ 单调不减；
2. $0\leq F(x)\leq1$；
3. 右连续；
4. $F(-\infty)=0,\ F(+\infty)=1$。

区间概率：

$$
P(a<X\leq b)=F(b)-F(a).
$$

---

## 2. 离散型随机变量

概率分布：

$$
P(X=x_k)=p_k,
$$

$$
p_k\geq0,
\qquad
\sum p_k=1.
$$

分布函数：

$$
F(x)=\sum_{x_k\leq x}p_k.
$$

---

## 3. 连续型随机变量

概率密度：

$$
f(x)\geq0,
\qquad
\int_{-\infty}^{+\infty}f(x)\,dx=1.
$$

分布函数：

$$
\boxed{
F(x)=\int_{-\infty}^{x}f(t)\,dt
}
$$

若 $F$ 可导：

$$
f(x)=F'(x).
$$

连续型随机变量：

$$
P(X=a)=0.
$$

---

## 4. 常见离散分布

### 4.1 0-1 分布

$$
P(X=1)=p,
\qquad
P(X=0)=1-p=q.
$$

$$
E(X)=p,
\qquad
D(X)=pq.
$$

---

### 4.2 二项分布

$$
X\sim B(n,p).
$$

$$
\boxed{
P(X=k)
=
\binom nkp^kq^{n-k}
}
$$

$$
E(X)=np,
\qquad
D(X)=npq.
$$

---

### 4.3 泊松分布

$$
X\sim P(\lambda).
$$

$$
\boxed{
P(X=k)
=
e^{-\lambda}\frac{\lambda^k}{k!}
}
$$

$$
E(X)=D(X)=\lambda.
$$

当 $n$ 大、$p$ 小、$np=\lambda$ 时：

$$
B(n,p)\approx P(\lambda).
$$

---

### 4.4 几何分布

采用“首次成功所需试验次数”定义：

$$
P(X=k)=q^{k-1}p,
\qquad k=1,2,\ldots
$$

$$
E(X)=\frac1p,
$$

$$
D(X)=\frac q{p^2}.
$$

具有无记忆性。

---

### 4.5 超几何分布

总体 $N$ 个，其中 $M$ 个为某类，不放回抽取 $n$ 个：

$$
P(X=k)
=
\frac{
\binom Mk
\binom{N-M}{n-k}
}{
\binom Nn
}.
$$

$$
E(X)=n\frac MN,
$$

$$
D(X)
=
n\frac MN
\left(1-\frac MN\right)
\frac{N-n}{N-1}.
$$

---

## 5. 常见连续分布

### 5.1 均匀分布

$$
X\sim U(a,b).
$$

$$
f(x)
=
\begin{cases}
\dfrac1{b-a},&a<x<b,\\
0,&\text{其他}.
\end{cases}
$$

$$
E(X)=\frac{a+b}{2},
$$

$$
D(X)=\frac{(b-a)^2}{12}.
$$

---

### 5.2 指数分布

$$
X\sim E(\lambda).
$$

$$
f(x)
=
\begin{cases}
\lambda e^{-\lambda x},&x>0,\\
0,&x\leq0.
\end{cases}
$$

$$
F(x)=1-e^{-\lambda x}
\qquad(x>0).
$$

$$
E(X)=\frac1\lambda,
\qquad
D(X)=\frac1{\lambda^2}.
$$

无记忆性：

$$
P(X>s+t|X>s)=P(X>t).
$$

---

### 5.3 正态分布

$$
X\sim N(\mu,\sigma^2).
$$

$$
\boxed{
f(x)
=
\frac1{\sqrt{2\pi}\sigma}
e^{-\frac{(x-\mu)^2}{2\sigma^2}}
}
$$

标准化：

$$
\boxed{
Z=\frac{X-\mu}{\sigma}
\sim N(0,1)
}
$$

$$
P(a<X<b)
=
\Phi\left(\frac{b-\mu}{\sigma}\right)
-
\Phi\left(\frac{a-\mu}{\sigma}\right).
$$

标准正态：

$$
\Phi(-x)=1-\Phi(x).
$$

---

## 6. 随机变量函数的分布

离散型：

若 $Y=g(X)$，将所有满足 $g(x_i)=y$ 的概率相加。

连续单调变换：

若 $Y=g(X)$ 严格单调，反函数为 $x=h(y)$：

$$
\boxed{
f_Y(y)
=
f_X(h(y))|h'(y)|
}
$$

非单调时，对所有原像求和：

$$
f_Y(y)
=
\sum_i
f_X(x_i(y))
\left|
\frac{dx_i}{dy}
\right|.
$$

---

# 第三章 二维随机变量及其分布

## 1. 联合分布函数

$$
\boxed{
F(x,y)=P(X\leq x,Y\leq y)
}
$$

连续型联合密度：

$$
f(x,y)\geq0,
$$

$$
\iint_{\mathbb R^2}f(x,y)\,dx\,dy=1.
$$

$$
F(x,y)
=
\int_{-\infty}^{x}
\int_{-\infty}^{y}
f(u,v)\,dv\,du.
$$

---

## 2. 边缘分布

连续型：

$$
\boxed{
f_X(x)
=
\int_{-\infty}^{+\infty}
f(x,y)\,dy
}
$$

$$
\boxed{
f_Y(y)
=
\int_{-\infty}^{+\infty}
f(x,y)\,dx
}
$$

离散型：

$$
P(X=x_i)=\sum_jp_{ij}.
$$

---

## 3. 条件分布

连续型：

$$
\boxed{
f_{Y|X}(y|x)
=
\frac{f(x,y)}{f_X(x)}
}
$$

$$
\boxed{
f_{X|Y}(x|y)
=
\frac{f(x,y)}{f_Y(y)}
}
$$

---

## 4. 独立性

$$
\boxed{
X,Y\text{ 独立}
\Longleftrightarrow
F(x,y)=F_X(x)F_Y(y)
}
$$

连续型：

$$
\boxed{
f(x,y)=f_X(x)f_Y(y)
}
$$

独立随机变量函数仍独立：

若 $X,Y$ 独立，则 $g(X),h(Y)$ 独立。

---

## 5. 二维正态分布

若 $(X,Y)$ 为二维正态，相关系数为 $\rho$。

重要结论：

$$
\boxed{
X,Y\text{ 独立}
\Longleftrightarrow
\rho=0
}
$$

该结论一般只对联合正态成立；普通随机变量“不相关”不一定独立。

---

## 6. 随机变量函数的分布

### 6.1 和的分布：卷积

若 $X,Y$ 独立，$Z=X+Y$：

$$
\boxed{
f_Z(z)
=
\int_{-\infty}^{+\infty}
f_X(x)f_Y(z-x)\,dx
}
$$

也可写：

$$
f_Z(z)
=
\int_{-\infty}^{+\infty}
f_X(z-y)f_Y(y)\,dy.
$$

---

### 6.2 最大值与最小值

独立同分布 $X_1,\ldots,X_n$，分布函数为 $F$。

最大值：

$$
M=\max X_i,
$$

$$
\boxed{
F_M(x)=[F(x)]^n
}
$$

最小值：

$$
m=\min X_i,
$$

$$
\boxed{
F_m(x)
=
1-[1-F(x)]^n
}
$$

---

### 6.3 二维变量变换

若：

$$
u=u(x,y),\qquad v=v(x,y),
$$

可逆为：

$$
x=x(u,v),\qquad y=y(u,v),
$$

则：

$$
\boxed{
f_{U,V}(u,v)
=
f_{X,Y}(x(u,v),y(u,v))
\left|
\frac{\partial(x,y)}{\partial(u,v)}
\right|
}
$$

---

# 第四章 随机变量的数字特征

## 1. 数学期望

离散型：

$$
\boxed{
E(X)=\sum_kx_kp_k
}
$$

连续型：

$$
\boxed{
E(X)=\int_{-\infty}^{+\infty}xf(x)\,dx
}
$$

函数期望：

$$
E[g(X)]
=
\sum g(x_k)p_k
$$

或

$$
E[g(X)]
=
\int g(x)f(x)\,dx.
$$

二维：

$$
E[g(X,Y)]
=
\iint g(x,y)f(x,y)\,dx\,dy.
$$

线性性质：

$$
\boxed{
E(aX+bY+c)
=
aE(X)+bE(Y)+c
}
$$

若 $X,Y$ 独立：

$$
\boxed{
E(XY)=E(X)E(Y)
}
$$

---

## 2. 方差

$$
\boxed{
D(X)
=
E[(X-E(X))^2]
=
E(X^2)-[E(X)]^2
}
$$

$$
D(aX+b)=a^2D(X).
$$

$$
\boxed{
D(X\pm Y)
=
D(X)+D(Y)\pm2\operatorname{Cov}(X,Y)
}
$$

若独立：

$$
D(X\pm Y)=D(X)+D(Y).
$$

---

## 3. 协方差与相关系数

$$
\boxed{
\operatorname{Cov}(X,Y)
=
E(XY)-E(X)E(Y)
}
$$

$$
\boxed{
\rho_{XY}
=
\frac{
\operatorname{Cov}(X,Y)
}{
\sqrt{D(X)D(Y)}
}
}
$$

$$
|\rho_{XY}|\leq1.
$$

$$
|\rho|=1
\Longleftrightarrow
Y=aX+b\text{ 几乎处处成立}.
$$

独立 $\Rightarrow$ 不相关；反之一般不成立。

---

## 4. 矩与协方差矩阵

$k$ 阶原点矩：

$$
E(X^k).
$$

$k$ 阶中心矩：

$$
E[(X-E(X))^k].
$$

协方差矩阵：

$$
\Sigma
=
\begin{pmatrix}
D(X_1)&\operatorname{Cov}(X_1,X_2)&\cdots\\
\operatorname{Cov}(X_2,X_1)&D(X_2)&\cdots\\
\vdots&\vdots&\ddots
\end{pmatrix}.
$$

---

# 第五章 大数定律与中心极限定理

## 1. 切比雪夫不等式

若 $E(X)=\mu$，$D(X)=\sigma^2$，则对任意 $\varepsilon>0$：

$$
\boxed{
P(|X-\mu|\geq\varepsilon)
\leq
\frac{\sigma^2}{\varepsilon^2}
}
$$

等价：

$$
P(|X-\mu|<\varepsilon)
\geq
1-\frac{\sigma^2}{\varepsilon^2}.
$$

---

## 2. 大数定律

### 2.1 切比雪夫大数定律

若 $X_i$ 两两独立，方差一致有界，则：

$$
\frac1n\sum_{i=1}^{n}X_i
-
\frac1n\sum_{i=1}^{n}E(X_i)
\xrightarrow{P}0.
$$

### 2.2 辛钦大数定律

若 $X_i$ 独立同分布，且 $E|X_i|<\infty$：

$$
\boxed{
\bar X_n\xrightarrow{P}E(X)
}
$$

### 2.3 伯努利大数定律

频率依概率收敛于概率：

$$
\boxed{
\frac{\nu_n}{n}
\xrightarrow{P}p
}
$$

---

## 3. 中心极限定理

### 3.1 林德伯格-莱维定理

若 $X_i$ 独立同分布：

$$
E(X_i)=\mu,
\qquad
D(X_i)=\sigma^2>0,
$$

则

$$
\boxed{
\frac{
\sum_{i=1}^{n}X_i-n\mu
}{
\sigma\sqrt n
}
\xrightarrow{d}
N(0,1)
}
$$

即：

$$
\frac{\bar X-\mu}{\sigma/\sqrt n}
\approx N(0,1).
$$

---

### 3.2 棣莫弗-拉普拉斯定理

若

$$
X\sim B(n,p),
$$

则当 $n$ 大时：

$$
\boxed{
\frac{X-np}{\sqrt{npq}}
\approx N(0,1)
}
$$

离散变量正态近似时可使用连续性修正：

$$
P(a\leq X\leq b)
\approx
P(a-0.5<Y<b+0.5).
$$

---

# 第六章 数理统计的基本概念

## 1. 样本与统计量

总体随机变量 $X$，样本：

$$
X_1,\ldots,X_n
$$

独立同分布于总体。

统计量是不含未知参数的样本函数。

样本均值：

$$
\boxed{
\bar X
=
\frac1n\sum_{i=1}^{n}X_i
}
$$

样本方差：

$$
\boxed{
S^2
=
\frac1{n-1}
\sum_{i=1}^{n}(X_i-\bar X)^2
}
$$

常用恒等式：

$$
\boxed{
\sum_{i=1}^{n}(X_i-\bar X)^2
=
\sum_{i=1}^{n}X_i^2-n\bar X^2
}
$$

---

## 2. 三大抽样分布

### 2.1 卡方分布

若 $Z_i\sim N(0,1)$ 独立：

$$
\boxed{
\sum_{i=1}^{n}Z_i^2
\sim\chi^2(n)
}
$$

$$
E(\chi^2_n)=n,
\qquad
D(\chi^2_n)=2n.
$$

---

### 2.2 $t$ 分布

若

$$
Z\sim N(0,1),
\quad
U\sim\chi^2(n),
$$

且独立，则

$$
\boxed{
T=\frac{Z}{\sqrt{U/n}}
\sim t(n)
}
$$

---

### 2.3 $F$ 分布

若

$$
U\sim\chi^2(m),
\quad
V\sim\chi^2(n),
$$

且独立，则

$$
\boxed{
F
=
\frac{U/m}{V/n}
\sim F(m,n)
}
$$

倒数关系：

$$
\boxed{
F\sim F(m,n)
\Rightarrow
\frac1F\sim F(n,m)
}
$$

---

## 3. 正态总体的抽样分布

若

$$
X_1,\ldots,X_n
\sim N(\mu,\sigma^2),
$$

则：

$$
\boxed{
\bar X
\sim
N\left(\mu,\frac{\sigma^2}{n}\right)
}
$$

$$
\boxed{
\frac{(n-1)S^2}{\sigma^2}
\sim\chi^2(n-1)
}
$$

$$
\boxed{
\bar X\text{ 与 }S^2\text{ 独立}
}
$$

$$
\boxed{
\frac{\bar X-\mu}{S/\sqrt n}
\sim t(n-1)
}
$$

两个独立正态总体：

$$
\boxed{
\frac{S_1^2/\sigma_1^2}
{S_2^2/\sigma_2^2}
\sim F(n_1-1,n_2-1)
}
$$

---

# 第七章 参数估计

## 1. 矩估计法

用样本矩代替总体矩。

例如：

$$
E(X)=g(\theta)
$$

对应：

$$
\bar X=g(\hat\theta).
$$

解得 $\hat\theta$。

若有多个参数，使用多个阶矩建立方程组。

---

## 2. 最大似然估计

离散型似然函数：

$$
L(\theta)
=
\prod_{i=1}^{n}
P_\theta(X=x_i).
$$

连续型：

$$
L(\theta)
=
\prod_{i=1}^{n}
f(x_i;\theta).
$$

通常取对数：

$$
\ell(\theta)=\ln L(\theta).
$$

求：

$$
\frac{d\ell}{d\theta}=0
$$

并结合参数范围、端点和单调性确定最大值。

---

## 3. 估计量评价标准

无偏性：

$$
\boxed{
E(\hat\theta)=\theta
}
$$

有效性：

在无偏估计量中，方差越小越有效。

一致性：

$$
\boxed{
\hat\theta_n\xrightarrow{P}\theta
}
$$

均方误差：

$$
\boxed{
\operatorname{MSE}(\hat\theta)
=
D(\hat\theta)
+
[E(\hat\theta)-\theta]^2
}
$$

---

## 4. 正态总体置信区间

下面使用 $q_p(\nu)$ 表示自由度为 $\nu$ 的分布的 $p$ 分位点。

### 4.1 $\mu$ 的区间，$\sigma$ 已知

$$
\boxed{
\mu
\in
\left[
\bar X-z_{1-\alpha/2}\frac{\sigma}{\sqrt n},
\ 
\bar X+z_{1-\alpha/2}\frac{\sigma}{\sqrt n}
\right]
}
$$

---

### 4.2 $\mu$ 的区间，$\sigma$ 未知

$$
\boxed{
\mu
\in
\left[
\bar X-t_{1-\alpha/2}(n-1)\frac{S}{\sqrt n},
\ 
\bar X+t_{1-\alpha/2}(n-1)\frac{S}{\sqrt n}
\right]
}
$$

---

### 4.3 $\sigma^2$ 的区间，$\mu$ 未知

$$
\boxed{
\sigma^2
\in
\left[
\frac{(n-1)S^2}{q_{1-\alpha/2}(n-1)},
\ 
\frac{(n-1)S^2}{q_{\alpha/2}(n-1)}
\right]
}
$$

其中 $q_p$ 为 $\chi^2(n-1)$ 的 $p$ 分位点。

---

# 第四部分 初等数学常用公式

# 第一章 初等代数

## 1. 常用因式分解

$$
a^2-b^2=(a-b)(a+b),
$$

$$
a^3-b^3=(a-b)(a^2+ab+b^2),
$$

$$
a^3+b^3=(a+b)(a^2-ab+b^2),
$$

$$
a^n-b^n=(a-b)
(a^{n-1}+a^{n-2}b+\cdots+b^{n-1}).
$$

---

## 2. 二项式定理

$$
\boxed{
(a+b)^n
=
\sum_{k=0}^{n}
\binom nka^{n-k}b^k
}
$$

$$
\binom nk
=
\frac{n!}{k!(n-k)!}.
$$

---

## 3. 常用不等式

基本不等式：

若 $a,b>0$：

$$
\boxed{
\frac{a+b}{2}\geq\sqrt{ab}
}
$$

柯西不等式：

$$
\boxed{
\left(\sum_{i=1}^{n}a_ib_i\right)^2
\leq
\left(\sum_{i=1}^{n}a_i^2\right)
\left(\sum_{i=1}^{n}b_i^2\right)
}
$$

三角不等式：

$$
|x+y|\leq|x|+|y|.
$$

---

## 4. 数列求和

等差数列：

$$
a_n=a_1+(n-1)d,
$$

$$
\boxed{
S_n
=
\frac n2(a_1+a_n)
=
na_1+\frac{n(n-1)}2d
}
$$

等比数列：

$$
a_n=a_1q^{n-1},
$$

$$
\boxed{
S_n
=
a_1\frac{1-q^n}{1-q}
\qquad(q\neq1)
}
$$

---

# 第二章 平面几何与立体几何

圆面积与周长：

$$
S=\pi R^2,
\qquad
C=2\pi R.
$$

扇形面积：

$$
S=\frac12R^2\theta
$$

其中 $\theta$ 用弧度。

球体积与表面积：

$$
\boxed{
V=\frac43\pi R^3,
\qquad
S=4\pi R^2
}
$$

圆柱：

$$
V=\pi R^2h.
$$

圆锥：

$$
V=\frac13\pi R^2h.
$$

---

# 第三章 三角公式

## 1. 基本关系

$$
\sin^2x+\cos^2x=1,
$$

$$
1+\tan^2x=\sec^2x,
$$

$$
1+\cot^2x=\csc^2x.
$$

---

## 2. 和差公式

$$
\sin(\alpha\pm\beta)
=
\sin\alpha\cos\beta
\pm
\cos\alpha\sin\beta,
$$

$$
\cos(\alpha\pm\beta)
=
\cos\alpha\cos\beta
\mp
\sin\alpha\sin\beta,
$$

$$
\tan(\alpha\pm\beta)
=
\frac{\tan\alpha\pm\tan\beta}
{1\mp\tan\alpha\tan\beta}.
$$

---

## 3. 倍角与半角

$$
\sin2x=2\sin x\cos x,
$$

$$
\cos2x
=
\cos^2x-\sin^2x
=
1-2\sin^2x
=
2\cos^2x-1,
$$

$$
\tan2x
=
\frac{2\tan x}{1-\tan^2x}.
$$

$$
\sin^2\frac x2
=
\frac{1-\cos x}{2},
$$

$$
\cos^2\frac x2
=
\frac{1+\cos x}{2}.
$$

---

## 4. 积化和差

$$
\sin\alpha\sin\beta
=
\frac12[\cos(\alpha-\beta)-\cos(\alpha+\beta)],
$$

$$
\cos\alpha\cos\beta
=
\frac12[\cos(\alpha-\beta)+\cos(\alpha+\beta)],
$$

$$
\sin\alpha\cos\beta
=
\frac12[\sin(\alpha+\beta)+\sin(\alpha-\beta)].
$$

---

## 5. 和差化积

$$
\sin\alpha+\sin\beta
=
2\sin\frac{\alpha+\beta}{2}
\cos\frac{\alpha-\beta}{2},
$$

$$
\sin\alpha-\sin\beta
=
2\cos\frac{\alpha+\beta}{2}
\sin\frac{\alpha-\beta}{2},
$$

$$
\cos\alpha+\cos\beta
=
2\cos\frac{\alpha+\beta}{2}
\cos\frac{\alpha-\beta}{2},
$$

$$
\cos\alpha-\cos\beta
=
-2\sin\frac{\alpha+\beta}{2}
\sin\frac{\alpha-\beta}{2}.
$$

---

# 第四章 平面解析几何

直线：

$$
y=kx+b.
$$

点斜式：

$$
y-y_0=k(x-x_0).
$$

两点距离：

$$
d
=
\sqrt{(x_2-x_1)^2+(y_2-y_1)^2}.
$$

点到直线：

$$
\boxed{
d
=
\frac{|Ax_0+By_0+C|}
{\sqrt{A^2+B^2}}
}
$$

圆：

$$
(x-a)^2+(y-b)^2=R^2.
$$

椭圆：

$$
\frac{x^2}{a^2}
+
\frac{y^2}{b^2}
=1
\qquad(a>b>0).
$$

$$
c^2=a^2-b^2,
\qquad
e=\frac ca.
$$

双曲线：

$$
\frac{x^2}{a^2}
-
\frac{y^2}{b^2}
=1.
$$

$$
c^2=a^2+b^2,
\qquad
e=\frac ca.
$$

渐近线：

$$
y=\pm\frac ba x.
$$

抛物线：

$$
y^2=2px,
$$

焦点：

$$
\left(\frac p2,0\right),
$$

准线：

$$
x=-\frac p2.
$$

---

# 第五部分 高频易错点与速查

## 1. 高等数学易错点

> [!failure]
> 1. $u_n\to0$ 不能推出 $\sum u_n$ 收敛；
> 2. 连续不能推出可导；
> 3. 偏导数存在不能推出多元函数可微；
> 4. 极值点不一定是驻点：不可导点也可能取得极值；
> 5. 驻点不一定是极值点；
> 6. 使用极坐标、柱面坐标、球面坐标时不能遗漏雅可比；
> 7. 第一型曲线、曲面积分与方向无关，第二型与方向有关；
> 8. 格林公式要求闭曲线，高斯公式要求封闭曲面；
> 9. 幂级数端点必须分别判断；
> 10. 洛必达法则只直接处理 $\dfrac00$ 和 $\dfrac\infty\infty$ 型。

---

## 2. 线性代数易错点

> [!failure]
> 1. 矩阵乘法一般不可交换；
> 2. $(AB)^{-1}=B^{-1}A^{-1}$，顺序必须倒置；
> 3. 相似矩阵特征值相同，但特征值相同不一定相似；
> 4. 不同特征值对应特征向量线性无关；
> 5. 重特征值不保证有足够多线性无关特征向量；
> 6. 非齐次方程组有解条件是 $r(A)=r(A,b)$；
> 7. 基础解系含 $n-r(A)$ 个向量；
> 8. 正定判别要求矩阵为实对称矩阵；
> 9. 合同与相似是不同关系；
> 10. 行变换与列变换对线性方程组的意义不同。

---

## 3. 概率论易错点

> [!failure]
> 1. 互斥一般不等于独立；
> 2. 独立事件未必互斥；
> 3. 两两独立不一定相互独立；
> 4. 连续型随机变量在单点概率为 $0$，不代表该点“不可能”；
> 5. $E(XY)=E(X)E(Y)$ 需要独立等条件；
> 6. 不相关一般不能推出独立，联合正态是重要例外；
> 7. $D(X+Y)$ 中不能漏掉协方差；
> 8. 样本方差分母是 $n-1$；
> 9. 最大似然估计必须检查参数范围和边界；
> 10. 使用正态近似二项分布时注意标准化和连续性修正。

---

## 4. 考前核心公式清单

- [ ] 两个重要极限；
- [ ] 常用等价无穷小与泰勒展开；
- [ ] 基本导数和积分公式；
- [ ] 中值定理与洛必达法则；
- [ ] 变限积分求导；
- [ ] 反常积分与级数的 $p$ 判别；
- [ ] 极坐标、柱面坐标、球面坐标雅可比；
- [ ] 格林、高斯、斯托克斯公式；
- [ ] 一阶线性微分方程公式；
- [ ] 常系数线性微分方程特征根结构；
- [ ] 行列式与逆矩阵公式；
- [ ] 秩与线性方程组解的判定；
- [ ] 特征值、相似、对角化条件；
- [ ] 正定二次型判定；
- [ ] 条件概率、全概率与贝叶斯公式；
- [ ] 常见分布的期望和方差；
- [ ] 协方差、相关系数；
- [ ] 大数定律与中心极限定理；
- [ ] 正态总体三大抽样分布；
- [ ] 矩估计、最大似然估计和置信区间。

---

# 总结

整套考研数学公式可以用三条主线记忆：

$$
\boxed{
\text{高等数学：局部变化}
\rightarrow
\text{整体累积}
\rightarrow
\text{多元推广}
}
$$

$$
\boxed{
\text{线性代数：矩阵运算}
\rightarrow
\text{线性结构}
\rightarrow
\text{特征结构}
}
$$

$$
\boxed{
\text{概率统计：概率模型}
\rightarrow
\text{随机变量}
\rightarrow
\text{统计推断}
}
$$

> [!success] 复习原则
> 公式不能只背结果。每个公式至少要同时记住：
>
> 1. **适用条件**
> 2. **符号含义**
> 3. **常见变形**
> 4. **典型错误**
> 5. **对应题型**
