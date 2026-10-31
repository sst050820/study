/*
 * 文件名：05-平衡二叉树AVL.cpp
 * 章节：第7章 查找
 * 内容：平衡二叉树（AVL）的插入与四种旋转、平衡因子计算
 * 重要度：★★★★（手算旋转必会，代码了解即可）
 * 考研考点：AVL定义、平衡因子、四种失衡类型与旋转、插入后调整、查找效率
 */

#include <stdio.h>
#include <stdlib.h>

typedef int ElemType; // 元素类型

// ======1. AVL节点结构======
typedef struct AVLNode {
    ElemType data;              // 数据域
    struct AVLNode *lchild;     // 左孩子
    struct AVLNode *rchild;     // 右孩子
    int height;                 // ★ 以该节点为根的子树高度（叶子高度为1）
} AVLNode, *AVLTree;

// ======2. 获取树高（空树高度为0）======
int GetHeight(AVLTree T) {
    if(T == NULL) return 0;
    return T->height;
}

// ======3. 更新节点高度（左右子树高度的最大值+1）======
void UpdateHeight(AVLTree T) {
    int lh = GetHeight(T->lchild);
    int rh = GetHeight(T->rchild);
    T->height = (lh > rh ? lh : rh) + 1;
}

// ======4. 计算平衡因子 BF = 左子树高 - 右子树高======
int GetBalanceFactor(AVLTree T) {
    if(T == NULL) return 0;
    return GetHeight(T->lchild) - GetHeight(T->rchild);
}

/*
 * ★ AVL的定义：
 * 每个节点的平衡因子BF ∈ {-1, 0, 1}
 * 即左子树和右子树的高度差的绝对值不超过1
 * 空树高度定义为0，叶子节点高度为1
 */

// ======5. 右单旋（LL型失衡，★★★★ 手算必会）======
// 失衡节点A，左孩子B，在B的左子树插入导致A的BF=2
// 旋转：B作为新根，A作为B的右孩子，B原来的右子树作为A的左子树
AVLTree RightRotate(AVLTree A) {
    AVLTree B = A->lchild;        // B是A的左孩子
    A->lchild = B->rchild;        // B的右子树挂到A的左孩子
    B->rchild = A;                 // A挂到B的右孩子
    // ★ 先更新A的高度（A是B的子树），再更新B的高度
    UpdateHeight(A);
    UpdateHeight(B);
    return B;                      // B成为新根
}

// ======6. 左单旋（RR型失衡，★★★★ 手算必会）======
// 失衡节点A，右孩子B，在B的右子树插入导致A的BF=-2
// 旋转：B作为新根，A作为B的左孩子，B原来的左子树作为A的右子树
AVLTree LeftRotate(AVLTree A) {
    AVLTree B = A->rchild;        // B是A的右孩子
    A->rchild = B->lchild;        // B的左子树挂到A的右孩子
    B->lchild = A;                 // A挂到B的左孩子
    UpdateHeight(A);
    UpdateHeight(B);
    return B;                      // B成为新根
}

// ======7. AVL插入（★★★★ 核心：插入后检查平衡因子，执行旋转）======
AVLTree AVL_Insert(AVLTree T, ElemType key) {
    // 第一步：按BST规则插入
    if(T == NULL) {
        T = (AVLNode*)malloc(sizeof(AVLNode));
        T->data = key;
        T->lchild = T->rchild = NULL;
        T->height = 1;             // 新节点是叶子，高度为1
        return T;
    }
    if(key == T->data)
        return T;                  // 关键字已存在，不插入
    if(key < T->data)
        T->lchild = AVL_Insert(T->lchild, key);
    else
        T->rchild = AVL_Insert(T->rchild, key);

    // 第二步：更新当前节点高度
    UpdateHeight(T);

    // 第三步：计算平衡因子，检查是否失衡
    int bf = GetBalanceFactor(T);

    // ★★★ 四种失衡类型与对应旋转
    // LL型：BF=2 且 左孩子BF>=0 → 右单旋
    if(bf > 1 && GetBalanceFactor(T->lchild) >= 0)
        return RightRotate(T);

    // LR型：BF=2 且 左孩子BF<0 → 先左旋后右旋
    if(bf > 1 && GetBalanceFactor(T->lchild) < 0) {
        T->lchild = LeftRotate(T->lchild);   // 先对左孩子做左单旋
        return RightRotate(T);                // 再对当前节点做右单旋
    }

    // RR型：BF=-2 且 右孩子BF<=0 → 左单旋
    if(bf < -1 && GetBalanceFactor(T->rchild) <= 0)
        return LeftRotate(T);

    // RL型：BF=-2 且 右孩子BF>0 → 先右旋后左旋
    if(bf < -1 && GetBalanceFactor(T->rchild) > 0) {
        T->rchild = RightRotate(T->rchild);  // 先对右孩子做右单旋
        return LeftRotate(T);                 // 再对当前节点做左单旋
    }

    return T;  // 未失衡，直接返回
}

/*
 * ★ 旋转记忆口诀：
 * LL → 右单旋（左边长了，往右旋）
 * RR → 左单旋（右边长了，往左旋）
 * LR → 先左旋后右旋（先处理左孩子的右失衡，再整体右旋）
 * RL → 先右旋后左旋（先处理右孩子的左失衡，再整体左旋）
 *
 * ★ 关键性质：旋转后最小不平衡子树的高度恢复到插入前，因此上层无需再调整。
 */

// ======8. 中序遍历（验证BST性质）======
void InOrder(AVLTree T) {
    if(T != NULL) {
        InOrder(T->lchild);
        printf("%d ", T->data);
        InOrder(T->rchild);
    }
}

// ======9. 验证是否为合法AVL（检查所有节点BF∈{-1,0,1}）======
bool IsValidAVL(AVLTree T) {
    if(T == NULL) return true;
    int bf = GetBalanceFactor(T);
    if(bf < -1 || bf > 1) return false;
    return IsValidAVL(T->lchild) && IsValidAVL(T->rchild);
}

// ======10. 主函数测试======
int main() {
    printf("====== 平衡二叉树AVL测试开始 ======\n\n");

    // 【测试1】依次插入关键字，观察AVL构造过程
    printf("【测试1】AVL构造（依次插入10,20,30,40,50,25）\n");
    AVLTree T = NULL;
    ElemType keys[] = {10, 20, 30, 40, 50, 25};
    int n = 6;
    for(int i = 0; i < n; i++) {
        T = AVL_Insert(T, keys[i]);
        printf("       插入%d后：树高=%d，平衡因子根=%d，合法AVL？%s\n",
               keys[i], GetHeight(T), GetBalanceFactor(T),
               IsValidAVL(T) ? "是" : "否");
    }
    printf("       中序遍历：");
    InOrder(T);
    printf("（期望递增）\n");

    // 【测试2】LL型旋转演示
    printf("\n【测试2】LL型旋转演示（插入5,3,2，会触发LL型右单旋）\n");
    AVLTree T2 = NULL;
    T2 = AVL_Insert(T2, 5);
    printf("       插入5：根=5，高=%d\n", GetHeight(T2));
    T2 = AVL_Insert(T2, 3);
    printf("       插入3：根=5，左孩子=3，高=%d，根BF=%d\n", GetHeight(T2), GetBalanceFactor(T2));
    T2 = AVL_Insert(T2, 2);
    printf("       插入2：触发LL型右单旋！新根=%d，高=%d，根BF=%d\n",
           T2->data, GetHeight(T2), GetBalanceFactor(T2));
    printf("       旋转后：根=3，左孩子=2，右孩子=5\n");

    // 【测试3】RR型旋转演示
    printf("\n【测试3】RR型旋转演示（插入2,4,6，会触发RR型左单旋）\n");
    AVLTree T3 = NULL;
    T3 = AVL_Insert(T3, 2);
    T3 = AVL_Insert(T3, 4);
    printf("       插入2,4：根=2，右孩子=4，根BF=%d\n", GetBalanceFactor(T3));
    T3 = AVL_Insert(T3, 6);
    printf("       插入6：触发RR型左单旋！新根=%d，高=%d，根BF=%d\n",
           T3->data, GetHeight(T3), GetBalanceFactor(T3));
    printf("       旋转后：根=4，左孩子=2，右孩子=6\n");

    // 【测试4】LR型旋转演示
    printf("\n【测试4】LR型旋转演示（插入5,3,4，会触发LR型先左后右）\n");
    AVLTree T4 = NULL;
    T4 = AVL_Insert(T4, 5);
    T4 = AVL_Insert(T4, 3);
    printf("       插入5,3：根=5，左孩子=3，根BF=%d\n", GetBalanceFactor(T4));
    T4 = AVL_Insert(T4, 4);
    printf("       插入4：触发LR型！先对左孩子3左旋，再对根5右旋\n");
    printf("       新根=%d，高=%d，根BF=%d\n", T4->data, GetHeight(T4), GetBalanceFactor(T4));
    printf("       旋转后：根=4，左孩子=3，右孩子=5\n");

    // 【测试5】AVL vs BST 对比
    printf("\n【测试5】AVL vs BST 对比（递增插入1~7）\n");
    AVLTree T_avl = NULL;
    for(int i = 1; i <= 7; i++) T_avl = AVL_Insert(T_avl, i);
    printf("       AVL递增插入1~7：树高=%d（期望3，保持平衡）\n", GetHeight(T_avl));
    printf("       BST递增插入1~7：树高=7（退化为右斜树）\n");
    printf("       AVL查找效率始终O(log n)，BST最坏O(n)\n");

    // 【测试6】复杂度总结
    printf("\n【测试6】AVL复杂度总结\n");
    printf("       查找：O(log n)（树高始终O(log n)）\n");
    printf("       插入：O(log n)（查找位置O(log n) + 旋转O(1)）\n");
    printf("       删除：O(log n)（可能需要多次旋转向上调整）\n");
    printf("       空间：O(n)\n");
    printf("       n个节点AVL最大高度≈1.44log2(n+2)\n");

    printf("\n====== 平衡二叉树AVL测试结束 ======\n");
    return 0;
}
