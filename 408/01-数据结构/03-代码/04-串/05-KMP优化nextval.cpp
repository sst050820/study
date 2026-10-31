/*
 * ============================================================
 * 文件名：05-KMP优化nextval.cpp
 * 章  节：第四章 串  4.2.3 KMP算法的进一步优化（nextval）
 * 内  容：在next基础上求nextval数组，并用nextval完成匹配
 * 重要度：★★★★ 熟练掌握（选择题常让你手算nextval）
 * 考研考点：nextval手算规则、连续重复字符的优化、匹配代码不变只换数组
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

// ======1. 先求next数组======
void GetNext(SString T, int next[]) {
    int i = 1, j = 0;
    next[1] = 0;
    while(i < T.length) {
        if(j == 0 || T.ch[i] == T.ch[j]) {
            i++; j++;
            next[i] = j;
        } else {
            j = next[j];
        }
    }
}

/*
 * ============================================================
 * 2. 由next求nextval（★★★★ 手算规则，必须会）
 *   - nextval[1] = 0（固定）
 *   - 对每个 j>=2：
 *       若 T[j] != T[next[j]]（跳到的位置字符不同，这次比较有意义）
 *            => nextval[j] = next[j]（保持不变）
 *       若 T[j] == T[next[j]]（字符相同，跳过去也必然失配，白比一次）
 *            => nextval[j] = nextval[next[j]]（沿链继续向前压缩）
 *   直观理解：失配处字符和跳转目标字符一样，那一跳注定失败，
 *            不如直接跳到"跳转目标再失配时该去的地方"。
 * ============================================================
 */
void GetNextVal(SString T, int next[], int nextval[]) {
    GetNext(T, next);                    // ★★★★ 先有next才能求nextval
    nextval[1] = 0;                      // ★★★★★ 固定
    for(int j = 2; j <= T.length; j++) {
        if(T.ch[j] == T.ch[next[j]]) {
            nextval[j] = nextval[next[j]];  // ★★★★★ 字符相同：继续向前压缩
        } else {
            nextval[j] = next[j];           // ★★★★ 字符不同：保留next原值
        }
    }
}

// ======3. 用nextval做KMP匹配（代码与普通KMP完全一致，只把next换成nextval）======
int Index_KMP_NextVal(SString S, SString T, int nextval[]) {
    int i = 1, j = 1;
    int cmpCount = 0;
    while(i <= S.length && j <= T.length) {
        cmpCount++;
        if(j == 0 || S.ch[i] == T.ch[j]) {
            i++; j++;
        } else {
            j = nextval[j];              // ★★★★ 唯一区别：失配时查nextval
        }
    }
    printf("       （nextval版KMP共比较 %d 次）\n", cmpCount);
    if(j > T.length)
        return i - T.length;
    return 0;
}

// ======打印某模式串的 字符/next/nextval 三行对照======
void ShowArrays(const char *str) {
    SString T;
    StrAssign(T, str);
    int next[MAXLEN], nextval[MAXLEN];
    GetNextVal(T, next, nextval);

    printf("       模式串：%s\n", str);
    printf("        下标 j ：");
    for(int j = 1; j <= T.length; j++) printf("%2d", j);
    printf("\n        字符 T[j]：");
    for(int j = 1; j <= T.length; j++) printf("%2c", T.ch[j]);
    printf("\n        next    ：");
    for(int j = 1; j <= T.length; j++) printf("%2d", next[j]);
    printf("\n        nextval ：");
    for(int j = 1; j <= T.length; j++) printf("%2d", nextval[j]);
    printf("\n\n");
}

// ======主函数：测试nextval的求法与匹配======
int main() {
    printf("====== KMP优化(nextval)测试开始 ======\n\n");

    printf("【测试1】视频例题 ababaa（next: 0 1 1 2 3 4 → nextval: 0 1 0 1 0 4）\n");
    ShowArrays("ababaa");

    printf("【测试2】连续重复 aaaab（next: 0 1 2 3 4 → nextval: 0 0 0 0 4，优化最明显）\n");
    ShowArrays("aaaab");

    printf("【测试3】视频例题 abaabc\n");
    ShowArrays("abaabc");

    // 用nextval实际匹配，对比连续重复串的比较次数
    printf("【测试4】用nextval匹配连续重复串，体会少做无意义比较\n");
    SString S, T;
    StrAssign(S, "aaaaaaaaaaab");
    StrAssign(T, "aaaab");
    int nv[MAXLEN], nxt[MAXLEN];
    GetNextVal(T, nxt, nv);
    int pos = Index_KMP_NextVal(S, T, nv);
    printf("       主串S=%s，模式串T=%s，起始位置=%d\n", S.ch + 1, T.ch + 1, pos);
    printf("       ★ 匹配逻辑和结果不变，只是减少了注定失败的比较次数\n");

    printf("\n====== nextval测试结束 ======\n");
    return 0;
}
