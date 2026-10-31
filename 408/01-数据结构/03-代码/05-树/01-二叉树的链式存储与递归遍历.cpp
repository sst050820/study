/*
 * ============================================================
 * 文件名：01-二叉树的链式存储与递归遍历.cpp
 * 章  节：第五章 树与二叉树
 * 内  容：二叉树的二叉链表存储 + 先序/中序/后序递归遍历 + 求深度/结点数/叶子数
 * 重要度：★★★★★ 必须熟练掌握（本章一切代码的地基，先中后序递归是必背题）
 * 考研考点：递归遍历顺序、求树高（后序思想）、空树边界处理
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef int ElemType; // 二叉树结点存放的数据类型（考试常用int，也可以是char）

/*
 * ============================================================
 * 二叉树的链式存储结构（二叉链表，★★★★★ 必背）：
 *   - data    ：数据域
 *   - lchild  ：左孩子指针，指向左子树的根
 *   - rchild  ：右孩子指针，指向右子树的根
 * 特点：n个结点有2n个指针域，n-1个非空（对应n-1条边），
 *       所以空链域 = 2n-(n-1) = n+1 个（这是线索二叉树的基础，选择题常考）
 * ============================================================
 */
typedef struct BiTNode {
    ElemType data;              // 数据域：存储结点的值
    struct BiTNode *lchild;     // 左孩子指针：指向左子树
    struct BiTNode *rchild;     // 右孩子指针：指向右子树
} BiTNode, *BiTree;             // BiTNode是结点类型；BiTree是指向结点的指针类型

// ======1. 创建单个结点（工具函数）======
BiTNode *CreateNode(ElemType e) {
    BiTNode *p = (BiTNode *)malloc(sizeof(BiTNode)); // 分配结点内存
    p->data = e;                                     // 数据域赋值
    p->lchild = NULL;                                // 左右孩子先置空
    p->rchild = NULL;
    return p;                                        // 返回新结点
}

/*
 * ======2. 按"先序序列"建树（以-1表示空结点）======
 * 例：输入序列 1 2 4 -1 -1 5 -1 -1 3 -1 -1 对应先序：1,2,4,5,3
 *     （建树用的是递归，考试若只让写遍历，这个函数可当工具用）
 * 说明：递归函数要"能改掉传入的指针本身"，所以参数必须用 BiTree &T（引用）
 */
void CreateBiTree(BiTree &T) {
    ElemType x;
    scanf("%d", &x);                 // 读入一个值
    if(x == -1) {                    // -1 代表空结点（虚结点）
        T = NULL;                    // 空子树
    } else {
        T = CreateNode(x);           // 先造出根结点
        CreateBiTree(T->lchild);     // ★ 再递归建左子树
        CreateBiTree(T->rchild);     // ★ 再递归建右子树
    }
}

// ======3. 先序遍历（根左右，★★★★★ 必背）======
void PreOrder(BiTree T) {
    if(T != NULL) {                  // 空树直接结束（递归出口）
        printf("%d ", T->data);      // ★ 1.访问根结点
        PreOrder(T->lchild);         // ★ 2.先序遍历左子树
        PreOrder(T->rchild);         // ★ 3.先序遍历右子树
    }
}

// ======4. 中序遍历（左根右，★★★★★ 必背）======
void InOrder(BiTree T) {
    if(T != NULL) {
        InOrder(T->lchild);          // ★ 1.中序遍历左子树
        printf("%d ", T->data);      // ★ 2.访问根结点（三兄弟唯一区别就是这行位置）
        InOrder(T->rchild);          // ★ 3.中序遍历右子树
    }
}

// ======5. 后序遍历（左右根，★★★★★ 必背）======
void PostOrder(BiTree T) {
    if(T != NULL) {
        PostOrder(T->lchild);        // ★ 1.后序遍历左子树
        PostOrder(T->rchild);        // ★ 2.后序遍历右子树
        printf("%d ", T->data);      // ★ 3.访问根结点
    }
}

/*
 * ======6. 求树的深度（高度，★★★★ 常考，用"后序"思想）======
 * 思想：树高 = max(左子树高, 右子树高) + 1
 * 为什么是后序：必须先知道左右子树的高度，才能算出当前结点为根的高度
 * 空树返回0；只有一个根结点返回1
 */
int TreeDepth(BiTree T) {
    if(T == NULL) {                  // 空树高度为0（递归出口）
        return 0;
    }
    int ldep = TreeDepth(T->lchild); // ★ 先求左子树深度
    int rdep = TreeDepth(T->rchild); // ★ 再求右子树深度
    return (ldep > rdep ? ldep : rdep) + 1; // ★ 取较大者+1
}

// ======7. 求结点总数（先序思想：每访问一个结点就计数）======
int CountNodes(BiTree T) {
    if(T == NULL) {                  // 空树0个结点
        return 0;
    }
    return 1 + CountNodes(T->lchild) + CountNodes(T->rchild); // 根 + 左 + 右
}

// ======8. 求叶子结点数（度为0的结点个数）======
int CountLeaves(BiTree T) {
    if(T == NULL) {                  // 空树没有叶子
        return 0;
    }
    if(T->lchild == NULL && T->rchild == NULL) { // ★ 左右孩子都空 → 自己是叶子
        return 1;
    }
    // 不是叶子：统计左右子树中的叶子之和
    return CountLeaves(T->lchild) + CountLeaves(T->rchild);
}

// ======主函数：测试所有函数======
int main() {
    BiTree T = NULL;                 // 定义一棵空二叉树

    printf("====== 二叉树链式存储与递归遍历测试开始 ======\n\n");

    // 测试1：手工建一棵树，避免输入（更直观）
    // 树形：1是根；2、3是1的孩子；4、5是2的孩子；6、7是3的孩子（完全二叉树）
    printf("【测试1】手工建树（完全二叉树 1~7）\n");
    T = CreateNode(1);                           // 根
    T->lchild = CreateNode(2);
    T->rchild = CreateNode(3);
    T->lchild->lchild = CreateNode(4);
    T->lchild->rchild = CreateNode(5);
    T->rchild->lchild = CreateNode(6);
    T->rchild->rchild = CreateNode(7);

    printf("       先序序列（根左右）："); PreOrder(T); printf("\n");
    printf("       中序序列（左根右）："); InOrder(T);  printf("\n");
    printf("       后序序列（左右根）："); PostOrder(T); printf("\n");

    // 测试2：统计类
    printf("\n【测试2】统计信息\n");
    printf("       树的深度 = %d\n", TreeDepth(T));      // 期望3
    printf("       结点总数 = %d\n", CountNodes(T));     // 期望7
    printf("       叶子结点数 = %d\n", CountLeaves(T));  // 期望4（4,5,6,7）

    // 测试3：单结点树与空树（边界）
    printf("\n【测试3】边界情况\n");
    BiTree Single = CreateNode(100);             // 只有根
    printf("       单结点树：深度=%d 结点数=%d 叶子=%d\n",
           TreeDepth(Single), CountNodes(Single), CountLeaves(Single));
    BiTree Empty = NULL;                         // 空树
    printf("       空树：深度=%d 结点数=%d 叶子=%d\n",
           TreeDepth(Empty), CountNodes(Empty), CountLeaves(Empty));

    // 测试4：先序输入建树（-1表示空），验证CreateBiTree
    printf("\n【测试4】按先序序列输入建树（-1表示空结点）\n");
    printf("       请输入：1 2 -1 -1 3 -1 -1\n");
    BiTree T2 = NULL;
    CreateBiTree(T2);
    printf("       建树成功，先序："); PreOrder(T2); printf("\n");
    printf("       建树成功，中序："); InOrder(T2);  printf("\n");

    printf("\n====== 二叉树链式存储与递归遍历测试结束 ======\n");
    return 0;
}
