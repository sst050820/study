/*
 * ============================================================
 * 文件名：05-后缀表达式求值.cpp
 * 章  节：第三章 栈、队列和数组
 * 内  容：后缀表达式（逆波兰式）求值
 * 重要度：★★★★☆ 代码理解即可，手算必须熟练（选择题常考）
 * 考研考点：操作数入栈、运算符弹两个操作数、先弹右后弹左
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MaxSize 100  // ★★★★☆ 定义栈中元素的最大个数

typedef int ElemType;  // ★★★★☆ 操作数栈存储的是整数，所以ElemType为int

// ======顺序栈的结构体定义======
typedef struct {
    ElemType data[MaxSize];  // 静态数组，存放栈中的操作数
    int top;  // 栈顶指针
} SqStack;

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
    if(S.top == MaxSize - 1)  // 栈满
        return false;
    S.data[++S.top] = x;  // 栈顶指针先加1，再存入元素
    return true;
}

// ======4. 出栈======
bool Pop(SqStack &S, ElemType &x) {
    if(S.top == -1)  // 栈空
        return false;
    x = S.data[S.top--];  // 先取出栈顶元素，再将栈顶指针减1
    return true;
}

/*
 * ============================================================
 * 后缀表达式求值算法（★★★★☆ 核心，必须理解）：
 *
 * 后缀表达式的优点：不需要括号，运算符出现的顺序就是运算顺序。
 *
 * 算法步骤：
 * 1. 从左到右扫描后缀表达式
 * 2. 遇到操作数（数字）：将其压入栈中
 * 3. 遇到运算符（+、-、*、/）：
 *    a. 弹出栈顶两个操作数
 *    b. ★★★★★ 第一次弹出的是右操作数（b），第二次弹出的是左操作数（a）
 *    c. 计算 a 运算符 b，将结果压回栈中
 * 4. 扫描结束后，栈中仅剩一个元素，就是表达式的值
 *
 * ★★★★★ 超级重点：减法和除法的操作数顺序不能反！
 *   先弹出的是右操作数，后弹出的是左操作数
 *   例如后缀 "5 3 -"：先弹3（右），再弹5（左），计算 5-3=2
 *   如果搞反了算成 3-5=-2，就错了！
 *
 * 时间复杂度：O(n)，每个元素入栈出栈各一次
 * 空间复杂度：O(n)，最坏情况下栈中存储所有操作数
 * ============================================================
 */

// ======5. 后缀表达式求值（核心函数）======
int EvalPostfix(char *postfix) {
    SqStack S;  // 定义操作数栈S
    InitStack(S);  // 初始化栈S

    printf("  求值过程：\n");
    printf("  %-8s %-20s %-30s\n", "扫描字符", "操作", "栈内容（栈底→栈顶）");
    printf("  %-8s %-20s %-30s\n", "--------", "--------------------", "------------------------------");

    for(int i = 0; postfix[i] != '\0'; i++) {  // ★★★★☆ 从左到右扫描后缀表达式
        char c = postfix[i];  // 当前扫描到的字符

        if(isdigit(c)) {
            // ★★★★☆ 情况1：遇到数字操作数，压入栈中
            int num = c - '0';  // 将字符转换为整数（'5'-'0'=5）
            Push(S, num);  // 压入栈中
            printf("  %-8c %-20s ", c, "入栈");
            printf("[");
            for(int k = 0; k <= S.top; k++) printf("%d ", S.data[k]);
            printf("]\n");
        }
        else if(c == '+' || c == '-' || c == '*' || c == '/') {
            // ★★★★☆ 情况2：遇到运算符，弹出两个操作数计算
            int b, a;  // ★★★★★ b是右操作数，a是左操作数（注意顺序！）
            Pop(S, b);  // ★★★★★ 先弹出的是右操作数b
            Pop(S, a);  // ★★★★★ 后弹出的是左操作数a

            int result;  // 定义变量result，用于存储运算结果
            switch(c) {
                case '+': result = a + b; break;  // 加法：a + b
                case '-': result = a - b; break;  // ★★★★★ 减法：a - b（不是b-a！）
                case '*': result = a * b; break;  // 乘法：a * b
                case '/': result = a / b; break;  // ★★★★★ 除法：a / b（不是b/a！）
            }

            Push(S, result);  // ★★★★☆ 将运算结果压回栈中

            char opStr[30];
            sprintf(opStr, "弹%d,%d 算%d%c%d=%d", b, a, a, c, b, result);
            printf("  %-8c %-20s ", c, opStr);
            printf("[");
            for(int k = 0; k <= S.top; k++) printf("%d ", S.data[k]);
            printf("]\n");
        }
        // 空格等其他字符直接跳过
    }

    // ★★★★☆ 扫描结束后，栈中仅剩一个元素，就是最终结果
    int finalResult;  // 定义变量finalResult，用于存储最终结果
    Pop(S, finalResult);  // 弹出栈中唯一的元素
    return finalResult;  // 返回最终结果
}

// ======主函数：测试后缀表达式求值======
int main() {
    char postfix[MaxSize];  // 后缀表达式

    printf("====== 后缀表达式求值测试 ======\n\n");

    // 测试用例1：5 2 3 * - （对应中缀 5 - 2 * 3 = 5 - 6 = -1）
    strcpy(postfix, "523*-");
    printf("【测试1】后缀表达式：5 2 3 * -\n");
    int result1 = EvalPostfix(postfix);
    printf("  最终结果：%d（预期：5 - 2*3 = -1）\n\n", result1);

    // 测试用例2：3 5 + 2 * （对应中缀 (3+5)*2 = 16）
    strcpy(postfix, "35+2*");
    printf("【测试2】后缀表达式：3 5 + 2 *\n");
    int result2 = EvalPostfix(postfix);
    printf("  最终结果：%d（预期：(3+5)*2 = 16）\n\n", result2);

    // 测试用例3：8 3 2 + * 4 - （对应中缀 8*(3+2)-4 = 36）
    strcpy(postfix, "832+*4-");
    printf("【测试3】后缀表达式：8 3 2 + * 4 -\n");
    int result3 = EvalPostfix(postfix);
    printf("  最终结果：%d（预期：8*(3+2)-4 = 36）\n\n", result3);

    // 测试用例4：1 2 + 3 4 + * （对应中缀 (1+2)*(3+4) = 21）
    strcpy(postfix, "12+34+*");
    printf("【测试4】后缀表达式：1 2 + 3 4 + *\n");
    int result4 = EvalPostfix(postfix);
    printf("  最终结果：%d（预期：(1+2)*(3+4) = 21）\n\n", result4);

    // 测试用例5：10 6 2 + / （对应中缀 10/(6+2) = 1）
    strcpy(postfix, "10 6 2 + /");  // 注意：多位数需要空格分隔
    printf("【测试5】后缀表达式：10 6 2 + /（多位数用空格分隔）\n");
    printf("  （本代码仅支持单个数字，多位数需要额外的词法分析处理）\n");
    printf("  预期结果：10/(6+2) = 1\n\n");

    printf("====== 测试结束 ======\n");
    printf("\n★ 重点提醒：\n");
    printf("  1. 遇到运算符时，先弹出的是右操作数，后弹出的是左操作数\n");
    printf("  2. 减法和除法的顺序绝对不能搞反！\n");
    printf("  3. 手算时建议每一步都画出栈的状态，避免出错\n");
    return 0;
}
