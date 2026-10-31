/*
 * ============================================================
 * 文件名：03-求next数组.cpp
 * 章  节：第四章 串  4.2.2 KMP算法（求next数组）
 * 内  容：手算next数组的方法 + 程序求next数组（王道1基版本）
 * 重要度：★★★★★ 必须熟练掌握（本章选择题第一高频考点）
 * 考研考点：next[1]=0、next[2]=1；next[j]=已匹配段最长相等前后缀长度+1
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

/*
 * ============================================================
 * 一、手算 next[j] 的方法（★★★★★ 考试就考这个，务必动笔练）
 *
 * next[j] 的含义：模式串第 j 个字符【失配】时，模式串指针应跳到第几个字符。
 *
 * 固定结论：next[1]=0，next[2]=1（任何模式串都成立）。
 *
 * 对 j>=3：只看失配字符【前面】那段 T[1..j-1]
 *   1) 找它的【最长相等真前缀与真后缀】，设长度为 L
 *      （真前缀/真后缀不能取整段本身）
 *   2) next[j] = L + 1
 *   3) 若找不到任何相等前后缀，则 next[j] = 1
 *
 * 例：模式串 abaabc
 *   j=6 时看前5个 "abaab"，最长相等前后缀是 "ab"(长2)，故 next[6]=3
 *   j=4 时看前3个 "aba"，  最长相等前后缀是 "a" (长1)，故 next[4]=2
 *
 * 二、程序求法（递推，时间复杂度O(m)）
 *   i 是当前正在求 next 的位置的"前驱游标"，j 是当前匹配的前后缀长度
 *   相等则 i++,j++,next[i]=j；失配则 j=next[j] 回退
 * ============================================================
 */
void GetNext(SString T, int next[]) {
    int i = 1, j = 0;
    next[1] = 0;                        // ★★★★★ 固定：next[1]=0
    while(i < T.length) {
        if(j == 0 || T.ch[i] == T.ch[j]) {  // j==0 或对应字符相等
            i++;
            j++;
            next[i] = j;                // ★★★★★ 递推得到 next[i]
        } else {
            j = next[j];                // ★★★★ 失配，j沿next回退（i不动）
        }
    }
}

// ======打印模式串及其next数组======
void PrintNext(const char *str) {
    SString T;
    StrAssign(T, str);
    int next[MAXLEN];
    GetNext(T, next);

    printf("       模式串：%-10s  下标：", str);
    for(int j = 1; j <= T.length; j++) printf("%2d", j);
    printf("\n                                字符：");
    for(int j = 1; j <= T.length; j++) printf("%2c", T.ch[j]);
    printf("\n                                next：");
    for(int j = 1; j <= T.length; j++) printf("%2d", next[j]);
    printf("\n\n");
}

// ======主函数：对多个经典模式串求next数组======
int main() {
    printf("====== 求next数组测试开始 ======\n\n");

    printf("【测试1】视频例题 abaabc（答案 next: 0 1 1 2 2 3）\n");
    PrintNext("abaabc");

    printf("【测试2】视频例题 google（答案 next: 0 1 1 1 2 1）\n");
    PrintNext("google");

    printf("【测试3】视频例题 ababaa（答案 next: 0 1 1 2 3 4）\n");
    PrintNext("ababaa");

    printf("【测试4】连续重复 aaaab（答案 next: 0 1 2 3 4）\n");
    PrintNext("aaaab");

    printf("★ 规律：next[1]恒为0、next[2]恒为1；其余看失配字符前的最长相等前后缀\n");
    printf("\n====== 求next数组测试结束 ======\n");
    return 0;
}
