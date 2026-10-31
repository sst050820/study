/*
 * ============================================================
 * 文件名：02-循环队列.cpp
 * 章  节：第三章 栈、队列和数组
 * 内  容：循环队列的基本操作（牺牲一个存储单元判满）
 * 重要度：★★★★★ 必须熟练掌握（本章核心，选择题+算法题都可能考）
 * 考研考点：判空判满条件、长度公式、取模运算、假溢出问题
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MaxSize 50  // ★★★★★ 定义队列中元素的最大个数

typedef int ElemType;  // ★★★★★ 队列中元素的类型

// ======循环队列的结构体定义======
typedef struct {
    ElemType data[MaxSize];  // ★★★★★ 静态数组，存放队列中的元素
    int front;  // ★★★★★ 队头指针，指向队头元素（当前第一个元素的下标）
    int rear;  // ★★★★★ 队尾指针，指向队尾元素的下一个位置（下一个可插入位置的下标）
} SqQueue;  // 循环队列的结构体类型名

/*
 * ============================================================
 * 关于循环队列的核心概念（★★★★★ 考研必考点）：
 *
 * 1. 假溢出问题：
 *    普通顺序队列中，front和rear都不断向后移动，当rear到达
 *    数组末尾时，即使数组前面还有空闲空间，也无法再入队，
 *    这就是"假溢出"。
 *
 * 2. 循环队列的解决方法：
 *    通过取模运算（% MaxSize），让front和rear在数组中循环移动，
 *    把存储队列元素的表从逻辑上看成一个环。
 *
 * 3. 三种判空判满方案：
 *    方案一（王道默认，本文件采用）：牺牲一个存储单元
 *      - 空队列：front == rear
 *      - 满队列：(rear + 1) % MaxSize == front
 *      - 最多存储 MaxSize - 1 个元素
 *      - 队列长度：(rear - front + MaxSize) % MaxSize
 *
 *    方案二：增加size计数器
 *      - 空队列：size == 0
 *      - 满队列：size == MaxSize
 *      - 可以使用全部MaxSize个单元
 *
 *    方案三：增加tag标志位
 *      - tag=0表示最近一次是出队，tag=1表示最近一次是入队
 *      - 当front==rear时，tag==0为空，tag==1为满
 *
 * ★★★★★ 考试时必须先确认题目用的是哪种方案！
 * ============================================================
 */

// ======1. 初始化循环队列======
void InitQueue(SqQueue &Q) {
    Q.front = Q.rear = 0;  // ★★★★★ 队头指针和队尾指针都初始化为0，表示空队列
}

// ======2. 判断循环队列是否为空======
bool QueueEmpty(SqQueue Q) {
    return Q.front == Q.rear;  // ★★★★★ 队头指针等于队尾指针表示空队列
}

// ======3. 判断循环队列是否为满======
bool QueueFull(SqQueue Q) {
    return (Q.rear + 1) % MaxSize == Q.front;  // ★★★★★ 牺牲一个单元的判满条件
}

// ======4. 入队（将元素x加入队尾）======
bool EnQueue(SqQueue &Q, ElemType x) {
    if((Q.rear + 1) % MaxSize == Q.front)  // ★★★★★ 队列已满，不能入队
        return false;
    Q.data[Q.rear] = x;  // ★★★★★ 将元素x放入队尾位置（rear指向的是下一个可插入位置）
    Q.rear = (Q.rear + 1) % MaxSize;  // ★★★★★ 队尾指针循环后移一位，取模实现循环
    return true;  // 入队成功
}

// ======5. 出队（删除队头元素，用x返回）======
bool DeQueue(SqQueue &Q, ElemType &x) {
    if(Q.front == Q.rear)  // ★★★★★ 队列已空，不能出队
        return false;
    x = Q.data[Q.front];  // ★★★★★ 将队头元素赋值给x（front指向队头元素）
    Q.front = (Q.front + 1) % MaxSize;  // ★★★★★ 队头指针循环后移一位，取模实现循环
    return true;  // 出队成功
}

// ======6. 读取队头元素（不删除，用x返回）======
bool GetHead(SqQueue Q, ElemType &x) {
    if(Q.front == Q.rear)  // ★★★★★ 队列已空，没有队头元素
        return false;
    x = Q.data[Q.front];  // ★★★★★ 将队头元素赋值给x，不修改front指针
    return true;  // 读取成功
}

// ======7. 求队列长度（队列中元素个数）======
int QueueLength(SqQueue Q) {
    // ★★★★★ 长度公式：(rear - front + MaxSize) % MaxSize
    // 加MaxSize是为了防止rear < front时出现负数
    return (Q.rear - Q.front + MaxSize) % MaxSize;
}

// ======8. 打印队列中所有元素（从队头到队尾）======
void PrintQueue(SqQueue Q) {
    if(Q.front == Q.rear) {  // 队列空，打印提示
        printf("队列为空\n");
        return;
    }
    printf("队列元素（队头→队尾）：");
    int i = Q.front;  // 从队头开始
    while(i != Q.rear) {  // 循环直到队尾
        printf("%d ", Q.data[i]);  // 打印当前元素
        i = (i + 1) % MaxSize;  // 循环后移
    }
    printf("\n");
}

// ======主函数：测试循环队列的所有操作======
int main() {
    SqQueue Q;  // 定义一个循环队列Q
    InitQueue(Q);  // 初始化循环队列Q

    printf("====== 循环队列测试开始 ======\n\n");

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
    printf("       front=%d, rear=%d\n", Q.front, Q.rear);

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
    printf("       front=%d, rear=%d\n", Q.front, Q.rear);

    // 测试5：继续入队，观察循环效果
    printf("\n【测试5】继续入队：60, 70, 80\n");
    for(int i = 6; i <= 8; i++) {
        EnQueue(Q, i * 10);  // 将 i*10 入队
    }
    PrintQueue(Q);  // 打印队列
    printf("       当前队列长度：%d\n", QueueLength(Q));
    printf("       front=%d, rear=%d（观察rear是否循环回到数组开头）\n", Q.front, Q.rear);

    // 测试6：验证牺牲一个单元——最多存MaxSize-1个元素
    printf("\n【测试6】验证牺牲一个单元（MaxSize=%d，最多存%d个元素）：\n", MaxSize, MaxSize - 1);
    printf("       当前已有%d个元素，继续入队直到满：\n", QueueLength(Q));
    int count = 0;  // 计数器，记录成功入队的个数
    while(EnQueue(Q, 999)) {  // 不断入队直到失败
        count++;
    }
    printf("       成功入队%d个，队列已满\n", count);
    printf("       队列是否已满：%s\n", QueueFull(Q) ? "是" : "否");
    printf("       最终队列长度：%d（应为%d）\n", QueueLength(Q), MaxSize - 1);
    printf("       front=%d, rear=%d\n", Q.front, Q.rear);

    // 测试7：队满时尝试入队（应该失败）
    printf("\n【测试7】队满时尝试入队888：");
    if(EnQueue(Q, 888)) {
        printf("成功（异常！）\n");
    } else {
        printf("失败（队满，正确）\n");
    }

    // 测试8：全部出队
    printf("\n【测试8】全部出队：\n");
    while(!QueueEmpty(Q)) {  // 当队列不为空时循环出队
        DeQueue(Q, deqVal);  // 出队
        printf("%d ", deqVal);  // 打印出队元素
    }
    printf("\n");
    printf("       出队完毕，队列是否为空：%s\n", QueueEmpty(Q) ? "是" : "否");

    // 测试9：空队列时尝试出队（应该失败）
    printf("\n【测试9】空队列时尝试出队：");
    if(DeQueue(Q, deqVal)) {
        printf("成功（异常！）\n");
    } else {
        printf("失败（队空，正确）\n");
    }

    printf("\n====== 循环队列测试结束 ======\n");
    return 0;  // 程序正常结束
}
