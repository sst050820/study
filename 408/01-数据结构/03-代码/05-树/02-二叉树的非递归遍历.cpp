/*
 * ============================================================
 * 文件名：02-二叉树的非递归遍历.cpp
 * 章  节：第五章 树与二叉树
 * 内  容：先序/中序/后序的非递归遍历（用栈模拟系统调用栈）
 * 重要度：★★★★ 重点掌握（中序非递归是算法题高频，后序双栈必会推导）
 * 考研考点：栈的作用、中序非递归的"左路入栈+出栈访问+转向右"框架、
 *          后序双栈法为什么能凑出"左右根"
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef int ElemType; // 结点数据类型

/*
 * ============================================================
 * 二叉链表结构体（同01号文件，★★★ 二叉树的存储就是它）
 * ============================================================
 */
typedef struct BiTNode {
    ElemType data;              // 数据域
    struct BiTNode *lchild;     // 左孩子
    struct BiTNode *rchild;     // 右孩子
} BiTNode, *BiTree;

// ======1. 创建单个结点（工具函数）======
BiTNode *CreateNode(ElemType e) {
    BiTNode *p = (BiTNode *)malloc(sizeof(BiTNode));
    p->data = e;
    p->lchild = NULL;
    p->rchild = NULL;
    return p;
}

/*
 * ============================================================
 * 链栈（存结点指针，★★★ 复习第3章栈的链式实现）
 * 为什么用链栈：树的高度不可预知，顺序栈要预分配大空间，链栈更安全
 * ============================================================
 */
typedef struct StackNode {
    BiTNode *data;              // ★ 栈里存的是"结点指针"，不是结点本身
    struct StackNode *next;     // 指向下一个栈元素
} StackNode, *LinkStack;

// ======2. 初始化链栈（栈顶置空）======
void InitStack(LinkStack &S) {
    S = NULL;                   // 空栈
}

// ======3. 入栈======
void Push(LinkStack &S, BiTNode *p) {
    StackNode *s = (StackNode *)malloc(sizeof(StackNode)); // 新栈元素
    s->data = p;                // 存入结点指针
    s->next = S;                // 头插法：新元素成为栈顶
    S = s;
}

// ======4. 出栈（弹出栈顶到p）======
bool Pop(LinkStack &S, BiTNode *&p) {
    if(S == NULL) {             // 空栈不能出
        return false;
    }
    p = S->data;                // 取栈顶元素
    StackNode *q = S;           // 临时保存栈顶结点
    S = S->next;                // 栈顶下移
    free(q);                    // 释放栈元素内存
    return true;
}

// ======5. 取栈顶元素（不弹出）======
bool GetTop(LinkStack S, BiTNode *&p) {
    if(S == NULL) {
        return false;
    }
    p = S->data;                // 只读栈顶
    return true;
}

// ======6. 判栈空======
bool StackEmpty(LinkStack S) {
    return S == NULL;
}

/*
 * ======7. 先序非递归遍历（根左右）======
 * 思路：根入栈；循环——
 *   弹出一个结点p，访问p；
 *   先把右孩子入栈（后出），再把左孩子入栈（先出）
 * 这样出栈顺序就是 根 → 左 → 右，与先序一致
 */
void PreOrderNonRec(BiTree T) {
    if(T == NULL) {             // 空树直接返回
        return;
    }
    LinkStack S;                // 辅助栈
    InitStack(S);
    Push(S, T);                 // 根结点入栈
    while(!StackEmpty(S)) {     // ★ 栈不空就继续
        BiTNode *p;
        Pop(S, p);              // 出栈一个结点
        printf("%d ", p->data); // 访问它（先序：出栈即访问）
        if(p->rchild != NULL) { // ★ 右孩子先入栈
            Push(S, p->rchild);
        }
        if(p->lchild != NULL) { // ★ 左孩子后入栈（后入先出，左先被访问）
            Push(S, p->lchild);
        }
    }
}

/*
 * ======8. 中序非递归遍历（左根右，★★★★★ 最重要）======
 * 框架（必背）：
 *   1. 沿着左链一直往左走，途中结点全部入栈（不访问）；
 *   2. 走到最左下（左孩子为空），出栈一个结点并访问（★ 访问时机在出栈时）；
 *   3. 转向该结点的右孩子，重复上述过程。
 * 类比：递归中序 = 系统栈替我们做了这套动作，这里用手工栈复刻。
 */
void InOrderNonRec(BiTree T) {
    LinkStack S;                // 辅助栈
    InitStack(S);
    BiTNode *p = T;             // 工作指针
    while(p != NULL || !StackEmpty(S)) {  // ★ 结点没走完 或 栈没空 都要继续
        if(p != NULL) {
            Push(S, p);         // ① 左路入栈
            p = p->lchild;      // 往左走
        } else {
            Pop(S, p);          // ② 左到底了，出栈一个
            printf("%d ", p->data);  // ★ 出栈时访问（中序：根在左右之间被访问）
            p = p->rchild;      // ③ 转向右子树
        }
    }
}

/*
 * ======9. 后序非递归遍历：双栈法（★★★ 会推导即可）======
 * 为什么不能直接仿中序：后序要求"左右根"，访问根时右子树已访问完，
 * 但弹栈时无法判断右子树是否处理过（栈只记住路径，记不住状态）。
 * 双栈法思路：
 *   先序是"根左右"，如果按"根右左"的顺序压栈/出栈，
 *   再把得到的序列逆序（倒进另一个栈）就是"左右根"= 后序。
 * 步骤：
 *   根入栈1；循环：栈1出p、访问顺序存入栈2；
 *   先压p的左孩子、再压p的右孩子（保证栈1出栈顺序为 根右左）
 *   最后把栈2全部弹出输出 → 左右根
 */
void PostOrderNonRec(BiTree T) {
    if(T == NULL) {
        return;
    }
    LinkStack S1, S2;           // S1辅助遍历，S2收集逆序
    InitStack(S1);
    InitStack(S2);
    Push(S1, T);                // 根入S1
    while(!StackEmpty(S1)) {
        BiTNode *p;
        Pop(S1, p);             // 弹出
        Push(S2, p);            // ★ 压入S2（收集结果）
        if(p->lchild != NULL) { // ★ 左孩子先入S1（S1出栈顺序=根右左）
            Push(S1, p->lchild);
        }
        if(p->rchild != NULL) { // 右孩子后入S1
            Push(S1, p->rchild);
        }
    }
    while(!StackEmpty(S2)) {    // ★ S2弹出顺序 = 后序
        BiTNode *p;
        Pop(S2, p);
        printf("%d ", p->data);
    }
}

// ======主函数：测试所有非递归遍历======
int main() {
    // 建一棵树：1是根；2、3是1的孩子；4、5是2的孩子；6是3的右孩子
    BiTree T = CreateNode(1);
    T->lchild = CreateNode(2);
    T->rchild = CreateNode(3);
    T->lchild->lchild = CreateNode(4);
    T->lchild->rchild = CreateNode(5);
    T->rchild->rchild = CreateNode(6);

    printf("====== 二叉树非递归遍历测试开始 ======\n\n");

    printf("【测试1】先序非递归\n");
    printf("       期望：1 2 4 5 3 6\n");
    printf("       实际：");
    PreOrderNonRec(T);
    printf("\n");

    printf("\n【测试2】中序非递归\n");
    printf("       期望：4 2 5 1 3 6\n");
    printf("       实际：");
    InOrderNonRec(T);
    printf("\n");

    printf("\n【测试3】后序非递归（双栈法）\n");
    printf("       期望：4 5 2 6 3 1\n");
    printf("       实际：");
    PostOrderNonRec(T);
    printf("\n");

    printf("\n【测试4】空树与单结点（边界）\n");
    BiTree Empty = NULL;
    PreOrderNonRec(Empty);      // 什么都不输出
    BiTree Single = CreateNode(7);
    printf("       单结点后序：");
    PostOrderNonRec(Single);
    printf("\n");

    printf("\n====== 二叉树非递归遍历测试结束 ======\n");
    return 0;
}
