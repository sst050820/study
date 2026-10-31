/*
 * ============================================================
 * 文件名：04-中缀转后缀.cpp
 * 章  节：第三章 栈、队列和数组
 * 内  容：中缀表达式转后缀表达式（逆波兰式）
 * 重要度：★★★★☆ 代码理解即可，手算必须熟练（选择题必考）
 * 考研考点：运算符优先级、左结合、括号处理、栈的状态变化
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MaxSize 100  // ★★★★☆ 定义栈中元素的最大个数

typedef char ElemType;  // ★★★★☆ 运算符栈存储的是字符，所以ElemType为char

// ======顺序栈的结构体定义======
typedef struct {
    ElemType data[MaxSize];  // 静态数组，存放栈中的运算符
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

// ======5. 读取栈顶元素（不弹出）======
bool GetTop(SqStack S, ElemType &x) {
    if(S.top == -1)  // 栈空
        return false;
    x = S.data[S.top];  // 将栈顶元素赋值给x
    return true;
}

/*
 * ============================================================
 * 运算符优先级（★★★★☆ 必须记住）：
 *
 *   括号：( 和 )  优先级最高（特殊处理）
 *   乘除：* 和 /  优先级 = 2
 *   加减：+ 和 -  优先级 = 1
 *   左括号：(      优先级 = 0（在栈中不会被其他运算符弹出）
 *
 * 结合性：
 *   +、-、*、/ 都是左结合（从左到右计算）
 *   同优先级时，栈顶的运算符先弹出（因为左结合）
 * ============================================================
 */

// ======6. 获取运算符的优先级======
int Priority(char op) {
    if(op == '*' || op == '/')
        return 2;  // ★★★★☆ 乘除优先级为2
    if(op == '+' || op == '-')
        return 1;  // ★★★★☆ 加减优先级为1
    if(op == '(')
        return 0;  // ★★★★☆ 左括号在栈中的优先级为0，不会被其他运算符弹出
    return -1;  // 其他字符（不应出现）
}

/*
 * ============================================================
 * 中缀表达式转后缀表达式算法（★★★★☆ 核心，必须理解）：
 *
 * 从左到右扫描中缀表达式：
 * 1. 遇到操作数（数字、字母）：直接加入后缀表达式
 * 2. 遇到左括号 (：压入栈中
 * 3. 遇到右括号 )：依次弹出栈中运算符加入后缀表达式，
 *    直到弹出左括号为止（左括号不加入后缀表达式）
 * 4. 遇到运算符 +、-、*、/：
 *    a. 依次弹出栈中优先级 >= 当前运算符优先级的运算符
 *       （因为都是左结合，同优先级也要弹出）
 *    b. 将当前运算符压入栈中
 * 5. 扫描结束后，将栈中剩余的运算符依次弹出加入后缀表达式
 *
 * 注意：操作数的相对顺序在中缀和后缀中保持不变！
 * ============================================================
 */

// ======7. 中缀表达式转后缀表达式（核心函数）======
void InfixToPostfix(char *infix, char *postfix) {
    SqStack S;  // 定义运算符栈S
    InitStack(S);  // 初始化栈S
    int j = 0;  // ★★★★☆ 后缀表达式的下标计数器，初始为0

    printf("  转换过程：\n");
    printf("  %-8s %-20s %-20s\n", "扫描字符", "后缀表达式", "运算符栈");
    printf("  %-8s %-20s %-20s\n", "--------", "--------------------", "--------------------");

    for(int i = 0; infix[i] != '\0'; i++) {  // ★★★★☆ 从左到右扫描中缀表达式
        char c = infix[i];  // 当前扫描到的字符

        if(isalnum(c)) {
            // ★★★★☆ 情况1：遇到操作数（字母或数字），直接加入后缀表达式
            postfix[j++] = c;
            printf("  %-8c %-20s ", c, postfix);
            // 打印栈内容
            printf("[");
            for(int k = 0; k <= S.top; k++) printf("%c", S.data[k]);
            printf("]\n");
        }
        else if(c == '(') {
            // ★★★★☆ 情况2：遇到左括号，直接压入栈中
            Push(S, c);
            printf("  %-8c %-20s ", c, postfix);
            printf("[");
            for(int k = 0; k <= S.top; k++) printf("%c", S.data[k]);
            printf("]\n");
        }
        else if(c == ')') {
            // ★★★★☆ 情况3：遇到右括号，弹出栈中运算符直到左括号
            char op;  // 定义变量op，用于存储弹出的运算符
            Pop(S, op);  // 先弹出栈顶元素
            while(op != '(') {  // ★★★★☆ 直到弹出左括号为止
                postfix[j++] = op;  // 将弹出的运算符加入后缀表达式
                Pop(S, op);  // 继续弹出栈顶元素
            }
            // ★★★★☆ 左括号不加入后缀表达式，直接丢弃
            printf("  %-8c %-20s ", c, postfix);
            printf("[");
            for(int k = 0; k <= S.top; k++) printf("%c", S.data[k]);
            printf("]\n");
        }
        else if(c == '+' || c == '-' || c == '*' || c == '/') {
            // ★★★★☆ 情况4：遇到运算符 +、-、*、/
            while(!StackEmpty(S)) {
                char topOp;  // 定义变量topOp，用于存储栈顶运算符
                GetTop(S, topOp);  // 读取栈顶运算符（不弹出）

                // ★★★★☆ 比较优先级：若栈顶运算符优先级 >= 当前运算符优先级
                // （因为都是左结合，同优先级也要弹出栈顶的）
                if(Priority(topOp) >= Priority(c)) {
                    Pop(S, topOp);  // 弹出栈顶运算符
                    postfix[j++] = topOp;  // 加入后缀表达式
                } else {
                    break;  // ★★★★☆ 栈顶运算符优先级更低，停止弹出
                }
            }
            Push(S, c);  // ★★★★☆ 将当前运算符压入栈中
            printf("  %-8c %-20s ", c, postfix);
            printf("[");
            for(int k = 0; k <= S.top; k++) printf("%c", S.data[k]);
            printf("]\n");
        }
        // 其他字符（如空格）直接跳过
    }

    // ★★★★☆ 扫描结束后，将栈中剩余的运算符依次弹出加入后缀表达式
    while(!StackEmpty(S)) {
        char op;  // 定义变量op，用于存储弹出的运算符
        Pop(S, op);  // 弹出栈顶运算符
        postfix[j++] = op;  // 加入后缀表达式
    }
    postfix[j] = '\0';  // ★★★★☆ 后缀表达式结束符

    printf("  %-8s %-20s %-20s\n", "结束", postfix, "[]（栈空）");
}

// ======主函数：测试中缀转后缀======
int main() {
    char infix[MaxSize];   // 中缀表达式
    char postfix[MaxSize];  // 后缀表达式

    printf("====== 中缀表达式转后缀表达式测试 ======\n\n");

    // 测试用例1：a+b*c-d/e
    strcpy(infix, "a+b*c-d/e");
    printf("【测试1】中缀表达式：%s\n", infix);
    InfixToPostfix(infix, postfix);
    printf("  后缀表达式：%s\n\n", postfix);

    // 测试用例2：a+b-c*d/e+f（全是同优先级和不同优先级混合）
    strcpy(infix, "a+b-c*d/e+f");
    printf("【测试2】中缀表达式：%s\n", infix);
    InfixToPostfix(infix, postfix);
    printf("  后缀表达式：%s\n\n", postfix);

    // 测试用例3：带括号的表达式 (a+b)*(c-d)
    strcpy(infix, "(a+b)*(c-d)");
    printf("【测试3】中缀表达式：%s\n", infix);
    InfixToPostfix(infix, postfix);
    printf("  后缀表达式：%s\n\n", postfix);

    // 测试用例4：嵌套括号 a+(b*c-(d/e+f)*g)*h
    strcpy(infix, "a+(b*c-(d/e+f)*g)*h");
    printf("【测试4】中缀表达式：%s\n", infix);
    InfixToPostfix(infix, postfix);
    printf("  后缀表达式：%s\n\n", postfix);

    // 测试用例5：数字表达式 3+5*2-8/4
    strcpy(infix, "3+5*2-8/4");
    printf("【测试5】中缀表达式：%s\n", infix);
    InfixToPostfix(infix, postfix);
    printf("  后缀表达式：%s\n", postfix);

    printf("\n====== 测试结束 ======\n");
    printf("\n★ 手算练习建议：拿到中缀表达式，先在草稿纸上画出每一步栈的状态，\n");
    printf("  这是选择题最常考的形式！\n");
    return 0;
}
