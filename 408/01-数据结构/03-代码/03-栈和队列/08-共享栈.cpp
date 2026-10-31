/*
 * ============================================================
 * 文件名：08-共享栈.cpp
 * 章  节：第三章 栈、队列和数组
 * 内  容：两个栈共享一个一维数组（共享栈）
 * 重要度：★★☆☆☆ 了解即可（只考选择题判满条件，不需要完整代码）
 * 考研考点：两端向中间增长、top0+1==top1 判满、空间利用率提高
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MaxSize 50  // ★★☆☆☆ 共享数组的最大容量

typedef int ElemType;  // ★★☆☆☆ 栈中元素的类型

// ======共享栈的结构体定义======
typedef struct {
    ElemType data[MaxSize];  // ★★☆☆☆ 共享数组空间，两个栈共用这一个数组
    int top0;  // ★★☆☆☆ 栈0的栈顶指针，栈0从数组下端（下标0）向上增长
    int top1;  // ★★☆☆☆ 栈1的栈顶指针，栈1从数组上端（下标MaxSize-1）向下增长
} ShStack;  // 共享栈的结构体类型名

/*
 * ============================================================
 * 共享栈的核心概念（★★☆☆☆ 选择题考点）：
 *
 * 1. 存储空间布局：
 *    - 栈0的栈底在数组下端（下标0），栈顶向上增长
 *    - 栈1的栈底在数组上端（下标MaxSize-1），栈顶向下增长
 *    - 两个栈的栈顶在数组中间相遇
 *
 * 2. 初始化：
 *    - top0 = -1（栈0为空，栈顶在数组下端之外）
 *    - top1 = MaxSize（栈1为空，栈顶在数组上端之外）
 *
 * 3. 栈满条件（★★★☆☆ 选择题高频考点）：
 *    - top0 + 1 == top1
 *    - 即两个栈顶相邻，中间没有空闲空间了
 *    - 注意：不是 top0 == top1！
 *
 * 4. 优点：
 *    - 提高空间利用率，一个栈空间不够时可以用另一个栈的空闲空间
 *    - 两个栈的总容量是MaxSize，单个栈的容量可以动态变化
 *
 * 5. 缺点：
 *    - 必须事先确定数组大小MaxSize
 *    - 两个栈的总元素个数不能超过MaxSize
 *
 * ★★★☆☆ 考研只考选择题：判满条件 top0+1==top1，以及入栈出栈时指针怎么移动
 * ============================================================
 */

// ======1. 初始化共享栈======
void InitStack(ShStack &S) {
    S.top0 = -1;  // ★★☆☆☆ 栈0的栈顶指针初始化为-1（在数组下端之外，表示空）
    S.top1 = MaxSize;  // ★★☆☆☆ 栈1的栈顶指针初始化为MaxSize（在数组上端之外，表示空）
}

// ======2. 判断共享栈是否为满======
bool StackFull(ShStack S) {
    return S.top0 + 1 == S.top1;  // ★★★☆☆ 两个栈顶相邻，表示共享栈已满
}

// ======3. 判断栈0是否为空======
bool Stack0Empty(ShStack S) {
    return S.top0 == -1;  // 栈0的栈顶指针为-1表示栈0为空
}

// ======4. 判断栈1是否为空======
bool Stack1Empty(ShStack S) {
    return S.top1 == MaxSize;  // 栈1的栈顶指针为MaxSize表示栈1为空
}

// ======5. 栈0入栈（将元素x压入栈0的栈顶）======
bool Push0(ShStack &S, ElemType x) {
    if(S.top0 + 1 == S.top1)  // ★★★☆☆ 共享栈已满，不能入栈
        return false;
    S.data[++S.top0] = x;  // ★★☆☆☆ 栈0的栈顶指针先加1（向上移动），再存入元素
    return true;  // 入栈成功
}

// ======6. 栈1入栈（将元素x压入栈1的栈顶）======
bool Push1(ShStack &S, ElemType x) {
    if(S.top0 + 1 == S.top1)  // ★★★☆☆ 共享栈已满，不能入栈
        return false;
    S.data[--S.top1] = x;  // ★★☆☆☆ 栈1的栈顶指针先减1（向下移动），再存入元素
    return true;  // 入栈成功
}

// ======7. 栈0出栈（弹出栈0的栈顶元素，用x返回）======
bool Pop0(ShStack &S, ElemType &x) {
    if(S.top0 == -1)  // 栈0为空，不能出栈
        return false;
    x = S.data[S.top0--];  // ★★☆☆☆ 先取出栈顶元素，再将栈0的栈顶指针减1（向下移动）
    return true;  // 出栈成功
}

// ======8. 栈1出栈（弹出栈1的栈顶元素，用x返回）======
bool Pop1(ShStack &S, ElemType &x) {
    if(S.top1 == MaxSize)  // 栈1为空，不能出栈
        return false;
    x = S.data[S.top1++];  // ★★☆☆☆ 先取出栈顶元素，再将栈1的栈顶指针加1（向上移动）
    return true;  // 出栈成功
}

// ======9. 读取栈0的栈顶元素（不弹出）======
bool GetTop0(ShStack S, ElemType &x) {
    if(S.top0 == -1)  // 栈0为空
        return false;
    x = S.data[S.top0];  // 栈0的栈顶元素
    return true;
}

// ======10. 读取栈1的栈顶元素（不弹出）======
bool GetTop1(ShStack S, ElemType &x) {
    if(S.top1 == MaxSize)  // 栈1为空
        return false;
    x = S.data[S.top1];  // 栈1的栈顶元素
    return true;
}

// ======11. 打印共享栈的状态======
void PrintSharedStack(ShStack S) {
    printf("  数组下标：");
    for(int i = 0; i < MaxSize; i++) {
        if(i >= S.top0 + 1 && i <= S.top1 - 1) {
            printf(" . ");  // 空闲空间用点表示
        } else {
            printf("%2d ", S.data[i]);  // 已使用空间
        }
    }
    printf("\n");
    printf("  top0=%d（栈0栈顶），top1=%d（栈1栈顶），栈满?%s\n",
           S.top0, S.top1, StackFull(S) ? "是" : "否");
}

// ======主函数：测试共享栈======
int main() {
    ShStack S;  // 定义一个共享栈S
    InitStack(S);  // 初始化共享栈S

    printf("====== 共享栈测试开始 ======\n");
    printf("数组容量 MaxSize = %d\n\n", MaxSize);

    // 测试1：初始状态
    printf("【测试1】初始状态：\n");
    printf("  栈0是否为空：%s，栈1是否为空：%s\n", Stack0Empty(S) ? "是" : "否", Stack1Empty(S) ? "是" : "否");
    printf("  共享栈是否已满：%s\n", StackFull(S) ? "是" : "否");

    // 测试2：栈0入栈 10, 20, 30
    printf("\n【测试2】栈0入栈：10, 20, 30\n");
    for(int i = 1; i <= 3; i++) Push0(S, i * 10);
    PrintSharedStack(S);

    // 测试3：栈1入栈 90, 80, 70
    printf("\n【测试3】栈1入栈：90, 80, 70\n");
    for(int i = 9; i >= 7; i--) Push1(S, i * 10);
    PrintSharedStack(S);

    // 测试4：读取两个栈的栈顶
    int topVal;
    GetTop0(S, topVal);
    printf("\n【测试4】栈0栈顶元素：%d（应为30）\n", topVal);
    GetTop1(S, topVal);
    printf("       栈1栈顶元素：%d（应为70）\n", topVal);

    // 测试5：栈0出栈一次
    printf("\n【测试5】栈0出栈一次：\n");
    Pop0(S, topVal);
    printf("  弹出元素：%d（应为30）\n", topVal);
    PrintSharedStack(S);

    // 测试6：栈1出栈一次
    printf("\n【测试6】栈1出栈一次：\n");
    Pop1(S, topVal);
    printf("  弹出元素：%d（应为70）\n", topVal);
    PrintSharedStack(S);

    // 测试7：★★★☆☆ 重点测试——两个栈轮流入栈直到满，验证判满条件
    printf("\n【测试7】重点测试：两个栈轮流入栈直到共享栈满，验证 top0+1==top1\n");
    int count0 = 0, count1 = 0;
    while(!StackFull(S)) {
        if(count0 <= count1) {
            Push0(S, 100 + count0);
            count0++;
        } else {
            Push1(S, 200 + count1);
            count1++;
        }
    }
    printf("  栈0共入栈%d个元素，栈1共入栈%d个元素，总计%d个（应为%d）\n",
           count0, count1, count0 + count1, MaxSize);
    printf("  top0=%d, top1=%d, top0+1=%d == top1? %s\n",
           S.top0, S.top1, S.top0 + 1, (S.top0 + 1 == S.top1) ? "是（栈满）" : "否");

    // 测试8：栈满时尝试入栈（应该失败）
    printf("\n【测试8】栈满时栈0尝试入栈999：");
    if(Push0(S, 999)) printf("成功（异常！）\n");
    else printf("失败（栈满，正确）\n");
    printf("       栈满时栈1尝试入栈888：");
    if(Push1(S, 888)) printf("成功（异常！）\n");
    else printf("失败（栈满，正确）\n");

    // 测试9：全部出栈
    printf("\n【测试9】栈0全部出栈：\n");
    printf("  ");
    while(!Stack0Empty(S)) {
        Pop0(S, topVal);
        printf("%d ", topVal);
    }
    printf("\n");
    printf("  栈1全部出栈：\n");
    printf("  ");
    while(!Stack1Empty(S)) {
        Pop1(S, topVal);
        printf("%d ", topVal);
    }
    printf("\n");

    printf("\n====== 共享栈测试结束 ======\n");
    printf("\n★ 考研重点提醒：共享栈只考选择题！记住判满条件 top0+1==top1 即可。\n");
    return 0;
}
