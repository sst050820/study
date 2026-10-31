/*
 * ============================================================
 * 文件名：04-KMP模式匹配算法.cpp
 * 章  节：第四章 串  4.2.2 KMP算法（匹配主过程）
 * 内  容：先用模式串求next数组，再用next数组完成KMP匹配
 * 重要度：★★★★★ 必须熟练掌握（本章算法核心，需看懂并能默写匹配主干）
 * 考研考点：主串指针i不回溯、失配 j=next[j]、成功返回 i-m、总复杂度O(m+n)
 * ============================================================
 */

#include <stdio.h>
#include <string.h>

#define MAXLEN 255

typedef struct {
    char ch[MAXLEN];
    int length;
} SString;

void StrAssign(SString &S, const char chars[]) {
    int len = (int)strlen(chars);
    for(int i = 1; i <= len; i++)
        S.ch[i] = chars[i - 1];
    S.ch[len + 1] = '\0';               // 末尾补'\0'：便于用%s打印，并清除上次赋值的残留字符
    S.length = len;
}

// ======1. 求模式串T的next数组（预处理，O(m)）======
void GetNext(SString T, int next[]) {
    int i = 1, j = 0;
    next[1] = 0;                        // ★★★★★ next[1]固定为0
    while(i < T.length) {
        if(j == 0 || T.ch[i] == T.ch[j]) {
            i++;
            j++;
            next[i] = j;
        } else {
            j = next[j];                // ★★★★ 模式串自身也用next回退
        }
    }
}

/*
 * ============================================================
 * 2. KMP模式匹配主过程（★★★★★ 8行主干，必须会默写）
 *   - 与朴素BF相比，主串指针 i 【从不回退】，只一路向后
 *   - 相等 或 j==0：i、j 同时后移
 *       （j==0 表示第1个字符就失配，需要整体前进到下一个主串字符）
 *   - 失配：i 不动，j = next[j]，跳到模式串该跳的位置
 *   - j 走完模式串 => 成功，返回 i-m；否则返回0
 * 复杂度：预处理O(m) + 匹配O(n) = O(m+n)，空间O(m)
 * ============================================================
 */
int Index_KMP(SString S, SString T, int next[]) {
    int i = 1, j = 1;
    int cmpCount = 0;                   // 统计比较次数，与BF对比体会优化
    while(i <= S.length && j <= T.length) {
        cmpCount++;
        if(j == 0 || S.ch[i] == T.ch[j]) {  // ★★★★★ j==0 或字符相等
            i++;
            j++;                            // 两指针共同后移，i只增不减
        } else {
            j = next[j];                    // ★★★★★ 失配：i不动，j跳到next[j]
        }
    }
    printf("       （KMP共比较字符 %d 次）\n", cmpCount);
    if(j > T.length) {
        return i - T.length;                // ★★★★★ 成功，返回起始位置
    }
    return 0;
}

// ======主函数：测试KMP匹配======
int main() {
    SString S, T;
    int next[MAXLEN];

    printf("====== KMP模式匹配测试开始 ======\n\n");

    // 测试1：视频经典例子
    printf("【测试1】经典匹配\n");
    StrAssign(S, "ababcabcacbab");
    StrAssign(T, "abcac");
    GetNext(T, next);
    printf("       主串S = %s\n", S.ch + 1);
    printf("       模式串T = %s\n", T.ch + 1);
    printf("       T的next数组：");
    for(int j = 1; j <= T.length; j++) printf("%d ", next[j]);
    printf("\n");
    int pos = Index_KMP(S, T, next);
    printf("       匹配结果：起始位置 = %d（%s）\n\n", pos, pos ? "成功" : "失败");

    // 测试2：google咯google
    printf("【测试2】视频例题 google\n");
    StrAssign(S, "googlegoogle");
    StrAssign(T, "google");
    GetNext(T, next);
    pos = Index_KMP(S, T, next);
    printf("       主串S = %s，模式串T = %s\n", S.ch + 1, T.ch + 1);
    printf("       匹配结果：起始位置 = %d\n\n", pos);

    // 测试3：最坏情况下对比BF，体会i不回溯
    printf("【测试3】大量重复字符，体会KMP比较次数明显少于BF的O(nm)\n");
    StrAssign(S, "aaaaaaaaaaab");
    StrAssign(T, "aaaab");
    GetNext(T, next);
    pos = Index_KMP(S, T, next);
    printf("       主串S = %s，模式串T = %s\n", S.ch + 1, T.ch + 1);
    printf("       匹配结果：起始位置 = %d\n", pos);
    printf("       ★ 主串指针i全程不回头，总复杂度O(m+n)\n");

    // 测试4：匹配失败
    printf("\n【测试4】模式串不存在\n");
    StrAssign(S, "abcdefg");
    StrAssign(T, "xyz");
    GetNext(T, next);
    pos = Index_KMP(S, T, next);
    printf("       匹配结果：%d（0表示失败）\n", pos);

    printf("\n====== KMP模式匹配测试结束 ======\n");
    return 0;
}
