/*
 * ============================================================
 * 文件名：01-顺序栈.cpp
 * 章  节：第三章 栈、队列和数组
 * 内  容：顺序栈的基本操作（top=-1 约定）
 * 重要度：★★★★★ 必须熟练掌握（栈的基础，所有栈应用的前提）
 * 考研考点：两种top约定的区别、入栈出栈代码、栈满栈空条件
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MaxSize 50  // ★★★★★ 定义栈中元素的最大个数，可根据需要修改

typedef int ElemType;  // ★★★★★ 栈中元素的类型，这里用int，考研中常用

// ======顺序栈的结构体定义======
typedef struct {
    ElemType data[MaxSize];  // ★★★★★ 静态数组，存放栈中的元素
    int top;  // ★★★★★ 栈顶指针，记录栈顶元素在数组中的下标
} SqStack;  // 顺序栈的结构体类型名

/*
 * ============================================================
 * 关于 top 指针的两种约定（★★★★★ 考研必考点，必须分清）：
 *
 * 约定一（王道教材默认，本文件采用）：
 *   - 空栈时 top = -1
 *   - 栈顶元素是 data[top]
 *   - 入栈：先 ++top，再 data[top] = x
 *   - 出栈：先 x = data[top]，再 --top
 *   - 栈满条件：top == MaxSize - 1
 *   - 栈中元素个数：top + 1
 *
 * 约定二（top指向下一个可插入位置）：
 *   - 空栈时 top = 0
 *   - 栈顶元素是 data[top - 1]
 *   - 入栈：先 data[top] = x，再 ++top
 *   - 出栈：先 --top，再 x = data[top]
 *   - 栈满条件：top == MaxSize
 *   - 栈中元素个数：top
 *
 * ★★★★★ 考试时必须先看题目说明用的是哪种约定，不能混用！
 * ============================================================
 */

// ======1. 初始化顺序栈======
void InitStack(SqStack &S) {
    S.top = -1;  // ★★★★★ 栈顶指针初始化为-1，表示空栈（约定一）
}

// ======2. 判断顺序栈是否为空======
bool StackEmpty(SqStack S) {
    return S.top == -1;  // ★★★★★ 栈顶指针为-1表示空栈，返回true；否则返回false
}

// ======3. 判断顺序栈是否为满======
bool StackFull(SqStack S) {
    return S.top == MaxSize - 1;  // ★★★★★ 栈顶指针等于MaxSize-1表示栈满
}

// ======4. 入栈（将元素x压入栈顶）======
bool Push(SqStack &S, ElemType x) {
    if(S.top == MaxSize - 1)  // ★★★★★ 栈满，不能再入栈，返回false
        return false;
    S.data[++S.top] = x;  // ★★★★★ 栈顶指针先加1（移动到新位置），再将元素x存入该位置
    return true;  // 入栈成功，返回true
}

// ======5. 出栈（弹出栈顶元素，用x返回）======
bool Pop(SqStack &S, ElemType &x) {
    if(S.top == -1)  // ★★★★★ 栈空，没有元素可以出栈，返回false
        return false;
    x = S.data[S.top--];  // ★★★★★ 先取出栈顶元素赋值给x，再将栈顶指针减1
    return true;  // 出栈成功，返回true
}

// ======6. 读取栈顶元素（不弹出，用x返回）======
bool GetTop(SqStack S, ElemType &x) {
    if(S.top == -1)  // ★★★★★ 栈空，没有栈顶元素，返回false
        return false;
    x = S.data[S.top];  // ★★★★★ 将栈顶元素的值赋值给x，不修改top指针
    return true;  // 读取成功，返回true
}

// ======7. 求栈的长度（栈中元素个数）======
int StackLength(SqStack S) {
    return S.top + 1;  // ★★★★★ top=-1时空栈长度为0，每入栈一个元素top加1，所以长度是top+1
}

// ======8. 打印栈中所有元素（从栈底到栈顶）======
void PrintStack(SqStack S) {
    if(S.top == -1) {  // 栈空，打印提示
        printf("栈为空\n");
        return;
    }
    printf("栈中元素（栈底→栈顶）：");
    for(int i = 0; i <= S.top; i++) {  // ★★★★★ 从下标0遍历到top，即从栈底到栈顶
        printf("%d ", S.data[i]);  // 打印每个元素
    }
    printf("\n");  // 换行
}

// ======主函数：测试顺序栈的所有操作======
int main() {
    SqStack S;  // 定义一个顺序栈S
    InitStack(S);  // 初始化顺序栈S

    printf("====== 顺序栈测试开始 ======\n\n");

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
    printf("       栈是否已满：%s\n", StackFull(S) ? "是" : "否");

    // 测试3：读取栈顶元素
    int topVal;  // 定义变量topVal，用于存储栈顶元素
    GetTop(S, topVal);  // 读取栈顶元素
    printf("\n【测试3】当前栈顶元素：%d\n", topVal);

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

    // 测试5：继续入栈到接近满
    printf("\n【测试5】继续入栈：60, 70, 80, 90, 100\n");
    for(int i = 6; i <= 10; i++) {
        Push(S, i * 10);  // 将 i*10 压入栈
    }
    PrintStack(S);  // 打印栈中元素
    printf("       当前栈长度：%d\n", StackLength(S));

    // 测试6：尝试在栈满时入栈（应该失败）
    printf("\n【测试6】栈满时尝试入栈999：");
    if(Push(S, 999)) {
        printf("成功（异常！）\n");
    } else {
        printf("失败（栈满，正确）\n");
    }

    // 测试7：全部出栈
    printf("\n【测试7】全部出栈：\n");
    while(!StackEmpty(S)) {  // 当栈不为空时循环出栈
        Pop(S, popVal);  // 出栈
        printf("%d ", popVal);  // 打印弹出的元素
    }
    printf("\n");
    printf("       出栈完毕，栈是否为空：%s\n", StackEmpty(S) ? "是" : "否");

    // 测试8：空栈时尝试出栈（应该失败）
    printf("\n【测试8】空栈时尝试出栈：");
    if(Pop(S, popVal)) {
        printf("成功（异常！）\n");
    } else {
        printf("失败（栈空，正确）\n");
    }

    printf("\n====== 顺序栈测试结束 ======\n");
    return 0;  // 程序正常结束
}
