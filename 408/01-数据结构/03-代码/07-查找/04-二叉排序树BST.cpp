/*
 * 文件名：04-二叉排序树BST.cpp
 * 章节：第7章 查找
 * 内容：二叉排序树（BST）的查找、插入、删除、构造、遍历、ASL计算
 * 重要度：★★★★★（必会代码+手算，大题和选择题都高频）
 * 考研考点：BST定义、中序遍历递增、查找插入删除代码、删除三种情况、查找效率分析
 */

#include <stdio.h>
#include <stdlib.h>

typedef int ElemType; // 元素类型

// ======1. BST节点结构（与第5章二叉树节点相同）======
typedef struct BSTNode {
    ElemType data;              // 数据域：关键字
    struct BSTNode *lchild;     // 左孩子指针（左子树所有节点 < 根）
    struct BSTNode *rchild;     // 右孩子指针（右子树所有节点 > 根）
} BSTNode, *BSTree;             // BSTNode是节点类型，BSTree是指向节点的指针类型

/*
 * ★ BST的核心性质：
 * 1. 左子树所有节点关键字 < 根节点关键字 < 右子树所有节点关键字
 * 2. 左子树和右子树递归地满足BST性质
 * 3. 中序遍历BST得到递增的有序序列（左-根-右 = 小-中-大）
 */

// ======2. BST查找（非递归，★★★★★ 能默写）======
BSTNode* BST_Search(BSTree T, ElemType key) {
    while(T != NULL && key != T->data) {
        if(key < T->data)
            T = T->lchild;    // ★ 目标更小，往左子树走
        else
            T = T->rchild;    // ★ 目标更大，往右子树走
    }
    return T;                  // 找到返回节点指针，失败返回NULL
}

// ======3. BST查找（递归版，了解）======
BSTNode* BST_Search_Recur(BSTree T, ElemType key) {
    if(T == NULL) return NULL;
    if(key == T->data) return T;
    if(key < T->data)
        return BST_Search_Recur(T->lchild, key);
    else
        return BST_Search_Recur(T->rchild, key);
}

// ======4. BST插入（★★★★★ 能默写）======
bool BST_Insert(BSTree &T, ElemType key) {
    if(T == NULL) {                                   // ★ 找到空位置，插入新节点
        T = (BSTNode*)malloc(sizeof(BSTNode));
        T->data = key;
        T->lchild = T->rchild = NULL;
        return true;
    }
    if(key == T->data)
        return false;                                  // ★ 关键字已存在，插入失败（BST不允许重复）
    if(key < T->data)
        return BST_Insert(T->lchild, key);            // 往左子树插
    else
        return BST_Insert(T->rchild, key);            // 往右子树插
}

/*
 * ★ 插入的特点：
 * 1. 新节点一定作为叶子节点插入（不会插到中间）
 * 2. 插入操作可能破坏树的平衡性（这就是需要AVL和红黑树的原因）
 * 3. 同样的关键字集合，插入顺序不同，BST形态可能完全不同
 */

// ======5. BST构造（依次插入关键字）======
void CreateBST(BSTree &T, ElemType keys[], int n) {
    T = NULL;                        // 初始为空树
    for(int i = 0; i < n; i++) {
        BST_Insert(T, keys[i]);     // 依次插入每个关键字
    }
}

// ======6. 中序遍历（★ 验证BST：中序遍历必为递增序列）======
void InOrder(BSTree T) {
    if(T != NULL) {
        InOrder(T->lchild);          // 先左
        printf("%d ", T->data);      // 再根
        InOrder(T->rchild);          // 后右
    }
}

// ======7. 求树高（用于ASL计算和效率分析）======
int TreeHeight(BSTree T) {
    if(T == NULL) return 0;
    int lh = TreeHeight(T->lchild);
    int rh = TreeHeight(T->rchild);
    return (lh > rh ? lh : rh) + 1;
}

// ======8. BST删除（★★★ 手算必会，代码了解）======
// 删除关键字为key的节点，返回是否删除成功
bool BST_Delete(BSTree &T, ElemType key) {
    if(T == NULL) return false;                      // 未找到

    if(key < T->data) {
        return BST_Delete(T->lchild, key);           // 往左子树找
    } else if(key > T->data) {
        return BST_Delete(T->rchild, key);           // 往右子树找
    } else {
        // ★ 找到要删除的节点T，分三种情况
        if(T->lchild == NULL) {
            // 情况1：左子树为空（含叶子节点和只有右子树的情况）
            BSTNode *temp = T;
            T = T->rchild;                            // 用右子树替代
            free(temp);
        } else if(T->rchild == NULL) {
            // 情况2：右子树为空（只有左子树）
            BSTNode *temp = T;
            T = T->lchild;                            // 用左子树替代
            free(temp);
        } else {
            // ★★★ 情况3：左右子树都不为空（最常考）
            // 用中序前驱（左子树中最右下的节点）替代被删节点
            BSTNode *p = T->lchild;                   // p指向左子树
            BSTNode *parent = T;
            while(p->rchild != NULL) {                // ★ 找左子树中最右下的节点（中序前驱）
                parent = p;
                p = p->rchild;
            }
            T->data = p->data;                        // 用前驱的值替代被删节点的值
            // 删除前驱节点p（p一定是叶子或只有左子树，转化为情况1或2）
            if(parent == T)
                parent->lchild = p->lchild;           // p是T的左孩子
            else
                parent->rchild = p->lchild;           // p在更下层
            free(p);
        }
        return true;
    }
}

/*
 * ★ 删除的核心思想：
 * 情况3用中序前驱/后继替代后，那个前驱/后继节点一定是叶子或只有一棵子树
 * （因为它是最左/最右的节点），所以删除它就转化为情况1或情况2。
 * 中序前驱 = 左子树中最右下的节点（左子树中最大的）
 * 中序后继 = 右子树中最左下的节点（右子树中最小的）
 */

// ======9. 计算BST的成功ASL（每个节点的比较次数=该节点的层数，根为第1层）======
void CalcASL_Helper(BSTree T, int level, int &totalCompare, int &nodeCount) {
    if(T == NULL) return;
    totalCompare += level;     // 该节点需要比较level次
    nodeCount++;
    CalcASL_Helper(T->lchild, level + 1, totalCompare, nodeCount);
    CalcASL_Helper(T->rchild, level + 1, totalCompare, nodeCount);
}

float Calc_BST_Success_ASL(BSTree T) {
    int total = 0, count = 0;
    CalcASL_Helper(T, 1, total, count);  // 根节点在第1层
    if(count == 0) return 0;
    return (float)total / count;
}

// ======10. 主函数测试======
int main() {
    printf("====== 二叉排序树BST测试开始 ======\n\n");

    // 【测试1】BST构造与中序遍历
    printf("【测试1】BST构造与中序遍历（验证中序递增）\n");
    ElemType keys[] = {45, 24, 53, 12, 37, 93, 28, 40, 50};
    int n = 9;
    BSTree T = NULL;
    CreateBST(T, keys, n);
    printf("       插入序列：");
    for(int i = 0; i < n; i++) printf("%d ", keys[i]);
    printf("\n       中序遍历：");
    InOrder(T);
    printf("（期望递增序列）\n");
    printf("       树高 = %d\n", TreeHeight(T));

    // 【测试2】BST查找
    printf("\n【测试2】BST查找\n");
    BSTNode *p1 = BST_Search(T, 37);
    printf("       查找37：%s（期望找到）\n", p1 ? "找到" : "未找到");
    BSTNode *p2 = BST_Search(T, 100);
    printf("       查找100：%s（期望未找到）\n", p2 ? "找到" : "未找到");

    // 【测试3】BST插入
    printf("\n【测试3】BST插入\n");
    bool ins1 = BST_Insert(T, 30);
    printf("       插入30：%s（期望成功）\n", ins1 ? "成功" : "失败");
    bool ins2 = BST_Insert(T, 45);
    printf("       插入45（已存在）：%s（期望失败）\n", ins2 ? "成功" : "失败");
    printf("       插入后中序遍历：");
    InOrder(T);
    printf("\n");

    // 【测试4】BST删除（三种情况）
    printf("\n【测试4】BST删除\n");
    // 删除叶子节点30
    bool del1 = BST_Delete(T, 30);
    printf("       删除叶子30：%s，中序：", del1 ? "成功" : "失败");
    InOrder(T); printf("\n");
    // 删除只有左子树的节点12（12的右子树为空）
    bool del2 = BST_Delete(T, 12);
    printf("       删除节点12（只有右子树为空）：%s，中序：", del2 ? "成功" : "失败");
    InOrder(T); printf("\n");
    // 删除有左右子树的节点24（用中序前驱28替代）
    bool del3 = BST_Delete(T, 24);
    printf("       删除节点24（有左右子树，用前驱28替代）：%s，中序：", del3 ? "成功" : "失败");
    InOrder(T); printf("\n");

    // 【测试5】ASL计算
    printf("\n【测试5】BST成功ASL计算\n");
    float asl = Calc_BST_Success_ASL(T);
    printf("       当前BST成功ASL = %.2f\n", asl);
    printf("       （每个节点的比较次数=该节点的层数，根为第1层）\n");

    // 【测试6】插入顺序对BST形态的影响
    printf("\n【测试6】插入顺序对BST形态的影响（★ 重要结论）\n");
    // 递增顺序插入 → 退化为右斜树
    ElemType incKeys[] = {1, 2, 3, 4, 5};
    BSTree T_inc = NULL;
    CreateBST(T_inc, incKeys, 5);
    printf("       递增插入[1,2,3,4,5]：树高=%d（期望5，退化为右斜树）\n", TreeHeight(T_inc));
    printf("       查找效率退化为O(n)，这就是BST的最大问题\n");

    // 随机顺序插入 → 较平衡
    ElemType randKeys[] = {3, 1, 4, 2, 5};
    BSTree T_rand = NULL;
    CreateBST(T_rand, randKeys, 5);
    printf("       随机插入[3,1,4,2,5]：树高=%d（期望3，较平衡）\n", TreeHeight(T_rand));

    printf("\n       ★ 结论：同样的关键字集合，插入顺序不同，BST形态可能完全不同。\n");
    printf("         AVL和红黑树就是为了解决BST查找效率不稳定的问题。\n");

    // 释放内存（简单处理，实际应递归释放）
    printf("\n====== 二叉排序树BST测试结束 ======\n");
    return 0;
}
