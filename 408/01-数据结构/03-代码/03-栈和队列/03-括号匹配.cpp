/*
 * ============================================================
 * 文件名：03-括号匹配.cpp
 * 章  节：第三章 栈、队列和数组
 * 内  容：栈在括号匹配中的应用
 * 重要度：★★★★☆ 建议能默写（栈的经典应用，代码短，可能考算法题）
 * 考研考点：左括号入栈、右括号弹栈匹配、扫描结束栈必须为空
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MaxSize 100  // ★★★★☆ 定义栈中元素的最大个数（括号序列的最大长度）

typedef char ElemType;  // ★★★★☆ 括号匹配中栈存储的是字符，所以ElemType为char

// ======顺序栈的结构体定义（复用顺序栈，top=-1约定）======
typedef struct {
    ElemType data[MaxSize];  // 静态数组，存放栈中的字符
    int top;  // 栈顶指针
} SqStack;  // 顺序栈的结构体类型名

// ======1. 初始化顺序栈======
void InitStack(SqStack &S) {
    S.top = -1;  // 栈顶指针初始化为-1，表示空栈
}

// ======2. 判断顺序栈是否为空======
bool StackEmpty(SqStack S) {
    return S.top == -1;  // 栈顶指针为-1表示空栈
}

// ======3. 入栈======
bool Push(SqStack &S, ElemType x) {
    if(S.top == MaxSize - 1)  // 栈满，不能入栈
        return false;
    S.data[++S.top] = x;  // 栈顶指针先加1，再存入元素
    return true;
}

// ======4. 出栈======
bool Pop(SqStack &S, ElemType &x) {
    if(S.top == -1)  // 栈空，不能出栈
        return false;
    x = S.data[S.top--];  // 先取出栈顶元素，再将栈顶指针减1
    return true;
}

// ======5. 读取栈顶元素（不弹出）======
bool GetTop(SqStack S, ElemType &x) {
    if(S.top == -1)  // 栈空，没有栈顶元素
        return false;
    x = S.data[S.top];  // 将栈顶元素赋值给x
    return true;
}

/*
 * ============================================================
 * 括号匹配算法核心思想（★★★★☆ 必须理解）：
 *
 * 利用栈的"后进先出"特性：
 * 1. 从左到右扫描表达式
 * 2. 遇到左括号（(、[、{），将其压入栈中
 * 3. 遇到右括号（)、]、}）：
 *    a. 若栈为空，说明右括号没有对应的左括号，匹配失败
 *    b. 弹出栈顶元素，检查是否与当前右括号匹配
 *    c. 若不匹配，匹配失败
 * 4. 扫描结束后，若栈不为空，说明有左括号没有对应的右括号，匹配失败
 *
 * 时间复杂度：O(n)，每个字符最多入栈出栈各一次
 * 空间复杂度：O(n)，最坏情况下所有字符都是左括号
 * ============================================================
 */

// ======6. 括号匹配算法（核心函数）======
bool BracketMatch(char *str, int n) {
    SqStack S;  // 定义一个顺序栈S，用于存储左括号
    InitStack(S);  // 初始化栈S

    for(int i = 0; i < n; i++) {  // ★★★★☆ 从左到右扫描表达式
        if(str[i] == '(' || str[i] == '[' || str[i] == '{') {
            // ★★★★☆ 遇到左括号，直接压入栈中
            Push(S, str[i]);
        } else if(str[i] == ')' || str[i] == ']' || str[i] == '}') {
            // ★★★★☆ 遇到右括号
            if(StackEmpty(S)) {
                // ★★★★☆ 栈为空，说明当前右括号没有对应的左括号，匹配失败
                printf("  位置%d：遇到右括号'%c'，但栈为空，匹配失败！\n", i + 1, str[i]);
                return false;
            }

            char topElem;  // 定义变量topElem，用于存储弹出的栈顶左括号
            Pop(S, topElem);  // 弹出栈顶元素

            // ★★★★☆ 检查栈顶左括号是否与当前右括号匹配
            if(str[i] == ')' && topElem != '(') {
                printf("  位置%d：右括号')'与左括号'%c'不匹配！\n", i + 1, topElem);
                return false;
            }
            if(str[i] == ']' && topElem != '[') {
                printf("  位置%d：右括号']'与左括号'%c'不匹配！\n", i + 1, topElem);
                return false;
            }
            if(str[i] == '}' && topElem != '{') {
                printf("  位置%d：右括号'}'与左括号'%c'不匹配！\n", i + 1, topElem);
                return false;
            }

            // 匹配成功，打印过程
            printf("  位置%d：右括号'%c'与左括号'%c'匹配成功\n", i + 1, str[i], topElem);
        }
        // 非括号字符直接跳过（本算法只检查括号，不处理其他字符）
    }

    // ★★★★☆ 扫描结束后，栈必须为空才匹配成功
    if(!StackEmpty(S)) {
        // 栈不为空，说明有左括号没有对应的右括号
        printf("  扫描结束，但栈中还有未匹配的左括号！\n");
        return false;
    }

    return true;  // 所有括号都匹配成功
}

// ======主函数：测试括号匹配算法======
int main() {
    printf("====== 括号匹配测试开始 ======\n\n");

    // 测试用例1：完全匹配的括号序列
    char str1[] = "({[]})";
    printf("【测试1】表达式：%s\n", str1);
    printf("  匹配过程：\n");
    if(BracketMatch(str1, strlen(str1))) {
        printf("  结果：括号匹配成功 ✓\n");
    } else {
        printf("  结果：括号匹配失败 ✗\n");
    }

    // 测试用例2：不匹配的括号序列（交叉错误）
    char str2[] = "([)]";
    printf("\n【测试2】表达式：%s\n", str2);
    printf("  匹配过程：\n");
    if(BracketMatch(str2, strlen(str2))) {
        printf("  结果：括号匹配成功 ✓\n");
    } else {
        printf("  结果：括号匹配失败 ✗\n");
    }

    // 测试用例3：右括号多余
    char str3[] = "(()))";
    printf("\n【测试3】表达式：%s\n", str3);
    printf("  匹配过程：\n");
    if(BracketMatch(str3, strlen(str3))) {
        printf("  结果：括号匹配成功 ✓\n");
    } else {
        printf("  结果：括号匹配失败 ✗\n");
    }

    // 测试用例4：左括号多余
    char str4[] = "((()))(";
    printf("\n【测试4】表达式：%s\n", str4);
    printf("  匹配过程：\n");
    if(BracketMatch(str4, strlen(str4))) {
        printf("  结果：括号匹配成功 ✓\n");
    } else {
        printf("  结果：括号匹配失败 ✗\n");
    }

    // 测试用例5：包含其他字符的表达式
    char str5[] = "a+(b*c)-{d/[e+f]}";
    printf("\n【测试5】表达式：%s\n", str5);
    printf("  匹配过程：\n");
    if(BracketMatch(str5, strlen(str5))) {
        printf("  结果：括号匹配成功 ✓\n");
    } else {
        printf("  结果：括号匹配失败 ✗\n");
    }

    // 测试用例6：空字符串
    char str6[] = "";
    printf("\n【测试6】空表达式：\n");
    if(BracketMatch(str6, strlen(str6))) {
        printf("  结果：括号匹配成功 ✓（空表达式视为合法）\n");
    } else {
        printf("  结果：括号匹配失败 ✗\n");
    }

    printf("\n====== 括号匹配测试结束 ======\n");
    return 0;  // 程序正常结束
}
