/*
 * ============================================================
 * 文件名：06-链队列.cpp
 * 章  节：第三章 栈、队列和数组
 * 内  容：队列的链式存储实现（带头结点）
 * 重要度：★★★☆☆ 理解即可（选择题考点，算法题考得少）
 * 考研考点：带头结点、入队在队尾、出队在队头、删最后元素改rear
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef int ElemType;  // ★★★☆☆ 队列中元素的类型

// ======链队列的节点结构体定义======
typedef struct LinkNode {  // ★★★☆☆ 链队列的节点类型
    ElemType data;  // 数据域：存储队列中的元素
    struct LinkNode *next;  // 指针域：指向下一个节点
} LinkNode;  // 节点类型名

// ======链队列的结构体定义（包含队头和队尾指针）======
typedef struct {  // ★★★☆☆ 链队列的结构体类型
    LinkNode *front;  // ★★★☆☆ 队头指针，指向头结点（头结点的next是队头元素）
    LinkNode *rear;   // ★★★☆☆ 队尾指针，指向队尾元素节点
} LinkQueue;  // 链队列的结构体类型名

/*
 * ============================================================
 * 链队列的核心概念（★★★☆☆ 必须理解）：
 *
 * 1. 带头结点：
 *    - 空队列时，front和rear都指向头结点
 *    - 头结点不存储数据，只是为了简化操作
 *    - 队头元素是 front->next 指向的节点
 *
 * 2. 入队（在队尾插入）：
 *    - 新建节点，赋值
 *    - rear->next = 新节点（原队尾节点指向新节点）
 *    - rear = 新节点（队尾指针指向新节点）
 *
 * 3. 出队（删除队头元素）：
 *    - p = front->next（p指向队头元素节点）
 *    - front->next = p->next（头结点指向队头的下一个节点）
 *    - ★★★★☆ 如果删除的是最后一个元素，必须令 rear = front
 *      （否则rear成为悬空指针，后续入队会出错！）
 *    - free(p)（释放原队头节点）
 *
 * ★★★★☆ 考研高频考点：删除最后一个元素后必须修改rear指针！
 * ============================================================
 */

// ======1. 初始化链队列（带头结点）======
void InitQueue(LinkQueue &Q) {
    Q.front = Q.rear = (LinkNode *)malloc(sizeof(LinkNode));  // ★★★☆☆ 建立头结点，队头队尾都指向头结点
    Q.front->next = NULL;  // ★★★☆☆ 头结点的指针域初始化为NULL，表示空队列
}

// ======2. 判断链队列是否为空======
bool QueueEmpty(LinkQueue Q) {
    return Q.front == Q.rear;  // ★★★☆☆ 队头指针等于队尾指针（都指向头结点）表示空队列
}

// ======3. 入队（在队尾插入元素x）======
void EnQueue(LinkQueue &Q, ElemType x) {
    LinkNode *s = (LinkNode *)malloc(sizeof(LinkNode));  // ★★★☆☆ 为新节点分配内存空间
    s->data = x;  // ★★★☆☆ 新节点的数据域赋值为x
    s->next = NULL;  // ★★★☆☆ 新节点的指针域初始化为NULL（作为新的队尾节点）
    Q.rear->next = s;  // ★★★☆☆ 原队尾节点的指针域指向新节点
    Q.rear = s;  // ★★★☆☆ 队尾指针指向新节点，新节点成为新的队尾
}

// ======4. 出队（删除队头元素，用x返回）======
bool DeQueue(LinkQueue &Q, ElemType &x) {
    if(Q.front == Q.rear)  // ★★★☆☆ 队列已空，不能出队
        return false;
    LinkNode *p = Q.front->next;  // ★★★☆☆ p指向队头元素节点（头结点的下一个节点）
    x = p->data;  // ★★★☆☆ 将队头元素的数据域赋值给x
    Q.front->next = p->next;  // ★★★☆☆ 头结点的指针域指向队头元素的下一个节点
    if(Q.rear == p) {  // ★★★★☆ 如果删除的是最后一个元素
        Q.rear = Q.front;  // ★★★★☆ 队尾指针指向头结点，避免尾指针悬空！
    }
    free(p);  // ★★★☆☆ 释放原队头元素节点的内存空间
    return true;  // 出队成功
}

// ======5. 读取队头元素（不删除，用x返回）======
bool GetHead(LinkQueue Q, ElemType &x) {
    if(Q.front == Q.rear)  // ★★★☆☆ 队列已空，没有队头元素
        return false;
    x = Q.front->next->data;  // ★★★☆☆ 队头元素是头结点的下一个节点的数据域
    return true;  // 读取成功
}

// ======6. 求队列长度======
int QueueLength(LinkQueue Q) {
    int len = 0;  // 定义计数器len，初始为0
    LinkNode *p = Q.front->next;  // p指向队头元素节点
    while(p != NULL) {  // 遍历队列
        len++;  // 计数器加1
        p = p->next;  // p指向下一个节点
    }
    return len;  // 返回队列长度
}

// ======7. 打印队列中所有元素（从队头到队尾）======
void PrintQueue(LinkQueue Q) {
    if(Q.front == Q.rear) {  // 队列空
        printf("队列为空\n");
        return;
    }
    printf("队列元素（队头→队尾）：");
    LinkNode *p = Q.front->next;  // 从队头元素开始
    while(p != NULL) {  // 遍历队列
        printf("%d ", p->data);  // 打印当前节点的数据
        p = p->next;  // 指向下一个节点
    }
    printf("\n");
}

// ======8. 销毁链队列（释放所有节点内存）======
void DestroyQueue(LinkQueue &Q) {
    LinkNode *p;  // 定义指针p，用于遍历释放
    while(Q.front != NULL) {  // 从头结点开始依次释放
        p = Q.front;  // p指向当前节点
        Q.front = Q.front->next;  // front指向下一个节点
        free(p);  // 释放当前节点
    }
    Q.rear = NULL;  // 队尾指针置空
}

// ======主函数：测试链队列的所有操作======
int main() {
    LinkQueue Q;  // 定义一个链队列Q
    InitQueue(Q);  // 初始化链队列Q

    printf("====== 链队列测试开始 ======\n\n");

    // 测试1：判断空队列
    printf("【测试1】初始队列是否为空：%s\n", QueueEmpty(Q) ? "是" : "否");
    printf("       初始队列长度：%d\n", QueueLength(Q));

    // 测试2：依次入队 10, 20, 30, 40, 50
    printf("\n【测试2】依次入队：10, 20, 30, 40, 50\n");
    for(int i = 1; i <= 5; i++) {
        EnQueue(Q, i * 10);  // 将 i*10 入队
    }
    PrintQueue(Q);  // 打印队列
    printf("       当前队列长度：%d\n", QueueLength(Q));

    // 测试3：读取队头元素
    int headVal;  // 定义变量headVal，用于存储队头元素
    GetHead(Q, headVal);  // 读取队头元素
    printf("\n【测试3】当前队头元素：%d\n", headVal);

    // 测试4：出队两次
    int deqVal;  // 定义变量deqVal，用于存储出队元素
    printf("\n【测试4】执行两次出队：\n");
    for(int i = 0; i < 2; i++) {
        if(DeQueue(Q, deqVal)) {  // 出队成功
            printf("       第%d次出队元素：%d\n", i + 1, deqVal);
        }
    }
    PrintQueue(Q);  // 打印出队后的队列
    printf("       当前队列长度：%d\n", QueueLength(Q));

    // 测试5：继续入队
    printf("\n【测试5】继续入队：60, 70\n");
    EnQueue(Q, 60);
    EnQueue(Q, 70);
    PrintQueue(Q);
    printf("       当前队列长度：%d\n", QueueLength(Q));

    // 测试6：★★★★☆ 重点测试——删除到最后一个元素，验证rear指针是否正确修改
    printf("\n【测试6】重点测试：删除所有元素，验证删最后一个时rear是否正确修改\n");
    printf("       当前队列：");
    PrintQueue(Q);
    printf("       开始逐个出队：\n");
    int count = 1;
    while(!QueueEmpty(Q)) {
        DeQueue(Q, deqVal);
        printf("       第%d次出队：%d，出队后队列是否为空：%s\n",
               count++, deqVal, QueueEmpty(Q) ? "是" : "否");
    }
    printf("       全部出队完毕，front和rear是否都指向头结点：%s\n",
           (Q.front == Q.rear) ? "是" : "否");

    // 测试7：★★★★☆ 重点测试——空队列后再入队，验证rear指针是否正常工作
    printf("\n【测试7】重点测试：空队列后重新入队100, 200（验证rear未悬空）\n");
    EnQueue(Q, 100);
    EnQueue(Q, 200);
    PrintQueue(Q);
    printf("       队头元素：");
    GetHead(Q, headVal);
    printf("%d\n", headVal);

    // 测试8：空队列时尝试出队（应该失败）
    printf("\n【测试8】先清空队列，再尝试出队：\n");
    while(!QueueEmpty(Q)) DeQueue(Q, deqVal);
    if(DeQueue(Q, deqVal)) {
        printf("       成功（异常！）\n");
    } else {
        printf("       失败（队空，正确）\n");
    }

    // 销毁队列
    DestroyQueue(Q);
    printf("\n====== 链队列测试结束 ======\n");
    return 0;  // 程序正常结束
}
