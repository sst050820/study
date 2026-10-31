/*
 * ============================================================
 * 文件名：02-朴素模式匹配算法.cpp
 * 章  节：第四章 串  4.2.1 朴素模式匹配（BF，Brute-Force）
 * 内  容：双指针直接操作数组下标的暴力匹配算法
 * 重要度：★★★★ 必须熟练掌握（KMP的对照对象，选择题常考复杂度与指针移动）
 * 考研考点：失配回退公式 i=i-j+2、j=1；成功返回 i-m；最坏时间复杂度 O(nm)
 * ============================================================
 */

#include <stdio.h>
#include <string.h>

#define MAXLEN 255

// ======定长顺序串结构体（王道约定：ch[0]废弃，字符从ch[1]开始）======
typedef struct {
    char ch[MAXLEN];
    int length;
} SString;

// ======把C字符串赋给顺序串======
void StrAssign(SString &S, const char chars[]) {
    int len = (int)strlen(chars);
    for(int i = 1; i <= len; i++)
        S.ch[i] = chars[i - 1];
    S.ch[len + 1] = '\0';               // 末尾补'\0'：便于用%s打印，并清除上次赋值的残留字符
    S.length = len;
}

/*
 * ============================================================
 * 朴素模式匹配算法（★★★★ 核心）
 *   - i 扫描主串S，j 扫描模式串T，都从1开始
 *   - 字符相等：i、j 同时后移
 *   - 字符失配：i 回退到"下一个候选子串"的起点，j 回到1
 *       i = i - j + 2  的推导：
 *         i-j 让 i 回到当前子串起点的前一个位置，+2 才是下一子串起点
 *   - j 超出模式串长度 => 匹配成功，返回起始位置 i-m
 *   - i 超出主串仍未成功 => 返回0（匹配失败）
 * 复杂度：最好O(n)，最坏O(nm)（每个子串前m-1个都相等、最后才失配）
 * ============================================================
 */
int Index_BF(SString S, SString T) {
    int i = 1, j = 1;                 // ★★★★ 双指针都从1开始
    int cmpCount = 0;                 // 统计字符比较次数（帮助理解复杂度）
    while(i <= S.length && j <= T.length) {
        cmpCount++;
        if(S.ch[i] == T.ch[j]) {      // ★★★★ 当前字符相等
            i++;
            j++;                      // 两指针同时后移
        } else {                      // ★★★★ 失配
            i = i - j + 2;            // ★★★★★ 主串指针回退到下一子串起点（1基公式）
            j = 1;                    // ★★★★★ 模式串指针回到第1个字符
        }
    }
    printf("       （本次共比较字符 %d 次）\n", cmpCount);
    if(j > T.length) {                // ★★★★ 模式串被完整走完，匹配成功
        return i - T.length;          // ★★★★★ 返回匹配子串的起始位置
    }
    return 0;                         // 匹配失败
}

// ======主函数：测试朴素模式匹配======
int main() {
    SString S, T;

    printf("====== 朴素模式匹配（BF）测试开始 ======\n\n");

    // 测试1：正常匹配
    printf("【测试1】正常匹配\n");
    StrAssign(S, "ababcabcacbab");
    StrAssign(T, "abcac");
    printf("       主串S = %s\n", S.ch + 1);
    printf("       模式串T = %s\n", T.ch + 1);
    int pos = Index_BF(S, T);
    printf("       匹配结果：%d（%s）\n\n", pos, pos ? "匹配成功" : "匹配失败");

    // 测试2：模式串不存在
    printf("【测试2】模式串不存在\n");
    StrAssign(T, "abcd");
    printf("       主串S = %s\n", S.ch + 1);
    printf("       模式串T = %s\n", T.ch + 1);
    pos = Index_BF(S, T);
    printf("       匹配结果：%d（%s）\n\n", pos, pos ? "匹配成功" : "匹配失败，返回0");

    // 测试3：最坏情况演示（大量前缀相同，只在末尾失配）
    printf("【测试3】最坏情况：主串全a末尾b，模式串aaaab，体会比较次数\n");
    StrAssign(S, "aaaaaaaaaaab");    // 11个a + b
    StrAssign(T, "aaaab");          // 4个a + b
    printf("       主串S = %s（长%d）\n", S.ch + 1, S.length);
    printf("       模式串T = %s（长%d）\n", T.ch + 1, T.length);
    pos = Index_BF(S, T);
    printf("       匹配结果：%d\n", pos);
    printf("       ★ 可见主串指针i反复回退，比较次数接近 n*m，这就是O(nm)最坏情况\n");

    printf("\n====== 朴素模式匹配测试结束 ======\n");
    return 0;
}
