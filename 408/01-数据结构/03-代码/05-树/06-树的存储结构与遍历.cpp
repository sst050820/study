/*
 * ============================================================
 * 文件名：06-树的存储结构与遍历.cpp
 * 章  节：第五章 树与二叉树
 * 内  容：双亲表示法（顺序存储）+ 孩子兄弟表示法（二叉链表）+ 树先根/后根遍历
 * 重要度：★★★ 理解掌握（选择题考概念与优缺点，代码以了解为主）
 * 考研考点：三种存储结构的优缺点对比、并查集=双亲表示法、
 *          孩子兄弟表示法是"树转二叉树"的本质
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef int ElemType; // 结点数据类型

/*
 * ============================================================
 * 一、双亲表示法（顺序存储，★★★ 选择题常考）：
 *   - 数组每个元素存 data 和 parent（双亲在数组中的下标）
 *   - 根结点 parent = -1
 *   - 优点：找双亲 O(1)；缺点：找孩子要遍历整个数组 O(n)
 *   - 典型应用：并查集（只找双亲、不找孩子）
 * ============================================================
 */
#define MAX_TREE_SIZE 100

typedef struct {                    // 树的结点定义
    ElemType data;                  // 数据域
    int parent;                     // 双亲在数组中的下标（根为-1）
} PTNode;

typedef struct {                    // 树的类型定义
    PTNode nodes[MAX_TREE_SIZE];    // 顺序存储的结点数组
    int n;                          // 当前结点个数
} PTree;

// ======1. 初始化双亲表示法树（预置一棵树方便测试）======
// 树形：A(0) 的孩子 B(1)、C(2)；B 的孩子 D(3)；C 的孩子 E(4)
void InitPTree(PTree &T) {
    T.n = 0;
    // 依次插入：A B C D E
    T.nodes[0].data = 1;   T.nodes[0].parent = -1;  // A：根
    T.nodes[1].data = 2;   T.nodes[1].parent = 0;   // B：双亲是A
    T.nodes[2].data = 3;   T.nodes[2].parent = 0;   // C：双亲是A
    T.nodes[3].data = 4;   T.nodes[3].parent = 1;   // D：双亲是B
    T.nodes[4].data = 5;   T.nodes[4].parent = 2;   // E：双亲是C
    T.n = 5;
}

// ======2. 查找指定下标结点的双亲下标（O(1)，双亲表示法优点）======
int FindParent(PTree T, int i) {
    if(i < 0 || i >= T.n) {
        return -1;                  // 下标越界
    }
    return T.nodes[i].parent;       // ★ 直接查数组，O(1)
}

// ======3. 打印指定下标结点的所有孩子（需遍历全表，O(n)）======
void PrintChildren(PTree T, int i) {
    printf("       结点%d(%d)的孩子：", i, T.nodes[i].data);
    int cnt = 0;
    for(int j = 0; j < T.n; j++) {  // ★ 只能全表扫描
        if(T.nodes[j].parent == i) { // 谁的parent指向i，谁就是i的孩子
            printf("%d ", T.nodes[j].data);
            cnt++;
        }
    }
    if(cnt == 0) {
        printf("(无)");
    }
    printf("\n");
}

/*
 * ============================================================
 * 二、孩子兄弟表示法（纯链式，★★★ 最重要的一种）：
 *   - 左指针 firstChild  = 第一个孩子
 *   - 右指针 nextSibling = 下一个兄弟
 *   - 存储形态和二叉树一模一样（二叉链表）→ 树可转二叉树
 *   - 既能存树也能存森林（森林的树根之间用右指针串成"兄弟"）
 * ============================================================
 */
typedef struct CSNode {
    ElemType data;                          // 数据域
    struct CSNode *firstChild, *nextSibling; // 第一个孩子 / 右兄弟
} CSNode, *CSTree;

// ======4. 创建孩子兄弟结点（工具函数）======
CSNode *CreateCSNode(ElemType e) {
    CSNode *p = (CSNode *)malloc(sizeof(CSNode));
    p->data = e;
    p->firstChild = NULL;
    p->nextSibling = NULL;
    return p;
}

// ======5. 建一棵孩子兄弟表示法的树（测试用）======
// 树形：1有孩子2、3、4；2有孩子5、6
// 孩子兄弟存储：1.firstChild=2；2.nextSibling=3；3.nextSibling=4；
//               2.firstChild=5；5.nextSibling=6
CSTree BuildCSTree() {
    CSTree T = CreateCSNode(1);
    T->firstChild = CreateCSNode(2);
    T->firstChild->nextSibling = CreateCSNode(3);
    T->firstChild->nextSibling->nextSibling = CreateCSNode(4);
    T->firstChild->firstChild = CreateCSNode(5);
    T->firstChild->firstChild->nextSibling = CreateCSNode(6);
    return T;
}

/*
 * ======6. 树的先根遍历（★★★ 了解，对应二叉树先序）======
 * 先访问根，再依次先根遍历每个孩子子树
 * 注意：孩子要用 firstChild + nextSibling 这条链逐个取出
 */
void PreOrderTree(CSTree T) {
    if(T != NULL) {
        printf("%d ", T->data);             // ★ 先访问根
        for(CSNode *p = T->firstChild; p != NULL; p = p->nextSibling) {
            PreOrderTree(p);                // ★ 再依次先根遍历各孩子子树
        }
    }
}

/*
 * ======7. 树的后根遍历（★★★ 了解，对应二叉树中序）======
 * 先依次后根遍历每个孩子子树，最后访问根
 */
void PostOrderTree(CSTree T) {
    if(T != NULL) {
        for(CSNode *p = T->firstChild; p != NULL; p = p->nextSibling) {
            PostOrderTree(p);               // ★ 先依次后根遍历各孩子子树
        }
        printf("%d ", T->data);             // ★ 最后访问根
    }
}

// ======主函数：测试两种存储结构======
int main() {
    printf("====== 树的存储结构与遍历测试开始 ======\n\n");

    // 测试1：双亲表示法
    printf("【测试1】双亲表示法（找双亲O(1)，找孩子O(n)）\n");
    PTree T;
    InitPTree(T);
    printf("       结点2(B)的双亲下标 = %d（期望0，即A）\n", FindParent(T, 1));
    printf("       结点0(A)的双亲下标 = %d（期望-1，根）\n", FindParent(T, 0));
    PrintChildren(T, 0);                    // 期望孩子：2 3
    PrintChildren(T, 1);                    // 期望孩子：4
    PrintChildren(T, 4);                    // 期望无

    // 测试2：孩子兄弟表示法 + 遍历
    printf("\n【测试2】孩子兄弟表示法 + 树遍历\n");
    CSTree CS = BuildCSTree();
    printf("       先根遍历：");
    PreOrderTree(CS);
    printf("\n");
    printf("       期望先根：1 2 5 6 3 4\n");
    printf("       后根遍历：");
    PostOrderTree(CS);
    printf("\n");
    printf("       期望后根：5 6 2 3 4 1\n");

    /*
     * 【考点对应】孩子兄弟表示法下：
     *   树先根遍历序列 = 对应二叉树的先序遍历序列
     *   树后根遍历序列 = 对应二叉树的中序遍历序列
     * 原因：孩子兄弟树的"右指针=兄弟"，二叉树的中序恰好先左（孩子）后右（兄弟链）
     */

    printf("\n====== 树的存储结构与遍历测试结束 ======\n");
    return 0;
}
