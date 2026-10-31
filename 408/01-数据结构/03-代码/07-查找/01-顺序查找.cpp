/*
 * 文件名：01-顺序查找.cpp
 * 章节：第7章 查找
 * 内容：顺序查找（普通写法 + 带哨兵写法 + ASL计算）
 * 重要度：★★★（理解+会算ASL，选择题高频）
 * 考研考点：哨兵的作用、成功/失败ASL计算、有序表顺序查找
 */

#include <stdio.h>
#include <stdlib.h>

typedef int ElemType; // 元素类型

// ======1. 静态查找表结构（教材口径：下标1~n存元素，0号位置留作哨兵）======
typedef struct {
    ElemType *element; // element[1]~element[n] 存有效元素，element[0] 是哨兵
    int length;        // 查找表长度（有效元素个数）
} SSTable;

// ======2. 初始化查找表（分配n+1个空间，下标0~n）======
bool InitTable(SSTable &ST, int n) {
    ST.element = (ElemType *)malloc(sizeof(ElemType) * (n + 1)); // ★ 多分配一个位置给哨兵
    if(ST.element == NULL)
        return false;
    ST.length = n;
    return true;
}

// ======3. 普通顺序查找（从下标0开始，无哨兵）======
int SequentialSearch_Normal(ElemType data[], int length, ElemType key) {
    for(int i = 0; i < length; i++) {       // 从第一个元素开始逐个比较
        if(data[i] == key)
            return i;                          // 查找成功，返回下标
    }
    return -1;                                 // 扫描结束仍未找到，返回-1
}

// ======4. 带哨兵的顺序查找（★ 教材标准写法，下标1~n）======
int Search_Seq(SSTable ST, ElemType key) {
    ST.element[0] = key;                       // ★★★ 哨兵：把目标关键字放到0号位置
    int i;
    // 从后往前找，无需判断 i>=1（因为element[0]=key，一定会在某个位置匹配）
    for(i = ST.length; ST.element[i] != key; i--);
    return i;                                   // 返回0表示查找失败（哨兵位置），否则返回元素位置(1~n)
}

/*
 * ★ 哨兵的作用（选择题高频）：
 * 免去查找过程中每一步都要检测整个表是否查找完毕（即不用写 i>=1 的边界判断），
 * 从而提高查找效率约一倍。
 * - 查找成功：返回元素所在位置（1~n）
 * - 查找失败：返回 0（哨兵位置）
 */

// ======5. 计算成功ASL（等概率，从后往前找）======
float Calc_Success_ASL(SSTable ST) {
    // 从后往前找：第i个位置（从后数第i个）需要比较i次
    // 第1个位置（最前面）需要比较n次，第n个位置（最后面）需要比较1次
    // 总比较次数 = 1+2+...+n = n(n+1)/2
    // 平均 = (n+1)/2
    int total = 0;
    for(int i = 1; i <= ST.length; i++) {
        total += (ST.length - i + 1);          // 第i个位置需要比较 (n-i+1) 次
    }
    return (float)total / ST.length;            // 等概率，除以n
}

// ======6. 主函数测试======
int main() {
    printf("====== 顺序查找测试开始 ======\n\n");

    // 【测试1】普通顺序查找
    printf("【测试1】普通顺序查找（无哨兵，下标0开始）\n");
    ElemType arr[] = {23, 45, 12, 67, 89, 34, 56};
    int n = 7;
    printf("       查找表：");
    for(int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
    int pos1 = SequentialSearch_Normal(arr, n, 67);
    printf("       查找67：下标=%d（期望3）\n", pos1);
    int pos2 = SequentialSearch_Normal(arr, n, 99);
    printf("       查找99：下标=%d（期望-1，不存在）\n", pos2);

    // 【测试2】带哨兵的顺序查找
    printf("\n【测试2】带哨兵的顺序查找（下标1~n，0号位置是哨兵）\n");
    SSTable ST;
    InitTable(ST, 6);
    ST.element[1] = 10;
    ST.element[2] = 20;
    ST.element[3] = 30;
    ST.element[4] = 40;
    ST.element[5] = 50;
    ST.element[6] = 60;
    printf("       查找表（下标1~6）：");
    for(int i = 1; i <= 6; i++) printf("%d ", ST.element[i]);
    printf("\n");

    int r1 = Search_Seq(ST, 40);
    printf("       查找40：位置=%d（期望4）\n", r1);
    int r2 = Search_Seq(ST, 10);
    printf("       查找10：位置=%d（期望1，第一个元素）\n", r2);
    int r3 = Search_Seq(ST, 60);
    printf("       查找60：位置=%d（期望6，最后一个元素）\n", r3);
    int r4 = Search_Seq(ST, 99);
    printf("       查找99：位置=%d（期望0，查找失败=哨兵位置）\n", r4);

    // 【测试3】成功ASL计算
    printf("\n【测试3】成功ASL计算（等概率，从后往前找）\n");
    float asl = Calc_Success_ASL(ST);
    printf("       n=%d，成功ASL=%.2f（期望(6+1)/2=3.50）\n", ST.length, asl);
    printf("       公式：ASL成功 = (n+1)/2 = (6+1)/2 = 3.5\n");

    // 【测试4】失败ASL说明
    printf("\n【测试4】失败ASL说明\n");
    printf("       带哨兵：失败时比较 n+1 = %d 次（n个元素 + 1次哨兵匹配）\n", ST.length + 1);
    printf("       不带哨兵：失败时比较 n = %d 次\n", ST.length);
    printf("       （按题目口径计算，考研题通常会明确说明是否带哨兵）\n");

    free(ST.element); // 释放内存
    printf("\n====== 顺序查找测试结束 ======\n");
    return 0;
}
