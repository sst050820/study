/*
 * ============================================================
 * 文件名：09-双端队列.cpp
 * 章  节：第三章 栈、队列和数组
 * 内  容：双端队列（一般双端、输入受限、输出受限）
 * 重要度：★★☆☆☆ 了解即可（只考选择题序列合法性，不需要完整代码）
 * 考研考点：两端都可入可出、输入/输出受限变体、输出序列合法性判断
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MaxSize 50  // ★★☆☆☆ 双端队列的最大容量

typedef int ElemType;  // ★★☆☆☆ 队列中元素的类型

// ======双端队列的结构体定义（循环数组实现）======
typedef struct {
    ElemType data[MaxSize];  // ★★☆☆☆ 静态数组，存放队列中的元素
    int front;  // ★★☆☆☆ 前端指针，指向前端元素
    int rear;   // ★★☆☆☆ 后端指针，指向后端元素的下一个位置
} Deque;  // 双端队列的结构体类型名

/*
 * ============================================================
 * 双端队列的核心概念（★★☆☆☆ 选择题考点）：
 *
 * 1. 一般双端队列（Deque）：
 *    - 前端（front）和后端（rear）都可以入队和出队
 *    - 前端入队：在front端插入
 *    - 前端出队：在front端删除
 *    - 后端入队：在rear端插入
 *    - 后端出队：在rear端删除
 *
 * 2. 输入受限的双端队列：
 *    - 只允许一端进行入队操作，另一端可以入队和出队
 *    - 例如：只允许后端入队，前端可以入队和出队
 *
 * 3. 输出受限的双端队列：
 *    - 只允许一端进行出队操作，另一端可以入队和出队
 *    - 例如：只允许前端出队，后端可以入队和出队
 *
 * 4. 特殊情况：
 *    - 栈 = 输入和输出都受限在同一端的双端队列
 *    - 普通队列 = 一端只能入、另一端只能出的双端队列
 *
 * ★★☆☆☆ 考研只考选择题：给定输入序列，判断哪些输出序列合法
 * 解题方法：按照双端队列的操作规则，模拟入队出队过程
 * ============================================================
 */

// ======1. 初始化双端队列======
void InitDeque(Deque &D) {
    D.front = D.rear = 0;  // 前端和后端指针都初始化为0，表示空队列
}

// ======2. 判断双端队列是否为空======
bool DequeEmpty(Deque D) {
    return D.front == D.rear;  // 前端指针等于后端指针表示空队列
}

// ======3. 判断双端队列是否为满（牺牲一个单元）======
bool DequeFull(Deque D) {
    return (D.rear + 1) % MaxSize == D.front;  // 牺牲一个单元的判满条件
}

// ======4. 后端入队（在rear端插入元素x）======
bool EnQueueRear(Deque &D, ElemType x) {
    if((D.rear + 1) % MaxSize == D.front)  // 队列已满
        return false;
    D.data[D.rear] = x;  // 将元素x放入后端位置
    D.rear = (D.rear + 1) % MaxSize;  // 后端指针循环后移
    return true;
}

// ======5. 前端入队（在front端插入元素x）======
bool EnQueueFront(Deque &D, ElemType x) {
    if((D.rear + 1) % MaxSize == D.front)  // 队列已满
        return false;
    D.front = (D.front - 1 + MaxSize) % MaxSize;  // ★★☆☆☆ 前端指针循环前移（先移动指针）
    D.data[D.front] = x;  // 将元素x放入前端位置
    return true;
}

// ======6. 前端出队（在front端删除元素，用x返回）======
bool DeQueueFront(Deque &D, ElemType &x) {
    if(D.front == D.rear)  // 队列已空
        return false;
    x = D.data[D.front];  // 取出前端元素
    D.front = (D.front + 1) % MaxSize;  // 前端指针循环后移
    return true;
}

// ======7. 后端出队（在rear端删除元素，用x返回）======
bool DeQueueRear(Deque &D, ElemType &x) {
    if(D.front == D.rear)  // 队列已空
        return false;
    D.rear = (D.rear - 1 + MaxSize) % MaxSize;  // ★★☆☆☆ 后端指针循环前移（先移动指针）
    x = D.data[D.rear];  // 取出后端元素
    return true;
}

// ======8. 读取前端元素（不删除）======
bool GetFront(Deque D, ElemType &x) {
    if(D.front == D.rear)  // 队列已空
        return false;
    x = D.data[D.front];  // 前端元素
    return true;
}

// ======9. 读取后端元素（不删除）======
bool GetRear(Deque D, ElemType &x) {
    if(D.front == D.rear)  // 队列已空
        return false;
    x = D.data[(D.rear - 1 + MaxSize) % MaxSize];  // 后端元素（rear的前一个位置）
    return true;
}

// ======10. 求队列长度======
int DequeLength(Deque D) {
    return (D.rear - D.front + MaxSize) % MaxSize;  // 长度公式
}

// ======11. 打印双端队列（从前端到后端）======
void PrintDeque(Deque D) {
    if(D.front == D.rear) {
        printf("队列为空\n");
        return;
    }
    printf("队列（前端→后端）：");
    int i = D.front;
    while(i != D.rear) {
        printf("%d ", D.data[i]);
        i = (i + 1) % MaxSize;
    }
    printf("\n");
}

/*
 * ============================================================
 * 输出序列合法性验证函数（★★☆☆☆ 选择题解题方法）
 *
 * 给定输入序列（按顺序入队），判断目标输出序列是否合法。
 * 模拟方法：按顺序将输入元素入队，在入队过程中可以从允许的一端出队，
 * 看能否得到目标输出序列。
 * ============================================================
 */

// ======12. 验证一般双端队列的输出序列是否合法======
bool CheckDequeOutput(int *input, int *output, int n) {
    Deque D;
    InitDeque(D);
    int inIdx = 0;  // 输入序列的当前下标
    int outIdx = 0;  // 输出序列的当前下标

    while(outIdx < n) {
        // 如果队列为空或前端/后端都不等于目标输出，则继续入队
        if(DequeEmpty(D) ||
           (D.data[D.front] != output[outIdx] &&
            D.data[(D.rear - 1 + MaxSize) % MaxSize] != output[outIdx])) {
            if(inIdx >= n) break;  // 输入已用完，无法继续
            EnQueueRear(D, input[inIdx++]);  // 从后端入队
        } else if(D.data[D.front] == output[outIdx]) {
            // 前端等于目标输出，从前端出队
            int x;
            DeQueueFront(D, x);
            outIdx++;
        } else {
            // 后端等于目标输出，从后端出队
            int x;
            DeQueueRear(D, x);
            outIdx++;
        }
    }
    return outIdx == n;  // 所有输出都匹配成功则合法
}

// ======主函数：测试双端队列======
int main() {
    Deque D;
    InitDeque(D);

    printf("====== 双端队列测试开始 ======\n\n");

    // 测试1：后端入队 10, 20, 30
    printf("【测试1】后端入队：10, 20, 30\n");
    EnQueueRear(D, 10);
    EnQueueRear(D, 20);
    EnQueueRear(D, 30);
    PrintDeque(D);
    printf("  前端=%d，后端=%d，长度=%d\n", D.front, D.rear, DequeLength(D));

    // 测试2：前端入队 5, 1
    printf("\n【测试2】前端入队：5, 1\n");
    EnQueueFront(D, 5);
    EnQueueFront(D, 1);
    PrintDeque(D);
    printf("  前端=%d，后端=%d，长度=%d\n", D.front, D.rear, DequeLength(D));

    // 测试3：读取前后端元素
    int frontVal, rearVal;
    GetFront(D, frontVal);
    GetRear(D, rearVal);
    printf("\n【测试3】前端元素：%d，后端元素：%d\n", frontVal, rearVal);

    // 测试4：前端出队两次
    int x;
    printf("\n【测试4】前端出队两次：\n");
    for(int i = 0; i < 2; i++) {
        if(DeQueueFront(D, x)) printf("  第%d次前端出队：%d\n", i + 1, x);
    }
    PrintDeque(D);

    // 测试5：后端出队两次
    printf("\n【测试5】后端出队两次：\n");
    for(int i = 0; i < 2; i++) {
        if(DeQueueRear(D, x)) printf("  第%d次后端出队：%d\n", i + 1, x);
    }
    PrintDeque(D);

    // 清空队列
    while(!DequeEmpty(D)) DeQueueFront(D, x);

    // 测试6：★★☆☆☆ 输出序列合法性验证
    printf("\n【测试6】输出序列合法性验证（输入序列：1, 2, 3, 4）\n");
    int input[] = {1, 2, 3, 4};

    int output1[] = {4, 3, 2, 1};  // 全部从后端出（类似栈）
    printf("  输出序列 4,3,2,1 是否合法：%s\n", CheckDequeOutput(input, output1, 4) ? "是" : "否");

    int output2[] = {1, 2, 3, 4};  // 全部从前端出（类似普通队列）
    printf("  输出序列 1,2,3,4 是否合法：%s\n", CheckDequeOutput(input, output2, 4) ? "是" : "否");

    int output3[] = {2, 1, 4, 3};  // 混合出队
    printf("  输出序列 2,1,4,3 是否合法：%s\n", CheckDequeOutput(input, output3, 4) ? "是" : "否");

    int output4[] = {3, 1, 2, 4};  // 这个应该不合法
    printf("  输出序列 3,1,2,4 是否合法：%s\n", CheckDequeOutput(input, output4, 4) ? "是" : "否");

    printf("\n====== 双端队列测试结束 ======\n");
    printf("\n★ 考研重点提醒：\n");
    printf("  1. 双端队列只考选择题，不考算法题\n");
    printf("  2. 解题方法：模拟入队出队过程，看能否得到目标序列\n");
    printf("  3. 记住：栈=输入输出都受限在同一端，普通队列=一端入一端出\n");
    printf("  4. 输入受限/输出受限的变体，按照题目给定的限制条件模拟即可\n");
    return 0;
}
