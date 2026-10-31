/*
 * ============================================================
 * 文件名：01-串的顺序存储与基本操作.cpp
 * 章  节：第四章 串
 * 内  容：定长顺序存储串的基本操作（王道约定：废弃ch[0]，字符从ch[1]开始存）
 * 重要度：★★★ 理解掌握（本章基础，模式匹配的前置知识）
 * 考研考点：串的三种记长度方式、求子串、串的比较规则、定位操作Index
 * ============================================================
 */

#include <stdio.h>
#include <string.h>

#define MAXLEN 255  // ★★★ 定长顺序串的最大容量（注意：若用ch[0]记长度则不能超过255）

/*
 * ============================================================
 * 王道定长顺序串结构体（★★★ 必须记住这个约定）：
 *   - ch[0] 故意废弃不用，字符从 ch[1] 开始存放
 *   - 这样字符的"位序"和数组下标只差固定的1，便于理解
 *   - 另设一个 int 型 length 记录串长，不受 char 只能到255 的限制
 * 对比另外两种记长度方式（选择题考点）：
 *   方案二：用 ch[0] 存长度 —— 位序与下标一一对应，但串长≤255
 *   方案三：末尾存 '\0'     —— 不记长度，但求串长要从头扫描 O(n)
 * ============================================================
 */
typedef struct {
    char ch[MAXLEN];  // ★★★ 静态字符数组，ch[0]废弃，字符从ch[1]开始
    int length;       // ★★★ 串的实际长度（当前有多少个字符）
} SString;

// ======1. 赋值操作：把C风格字符串chars赋给串S======
void StrAssign(SString &S, const char chars[]) {
    int len = (int)strlen(chars);           // 求C字符串长度
    for(int i = 1; i <= len; i++) {
        S.ch[i] = chars[i - 1];             // ★★★ chars从下标0开始，S从下标1开始，错位1
    }
    S.ch[len + 1] = '\0';                    // 末尾补'\0'：便于用%s打印，并清除上次赋值的残留字符
    S.length = len;                         // ★★★ 别忘了更新串长
}

// ======2. 求串长======
int StrLength(SString S) {
    return S.length;                        // ★★★ 直接返回length，时间O(1)
}

// ======3. 判空操作======
bool StrEmpty(SString S) {
    return S.length == 0;                   // ★★★ 长度为0即空串
}

// ======4. 清空操作（逻辑清空，不释放内存，区别于销毁）======
void ClearString(SString &S) {
    S.length = 0;                           // ★★★ 只把长度置0，数组里的旧数据还在但逻辑上不可见
}

// ======5. 串的比较：S>T返回正值，相等返回0，S<T返回负值======
int StrCompare(SString S, SString T) {
    /*
     * ★★★★ 比较规则（选择题常考）：
     * 1. 从第1个字符开始逐个比，遇到第一对不相等的字符，
     *    谁的字符大（比ASCII编码）谁所在的串就大，后面不再看；
     * 2. 若前面字符全相同，则更长的串更大；
     * 3. 字符全同且长度相等才相等。
     */
    for(int i = 1; i <= S.length && i <= T.length; i++) {
        if(S.ch[i] != T.ch[i]) {
            return S.ch[i] - T.ch[i];       // ★★★★ 第一对不同字符相减，自然得到正/负值
        }
    }
    return S.length - T.length;             // ★★★★ 前缀全同，用长度差决定（正/0/负）
}

// ======6. 求子串：用Sub返回S中从第pos个字符起、长度为len的子串======
bool SubString(SString &Sub, SString S, int pos, int len) {
    if(pos < 1 || pos > S.length) {         // ★★★ 起始位置非法
        return false;
    }
    if(len < 0 || pos + len - 1 > S.length) {  // ★★★★ 子串范围越界检查（必写）
        return false;
    }
    for(int i = pos; i < pos + len; i++) {
        Sub.ch[i - pos + 1] = S.ch[i];      // ★★★ 复制到Sub，仍从下标1开始
    }
    Sub.length = len;                       // ★★★ 设置子串长度
    return true;
}

// ======7. 串的连接：用T返回 S1 接上 S2 的结果======
bool Concat(SString &T, SString S1, SString S2) {
    if(S1.length + S2.length > MAXLEN - 1) {  // 连接后超长（ch[0]不用，最多MAXLEN-1）
        return false;
    }
    for(int i = 1; i <= S1.length; i++) {
        T.ch[i] = S1.ch[i];                 // 先复制S1
    }
    for(int i = 1; i <= S2.length; i++) {
        T.ch[S1.length + i] = S2.ch[i];     // ★★★ S2紧接在S1后面
    }
    T.length = S1.length + S2.length;
    return true;
}

// ======8. 定位操作Index：返回子串T在主串S中第一次出现的位置，找不到返回0======
int Index(SString S, SString T) {
    /*
     * ★★★ 思路（本质就是朴素模式匹配，只是复用了"求子串+比较"两个基本操作）：
     * 主串长度n、模式串长度m，共有 n-m+1 个候选子串，逐个取出来与T比较。
     */
    int n = StrLength(S);
    int m = StrLength(T);
    if(m == 0 || m > n) {
        return 0;                           // 空串或模式串比主串还长，无意义
    }
    SString sub;
    for(int i = 1; i + m - 1 <= n; i++) {   // ★★★ i是候选子串起点，最多到 n-m+1
        SubString(sub, S, i, m);            // 取出从i起、长度为m的子串
        if(StrCompare(sub, T) == 0) {       // ★★★ 与模式串相等
            return i;                       // 返回该子串起始位置
        }
    }
    return 0;                               // 全部不匹配，返回0
}

// ======9. 打印串（连同长度一起显示）======
void PrintString(SString S) {
    printf("\"");
    for(int i = 1; i <= S.length; i++) {
        printf("%c", S.ch[i]);
    }
    printf("\"（长度=%d）", S.length);
}

// ======主函数：测试顺序串的所有基本操作======
int main() {
    SString S, T, Sub, Cat;

    printf("====== 顺序串基本操作测试开始 ======\n\n");

    // 测试1：赋值、求长、判空
    printf("【测试1】赋值与判空\n");
    StrAssign(S, "wangdao");
    printf("       串S = "); PrintString(S); printf("\n");
    StrAssign(T, "");
    printf("       空串T = "); PrintString(T);
    printf("，是否为空：%s\n", StrEmpty(T) ? "是" : "否");

    // 测试2：串的比较
    printf("\n【测试2】串的比较（返回正=前者大，0=相等，负=前者小）\n");
    SString A, B, C;
    StrAssign(A, "abandon");
    StrAssign(B, "aboard");
    StrAssign(C, "abandon");
    printf("       abandon vs aboard = %d（负，因第3个字符 a<o）\n", StrCompare(A, B));
    printf("       abandon vs abandon = %d（相等）\n", StrCompare(A, C));
    printf("       aboard  vs abandon = %d（正）\n", StrCompare(B, A));

    // 测试3：求子串
    printf("\n【测试3】求子串\n");
    StrAssign(S, "abcdefg");
    printf("       原串S = "); PrintString(S); printf("\n");
    if(SubString(Sub, S, 4, 3)) {           // 从第4个字符起取3个
        printf("       从第4位起取3个字符 = "); PrintString(Sub); printf("\n");
    }
    printf("       越界测试（从第6位取4个）：%s\n",
           SubString(Sub, S, 6, 4) ? "成功(异常!)" : "失败(正确，越界)");

    // 测试4：连接
    printf("\n【测试4】串的连接\n");
    SString S1, S2;
    StrAssign(S1, "Hello ");
    StrAssign(S2, "World");
    Concat(Cat, S1, S2);
    printf("       \"Hello \" + \"World\" = "); PrintString(Cat); printf("\n");

    // 测试5：定位操作Index
    printf("\n【测试5】定位操作Index\n");
    StrAssign(S, "this is a test string");
    StrAssign(T, "test");
    printf("       主串S = "); PrintString(S); printf("\n");
    printf("       子串\"test\"第一次出现的位置 = %d\n", Index(S, T));
    StrAssign(T, "xyz");
    printf("       子串\"xyz\"的位置 = %d（0表示不存在）\n", Index(S, T));

    // 测试6：清空
    printf("\n【测试6】清空操作\n");
    ClearString(Cat);
    printf("       清空后 Cat = "); PrintString(Cat);
    printf("，是否为空：%s\n", StrEmpty(Cat) ? "是" : "否");

    printf("\n====== 顺序串基本操作测试结束 ======\n");
    return 0;
}
