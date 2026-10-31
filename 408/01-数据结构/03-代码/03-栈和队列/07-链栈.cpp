/*
 * ============================================================
 * 文件名：07-链栈.cpp
 * 章  节：第三章 栈、队列和数组
 * 内  容：栈的链式存储实现（表头作为栈顶，不带头结点）
 * 重要度：★★☆☆☆ 了解即可（考得少，顺序栈才是重点）
 * 考研考点：表头作栈顶、入栈出栈都在表头、O(1)时间复杂度
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef int ElemType;  // ★★☆☆☆ 栈中元素的类型

// ======链栈的节点结构体定义======
typedef struct Linknode {  // ★★☆☆☆ 链栈的节点类型
    ElemType data;  // 数据域：存储栈中的元素
    struct Linknode *next;  // 指针域：指向下一个节点
} Linknode, *LiStack;  // ★★☆☆☆ Linknode是节点类型，LiStack是指向节点的指针类型（即链栈类型）

/*
 * ============================================================
 * 链栈的核心概念（★★☆☆☆ 了解即可）：
 *
 * 1. 表头作为栈顶：
 *    - 链栈通常用单链表实现，规定所有操作都在表头进行
 *    - 表头节点就是栈顶元素
 *    - 栈顶指针 S 就是链表的头指针
 *
 * 2. 不带头结点：
 *    - 链栈一般不需要头结点
 *    - 因为栈的操作只在栈顶（表头）进行，带头结点反而多一步
 *    - 空栈时 S = NULL
 *
 * 3. 入栈（头插法）：
 *    - 新建节点s，赋值
 *    - s->next = S（新节点指向原栈顶）
 *    - S = s（栈顶指针指向新节点）
 *
 * 4. 出栈（删除表头节点）：
 *    - p = S（p指向栈顶节点）
 *    - x = p->data（取出栈顶元素）
 *    - S = p->next（栈顶指针指向下一个节点）
 *    - free(p)（释放原栈顶节点）
 *
 * 时间复杂度：入栈出栈都是O(1)，因为都在表头操作
 * ============================================================
 */

// ======1. 初始化链栈======
void InitStack(LiStack &S) {
    S = NULL;  // ★★☆☆☆ 栈顶指针初始化为NULL，表示空栈（不带头结点）
}

// ======2. 判断链栈是否为空======
bool StackEmpty(LiStack S) {
    return S == NULL;  // ★★☆☆☆ 栈顶指针为NULL表示空栈
}

// ======3. 入栈（头插法，将元素x压入栈顶）======
bool Push(LiStack &S, ElemType x) {
    Linknode *s = (Linknode *)malloc(sizeof(Linknode));  // ★★☆☆☆ 为新节点分配内存空间
    if(s == NULL)  // 内存分配失败
        return false;
    s->data = x;  // ★★☆☆☆ 新节点的数据域赋值为x
    s->next = S;  // ★★☆☆☆ 新节点的指针域指向原来的栈顶节点
    S = s;  // ★★☆☆☆ 栈顶指针指向新节点，新节点成为新的栈顶
    return true;  // 入栈成功
}

// ======4. 出栈（弹出栈顶元素，用x返回）======
bool Pop(LiStack &S, ElemType &x) {
    if(S == NULL)  // ★★☆☆☆ 栈空，没有元素可以出栈
        return false;
    Linknode *p = S;  // ★★☆☆☆ p指向栈顶节点
    x = p->data;  // ★★☆☆☆ 将栈顶元素的数据域赋值给x
    S = p->next;  // ★★☆☆☆ 栈顶指针指向下一个节点
    free(p);  // ★★☆☆☆ 释放原栈顶节点的内存空间
    return true;  // 出栈成功
}

// ======5. 读取栈顶元素（不弹出，用x返回）======
bool GetTop(LiStack S, ElemType &x) {
    if(S == NULL)  // ★★☆☆☆ 栈空，没有栈顶元素
        return false;
    x = S->data;  // ★★☆☆☆ 栈顶元素就是S指向的节点的数据域
    return true;  // 读取成功
}

// ======6. 求栈的长度======
int StackLength(LiStack S) {
    int len = 0;  // 定义计数器len，初始为0
    Linknode *p = S;  // p从栈顶开始
    while(p != NULL) {  // 遍历栈
        len++;  // 计数器加1
        p = p->next;  // p指向下一个节点
    }
    return len;  // 返回栈的长度
}

// ======7. 打印栈中所有元素（从栈底到栈顶）======
void PrintStack(LiStack S) {
    if(S == NULL) {  // 栈空
        printf("栈为空\n");
        return;
    }
    // 先遍历到栈底，再反向打印（或者用递归逆序打印）
    // 这里用数组暂存，然后逆序输出
    ElemType temp[100];  // 临时数组，暂存元素
    int count = 0;  // 元素个数计数器
    Linknode *p = S;  // 从栈顶开始
    while(p != NULL) {
        temp[count++] = p->data;  // 暂存元素
        p = p->next;  // 下一个节点
    }
    printf("栈中元素（栈底→栈顶）：");
    for(int i = count - 1; i >= 0; i--) {  // 逆序输出，即栈底到栈顶
        printf("%d ", temp[i]);
    }
    printf("\n");
}

// ======8. 销毁链栈（释放所有节点内存）======
void DestroyStack(LiStack &S) {
    Linknode *p;  // 定义指针p，用于遍历释放
    while(S != NULL) {  // 从栈顶开始依次释放
        p = S;  // p指向当前节点
        S = S->next;  // S指向下一个节点
        free(p);  // 释放当前节点
    }
}

// ======主函数：测试链栈的所有操作======
int main() {
    LiStack S;  // 定义一个链栈S
    InitStack(S);  // 初始化链栈S

    printf("====== 链栈测试开始 ======\n\n");

    // 测试1：判断空栈
    printf("【测试1】初始栈是否为空：%s\n", StackEmpty(S) ? "是" : "否");
    printf("       初始栈长度：%d\n", StackLength(S));

    // 测试2：依次入栈 10, 20, 30, 40, 50
    printf("\n【测试2】依次入栈：10, 20, 30, 40, 50\n");
    for(int i = 1; i <= 5; i++) {
        Push(S, i * 10);  // 将 i*10 压入栈
    }
    PrintStack(S);  // 打印栈中元素
    printf("       当前栈长度：%d\n", StackLength(S));

    // 测试3：读取栈顶元素
    int topVal;  // 定义变量topVal，用于存储栈顶元素
    GetTop(S, topVal);  // 读取栈顶元素
    printf("\n【测试3】当前栈顶元素：%d（应为50，最后入栈的在栈顶）\n", topVal);

    // 测试4：出栈两次
    int popVal;  // 定义变量popVal，用于存储弹出的元素
    printf("\n【测试4】执行两次出栈：\n");
    for(int i = 0; i < 2; i++) {
        if(Pop(S, popVal)) {  // 出栈成功
            printf("       第%d次出栈元素：%d\n", i + 1, popVal);
        }
    }
    PrintStack(S);  // 打印出栈后的栈
    printf("       当前栈长度：%d\n", StackLength(S));

    // 测试5：继续入栈
    printf("\n【测试5】继续入栈：60, 70\n");
    Push(S, 60);
    Push(S, 70);
    PrintStack(S);
    printf("       当前栈长度：%d\n", StackLength(S));
    GetTop(S, topVal);
    printf("       当前栈顶元素：%d\n", topVal);

    // 测试6：全部出栈
    printf("\n【测试6】全部出栈：\n");
    while(!StackEmpty(S)) {  // 当栈不为空时循环出栈
        Pop(S, popVal);  // 出栈
        printf("%d ", popVal);  // 打印弹出的元素
    }
    printf("\n");
    printf("       出栈完毕，栈是否为空：%s\n", StackEmpty(S) ? "是" : "否");

    // 测试7：空栈时尝试出栈（应该失败）
    printf("\n【测试7】空栈时尝试出栈：");
    if(Pop(S, popVal)) {
        printf("成功（异常！）\n");
    } else {
        printf("失败（栈空，正确）\n");
    }

    // 销毁栈
    DestroyStack(S);
    printf("\n====== 链栈测试结束 ======\n");
    return 0;  // 程序正常结束
}
