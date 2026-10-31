/*
 * ============================================================
 * 文件名：03-二叉树的层次遍历.cpp
 * 章  节：第五章 树与二叉树
 * 内  容：层次遍历（广度优先遍历），用链队列存结点指针
 * 重要度：★★★★ 重点掌握（选择题/算法题常客，与图的BFS同源）
 * 考研考点：队列作用、入队的是指针、层次遍历序列
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef int ElemType; // 结点数据类型

/*
 * ============================================================
 * 二叉链表结构体（同01/02号文件）
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
 * 链队列（存结点指针，★★★ 复习第3章队列的链式实现）
 * 为什么用链队列：层次遍历要"先入先出"地处理结点，队列天然满足；
 * 为什么存指针：不复制结点本身，省空间、操作快
 * ============================================================
 */
typedef struct QNode {
    BiTNode *data;              // ★ 队列元素 = 结点指针
    struct QNode *next;
} QNode;

typedef struct {
    QNode *front;               // 队头指针
    QNode *rear;                // 队尾指针
} LinkQueue;

// ======2. 初始化队列（队头队尾都置空）======
void InitQueue(LinkQueue &Q) {
    Q.front = NULL;             // 带头结点的链队列也可加头结点，这里用不带头结点版
    Q.rear = NULL;
}

// ======3. 入队（从队尾插入）======
void EnQueue(LinkQueue &Q, BiTNode *p) {
    QNode *s = (QNode *)malloc(sizeof(QNode)); // 新队元素
    s->data = p;
    s->next = NULL;             // 新元素成为新的队尾
    if(Q.rear == NULL) {        // ★ 空队列特殊处理：队头队尾都指向它
        Q.front = s;
        Q.rear = s;
    } else {
        Q.rear->next = s;       // 挂到原队尾后面
        Q.rear = s;             // 更新队尾
    }
}

// ======4. 出队（从队头取出，带回p）======
bool DeQueue(LinkQueue &Q, BiTNode *&p) {
    if(Q.front == NULL) {       // 空队列不能出
        return false;
    }
    QNode *q = Q.front;         // 保存队头
    p = q->data;                // ★ 取出结点指针
    Q.front = q->next;          // 队头后移
    if(Q.front == NULL) {       // 队列变空，队尾也要置空
        Q.rear = NULL;
    }
    free(q);                    // 释放队元素内存
    return true;
}

// ======5. 判队空======
bool QueueEmpty(LinkQueue Q) {
    return Q.front == NULL;
}

/*
 * ======6. 层次遍历（★★★ 必背框架）======
 * 步骤：
 *   1. 根结点入队
 *   2. 循环：队列不空时——
 *       出队一个结点p，访问p
 *       若p有左孩子 → 左孩子入队
 *       若p有右孩子 → 右孩子入队
 * 效果：从上到下、从左到右一层层访问，就是"广度优先"
 */
void LevelOrder(BiTree T) {
    LinkQueue Q;                // 辅助队列
    InitQueue(Q);
    if(T != NULL) {
        EnQueue(Q, T);          // ★ 1.根入队
    }
    while(!QueueEmpty(Q)) {     // ★ 2.队不空就循环
        BiTNode *p;
        DeQueue(Q, p);          // 出队一个结点
        printf("%d ", p->data); // 访问它
        if(p->lchild != NULL) { // ★ 左孩子入队（同一层按从左到右）
            EnQueue(Q, p->lchild);
        }
        if(p->rchild != NULL) { // ★ 右孩子入队
            EnQueue(Q, p->rchild);
        }
    }
}

/*
 * ======7. 按层打印（进阶：每层一行，了解即可）======
 * 用变量记录"当前层剩余结点数"，当一层出完就换行
 */
void LevelOrderByLayer(BiTree T) {
    LinkQueue Q;
    InitQueue(Q);
    if(T != NULL) {
        EnQueue(Q, T);
    }
    int cur = 1;                // ★ 本层应有结点数（初始第1层只有根=1个）
    while(!QueueEmpty(Q)) {     // 注意：cur在外层循环外声明，不能每次重置
        int cnt = 0;            // 统计本层入队个数（=下一层应出个数）
        while(cur > 0) {        // ★ 把"当前层"全部出完再换行
            BiTNode *p;
            DeQueue(Q, p);
            printf("%d ", p->data);
            if(p->lchild != NULL) { EnQueue(Q, p->lchild); cnt++; }
            if(p->rchild != NULL) { EnQueue(Q, p->rchild); cnt++; }
            cur--;              // 本层还剩几个没出
        }
        printf("\n");           // 一层结束，换行
        cur = cnt;              // 下一层应出个数 = 本层入队个数
    }
}

// ======主函数：测试层次遍历======
int main() {
    // 建一棵树：1是根；2、3是1的孩子；4、5、6、7是最底层（完全二叉树）
    BiTree T = CreateNode(1);
    T->lchild = CreateNode(2);
    T->rchild = CreateNode(3);
    T->lchild->lchild = CreateNode(4);
    T->lchild->rchild = CreateNode(5);
    T->rchild->lchild = CreateNode(6);
    T->rchild->rchild = CreateNode(7);

    printf("====== 二叉树层次遍历测试开始 ======\n\n");

    printf("【测试1】层次遍历（一行输出）\n");
    printf("       期望：1 2 3 4 5 6 7\n");
    printf("       实际：");
    LevelOrder(T);
    printf("\n");

    printf("\n【测试2】按层分行打印（进阶）\n");
    printf("       期望：\n");
    printf("          1\n");
    printf("          2 3\n");
    printf("          4 5 6 7\n");
    printf("       实际：\n");
    LevelOrderByLayer(T);

    printf("\n【测试3】边界：空树 / 只有根\n");
    BiTree Empty = NULL;
    LevelOrder(Empty);          // 什么都不输出
    printf("       空树遍历完成（无输出）\n");
    BiTree Single = CreateNode(9);
    printf("       单结点树：");
    LevelOrder(Single);
    printf("\n");

    printf("\n====== 二叉树层次遍历测试结束 ======\n");
    return 0;
}
