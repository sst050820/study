/*
 * ============================================================
 * 文件名：05-线索二叉树.cpp
 * 章  节：第五章 树与二叉树
 * 内  容：中序线索二叉树：线索化、找中序前驱/后继、线索遍历（非递归）
 * 重要度：★★★★★ 必须熟练掌握（考研大题高频：线索化+遍历序列）
 * 考研考点：ltag/rtag含义、pre指针的妙用、最后一个结点处理、
 *          中序后继=右子树最左下、中序前驱=左子树最右下
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef int ElemType; // 结点数据类型

/*
 * ============================================================
 * 线索二叉树结点（★★★★★ 结构体必背）：
 *   - lchild/rchild：孩子指针 或 线索指针（由tag决定）
 *   - ltag：0=lchild指向左孩子；1=lchild指向前驱线索
 *   - rtag：0=rchild指向右孩子；1=rchild指向后继线索
 * 为什么要线索化：普通二叉树找前驱/后继必须重新遍历（O(n)），
 * 利用 n+1 个空链域存线索，就能从任意结点直接找前驱后继
 * ============================================================
 */
typedef struct ThreadNode {
    ElemType data;                        // 数据域
    struct ThreadNode *lchild, *rchild;   // 左右指针（孩子或线索）
    int ltag, rtag;                       // ★ 左右标志：0=孩子，1=线索
} ThreadNode, *ThreadTree;

// ======1. 创建普通结点（工具函数，tag初始为0=孩子）======
ThreadNode *CreateNode(ElemType e) {
    ThreadNode *p = (ThreadNode *)malloc(sizeof(ThreadNode));
    p->data = e;
    p->lchild = NULL;
    p->rchild = NULL;
    p->ltag = 0;                          // 初始都是孩子指针
    p->rtag = 0;
    return p;
}

// ======2. 手工建一棵普通二叉树（测试用）======
// 树形：1是根；2、3是1的孩子；4、5是2的孩子；6是3的右孩子
ThreadTree BuildTree() {
    ThreadTree T = CreateNode(1);
    T->lchild = CreateNode(2);
    T->rchild = CreateNode(3);
    T->lchild->lchild = CreateNode(4);
    T->lchild->rchild = CreateNode(5);
    T->rchild->rchild = CreateNode(6);
    return T;
}

/*
 * ======3. 中序线索化核心函数（★★★★★ 必背）======
 * 实质：中序遍历（左根右）+ 一边遍历一边把空链域改成线索
 * pre：指向"当前访问结点p的前驱"，用引用参数传递（效果=全局变量）
 * 两个动作（每次访问到p时执行）：
 *   ① p左孩子为空 → p.lchild = pre（p的前驱线索），ltag=1
 *   ② pre右孩子为空 → pre.rchild = p（pre的后继线索），rtag=1
 * 然后 pre = p（把p交给下一次访问）
 */
void InThread(ThreadTree &p, ThreadNode *&pre) {
    if(p == NULL) {                       // 空子树返回
        return;
    }
    InThread(p->lchild, pre);             // ★ 1.先线索化左子树（中序"左"）
    if(p->lchild == NULL) {               // ★ 2.处理当前结点p（中序"根"）
        p->lchild = pre;                  // 左孩子为空：左指针指向前驱
        p->ltag = 1;                      // 标记为前驱线索
    }
    if(pre != NULL && pre->rchild == NULL) { // ★ 处理前驱pre的右指针
        pre->rchild = p;                  // pre右孩子为空：右指针指向p（p是pre的后继）
        pre->rtag = 1;
    }
    pre = p;                              // ★ 3.更新pre为当前结点
    InThread(p->rchild, pre);             // ★ 4.再线索化右子树（中序"右"）
}

// ======4. 中序线索化入口函数（★★★★★ 必背）======
void CreateInThread(ThreadTree T) {
    ThreadNode *pre = NULL;               // 前驱初始为空
    if(T != NULL) {
        InThread(T, pre);                 // 线索化整棵树
        pre->rchild = NULL;               // ★ 最后一个结点的右指针置空
        pre->rtag = 1;                    // 并标记为线索
        /*
         * 为什么可以放心置NULL：
         * 中序顺序是"左根右"，若最后访问的结点还有右孩子，
         * 右孩子必在它之后被访问 → 矛盾
         * 所以中序（后序同理）最后一个被访问的结点必无右孩子
         */
    }
}

// ======5. 求"一棵子树中中序第一个被访问的结点"（最左下）======
ThreadNode *FirstNode(ThreadNode *p) {
    while(p->ltag == 0) {                 // ★ 一直往左走，直到左指针是线索
        p = p->lchild;                    // 左指针是孩子就继续下探
    }
    return p;                             // 最左下结点 = 中序第一个
}

// ======6. 求"一棵子树中中序最后一个被访问的结点"（最右下）======
ThreadNode *LastNode(ThreadNode *p) {
    while(p->rtag == 0) {                 // 一直往右走
        p = p->rchild;
    }
    return p;                             // 最右下结点 = 中序最后一个
}

/*
 * ======7. 找结点p的中序后继（★★★★★ 必背）======
 * ① rtag==1：右指针就是后继线索，直接返回
 * ② rtag==0：p有右孩子 → 后继 = 右子树中序第一个结点（最左下）
 */
ThreadNode *NextNode(ThreadNode *p) {
    if(p->rtag == 1) {                    // ① 右指针是线索
        return p->rchild;                 // 直接返回后继
    }
    return FirstNode(p->rchild);          // ② 右子树最左下
}

/*
 * ======8. 找结点p的中序前驱（★★★★★ 必背）======
 * ① ltag==1：左指针就是前驱线索，直接返回
 * ② ltag==0：p有左孩子 → 前驱 = 左子树中序最后一个结点（最右下）
 */
ThreadNode *PreNode(ThreadNode *p) {
    if(p->ltag == 1) {                    // ① 左指针是线索
        return p->lchild;                 // 直接返回前驱
    }
    return LastNode(p->lchild);           // ② 左子树最右下
}

/*
 * ======9. 中序线索二叉树的正向遍历（★★★★★ 非递归O(1)空间）======
 * 思路：找到全树第一个结点（根的最左下）→ 访问 → 反复找后继
 * 不需要栈、不需要递归 → 空间O(1)，这就是线索化的意义之一
 */
void InOrderByThread(ThreadTree T) {
    for(ThreadNode *p = FirstNode(T); p != NULL; p = NextNode(p)) {
        printf("%d ", p->data);           // 访问p，然后p=p的后继
    }
}

// ======10. 中序线索二叉树的逆向遍历（了解）======
void ReverseInOrderByThread(ThreadTree T) {
    for(ThreadNode *p = LastNode(T); p != NULL; p = PreNode(p)) {
        printf("%d ", p->data);           // 从最后往前找前驱
    }
}

// ======主函数：测试线索化与找前驱后继======
int main() {
    printf("====== 中序线索二叉树测试开始 ======\n\n");

    // 测试1：建树并线索化
    printf("【测试1】建树 + 中序线索化\n");
    ThreadTree T = BuildTree();
    CreateInThread(T);                    // 线索化后，空链域全部变为线索
    printf("       线索化完成（空链域已全部改为线索）\n");

    // 测试2：线索遍历（应与普通中序遍历结果一致）
    printf("\n【测试2】中序线索遍历（非递归、O(1)空间）\n");
    printf("       期望：4 2 5 1 3 6\n");
    printf("       实际：");
    InOrderByThread(T);
    printf("\n");
    printf("       逆向中序遍历：");
    ReverseInOrderByThread(T);
    printf("\n");

    // 测试3：找任意结点的中序后继/前驱
    printf("\n【测试3】找中序后继/前驱（按值找结点再演示）\n");
    // 找值=1的根结点：它的中序后继应该是3（右子树最左下=3）
    ThreadNode *p1 = T;                   // 根结点（值1）
    ThreadNode *succ1 = NextNode(p1);
    ThreadNode *pred1 = PreNode(p1);
    printf("       结点1：后继=%d（期望3），前驱=%d（期望5）\n",
           succ1 ? succ1->data : -1, pred1 ? pred1->data : -1);
    // 找结点5（在左子树里）：先手动定位
    ThreadNode *p5 = T->lchild->rchild;   // 值5
    ThreadNode *succ5 = NextNode(p5);
    ThreadNode *pred5 = PreNode(p5);
    printf("       结点5：后继=%d（期望1），前驱=%d（期望2）\n",
           succ5 ? succ5->data : -1, pred5 ? pred5->data : -1);
    // 结点4是最左下，无前驱；结点6是最后，无后继
    ThreadNode *p4 = T->lchild->lchild;   // 值4
    ThreadNode *p6 = T->rchild->rchild;   // 值6
    printf("       结点4：前驱=%d（期望-1即NULL）\n",
           PreNode(p4) ? PreNode(p4)->data : -1);
    printf("       结点6：后继=%d（期望-1即NULL）\n",
           NextNode(p6) ? NextNode(p6)->data : -1);

    /*
     * 【总结】中序线索树能同时方便地找前驱和后继；
     * 先序线索树只能方便找后继（有左孩则左孩，无左孩则右孩）；
     * 后序线索树只能方便找前驱（有右孩则右孩，无右孩则左孩）。
     * 记忆：线索化用哪个序，就按哪个序找前驱后继；"短板"方向
     *       （先序的前驱/后序的后继）在普通二叉链表里找不到。
     */

    printf("\n====== 中序线索二叉树测试结束 ======\n");
    return 0;
}
