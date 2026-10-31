/*
 * ============================================================
 * 文件名：04-由遍历序列构造二叉树.cpp
 * 章  节：第五章 树与二叉树
 * 内  容：先序+中序 / 后序+中序 / 层序+中序 构造二叉树
 * 重要度：★★★★ 重点掌握（选择题+大题必考，"必须有中序"是铁律）
 * 考研考点：唯一确定二叉树的条件、递归切分左右子树区间
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef int ElemType; // 结点数据类型

/*
 * ============================================================
 * 二叉链表结构体
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

// ======2. 在中序序列中查找值e的位置（返回下标，找不到返回-1）======
int FindPos(int in[], int inL, int inR, ElemType e) {
    for(int i = inL; i <= inR; i++) {
        if(in[i] == e) {
            return i;           // ★ 找到返回下标
        }
    }
    return -1;                  // 找不到（正常情况不会发生）
}

/*
 * ======3. 由先序序列+中序序列构造二叉树（★★★★★ 必背）======
 * 原理：
 *   - 先序第一个元素必是根
 *   - 在 中序 里找到根的位置k：k左边全是左子树结点，k右边全是右子树结点
 *   - 左子树结点数 = k - inL，用它把先序也切成左右两半，递归
 * 参数含义：pre[preL..preR]是先序区间，in[inL..inR]是中序区间
 */
BiTNode *BuildByPreAndIn(int pre[], int preL, int preR,
                         int in[],  int inL,  int inR) {
    if(preL > preR || inL > inR) {   // ★ 区间为空，说明没有子树
        return NULL;
    }
    ElemType rootVal = pre[preL];    // ★ 先序第一个 = 根
    BiTNode *root = CreateNode(rootVal);
    int k = FindPos(in, inL, inR, rootVal); // 根在中序中的位置
    int leftLen = k - inL;           // ★ 左子树结点数（关键）
    // 递归建左子树：先序[leftLen个]对应中序[inL..k-1]
    root->lchild = BuildByPreAndIn(pre, preL + 1, preL + leftLen,
                                   in,  inL,      k - 1);
    // 递归建右子树：先序剩下的对应中序[k+1..inR]
    root->rchild = BuildByPreAndIn(pre, preL + leftLen + 1, preR,
                                   in,  k + 1,              inR);
    return root;
}

/*
 * ======4. 由后序序列+中序序列构造二叉树（★★★★ 必会）======
 * 原理与上面镜像：
 *   - 后序最后一个元素必是根
 *   - 同样用中序划分左右子树，再切后序区间
 */
BiTNode *BuildByPostAndIn(int post[], int postL, int postR,
                          int in[],   int inL,   int inR) {
    if(postL > postR || inL > inR) { // 空区间
        return NULL;
    }
    ElemType rootVal = post[postR];  // ★ 后序最后一个 = 根
    BiTNode *root = CreateNode(rootVal);
    int k = FindPos(in, inL, inR, rootVal);  // 根在中序中的位置
    int leftLen = k - inL;           // 左子树结点数
    // 左子树后序：[postL, postL+leftLen-1]；右子树后序：[postL+leftLen, postR-1]
    root->lchild = BuildByPostAndIn(post, postL, postL + leftLen - 1,
                                    in,   inL,   k - 1);
    root->rchild = BuildByPostAndIn(post, postL + leftLen, postR - 1,
                                    in,   k + 1,           inR);
    return root;
}

/*
 * ======5. 由层序序列+中序序列构造二叉树（★★★★ 会推导）======
 * 原理：
 *   - 层序第一个元素必是整棵树的根
 *   - 关键推论：对任意区间，层序中"第一个落在该区间内的结点"
 *     就是该区间的根（因为层序保证上层结点先出现）
 * 实现：每次递归都从头扫描层序，找第一个属于当前中序区间的值
 */
BiTNode *BuildByLevelAndIn(int level[], int n,
                           int in[], int inL, int inR) {
    if(inL > inR) {              // 空区间
        return NULL;
    }
    // ★ 找层序中第一个落在[inL,inR]区间的结点作根
    int rootVal = -1, k = -1;
    for(int i = 0; i < n; i++) {          // 扫层序
        for(int j = inL; j <= inR; j++) { // 找它在中序区间的位置
            if(level[i] == in[j]) {
                rootVal = level[i];
                k = j;                    // 记下中序位置
                break;
            }
        }
        if(rootVal != -1) {
            break;                        // 找到了就停止（层序靠前=层数浅=根）
        }
    }
    BiTNode *root = CreateNode(rootVal);
    root->lchild = BuildByLevelAndIn(level, n, in, inL, k - 1);
    root->rchild = BuildByLevelAndIn(level, n, in, k + 1, inR);
    return root;
}

// ======6. 先序遍历（验证用）======
void PreOrder(BiTree T) {
    if(T != NULL) {
        printf("%d ", T->data);
        PreOrder(T->lchild);
        PreOrder(T->rchild);
    }
}

// ======7. 中序遍历（验证用）======
void InOrder(BiTree T) {
    if(T != NULL) {
        InOrder(T->lchild);
        printf("%d ", T->data);
        InOrder(T->rchild);
    }
}

// ======8. 后序遍历（验证用）======
void PostOrder(BiTree T) {
    if(T != NULL) {
        PostOrder(T->lchild);
        PostOrder(T->rchild);
        printf("%d ", T->data);
    }
}

// ======主函数：测试三种构造方法======
int main() {
    /*
     * 以这棵树为验证目标：
     *        1
     *       / \
     *      2   3
     *     / \   \
     *    4   5   6
     * 先序：1 2 4 5 3 6
     * 中序：4 2 5 1 3 6
     * 后序：4 5 2 6 3 1
     * 层序：1 2 3 4 5 6
     */
    int pre[]  = {1, 2, 4, 5, 3, 6};
    int in[]   = {4, 2, 5, 1, 3, 6};
    int post[] = {4, 5, 2, 6, 3, 1};
    int level[]= {1, 2, 3, 4, 5, 6};
    int n = 6;

    printf("====== 由遍历序列构造二叉树测试开始 ======\n\n");

    printf("【测试1】先序+中序 构造\n");
    BiTree T1 = BuildByPreAndIn(pre, 0, n - 1, in, 0, n - 1);
    printf("       重建后先序："); PreOrder(T1);  printf("\n");
    printf("       重建后中序："); InOrder(T1);   printf("\n");
    printf("       重建后后序："); PostOrder(T1); printf("\n");

    printf("\n【测试2】后序+中序 构造\n");
    BiTree T2 = BuildByPostAndIn(post, 0, n - 1, in, 0, n - 1);
    printf("       重建后先序："); PreOrder(T2);  printf("\n");
    printf("       重建后中序："); InOrder(T2);   printf("\n");
    printf("       重建后后序："); PostOrder(T2); printf("\n");

    printf("\n【测试3】层序+中序 构造（进阶）\n");
    BiTree T3 = BuildByLevelAndIn(level, n, in, 0, n - 1);
    printf("       重建后先序："); PreOrder(T3);  printf("\n");
    printf("       重建后中序："); InOrder(T3);   printf("\n");
    printf("       重建后后序："); PostOrder(T3); printf("\n");

    /*
     * 【提醒】为什么"先序+后序"不行？
     * 两个结点的树：根A，B可以是左孩子也可以是右孩子
     *   先序都是 AB，后序都是 BA → 无法区分 → 不能唯一确定
     * 唯一例外：限定每个内部结点度都为2（满二叉树），先序+后序才行
     */

    printf("\n====== 由遍历序列构造二叉树测试结束 ======\n");
    return 0;
}
