/*
 * 文件名：03-分块查找.cpp
 * 章节：第7章 查找
 * 内容：分块查找（索引顺序查找）+ ASL计算
 * 重要度：★★★（理解+会算ASL，选择题常考）
 * 考研考点：分块查找结构、索引表+块内查找、ASL计算、最优块大小
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef int ElemType; // 元素类型

#define MAX_BLOCK 10   // 最大块数
#define BLOCK_SIZE 5   // 每块大小

// ======1. 索引项结构======
typedef struct {
    ElemType maxKey;  // 块内最大关键字
    int start;        // 块的起始位置（在原表中的下标）
} IndexItem;

// ======2. 分块查找表结构======
typedef struct {
    ElemType *data;       // 数据区（所有块连续存放）
    int length;           // 总元素数
    IndexItem index[MAX_BLOCK]; // 索引表
    int blockNum;         // 块数
} BlockTable;

// ======3. 初始化分块查找表（手动构造，演示用）======
// 数据：块内无序、块间有序（第i块所有元素 < 第i+1块所有元素）
bool InitBlockTable(BlockTable &BT) {
    // 共15个元素，分3块，每块5个
    BT.length = 15;
    BT.blockNum = 3;
    BT.data = (ElemType *)malloc(sizeof(ElemType) * BT.length);
    if(BT.data == NULL) return false;

    // 第1块（下标0~4）：最大关键字20，块内无序
    BT.data[0] = 12; BT.data[1] = 8;  BT.data[2] = 20; BT.data[3] = 5;  BT.data[4] = 15;
    // 第2块（下标5~9）：最大关键字45，块内无序
    BT.data[5] = 35; BT.data[6] = 25; BT.data[7] = 45; BT.data[8] = 30; BT.data[9] = 40;
    // 第3块（下标10~14）：最大关键字80，块内无序
    BT.data[10] = 60; BT.data[11] = 55; BT.data[12] = 80; BT.data[13] = 70; BT.data[14] = 65;

    // 索引表：每块的最大关键字和起始位置
    BT.index[0].maxKey = 20; BT.index[0].start = 0;
    BT.index[1].maxKey = 45; BT.index[1].start = 5;
    BT.index[2].maxKey = 80; BT.index[2].start = 10;

    return true;
}

// ======4. 分块查找（索引表用顺序查找，块内用顺序查找）======
int BlockSearch(BlockTable BT, ElemType key) {
    // 第一步：查索引表（顺序查找，确定在哪一块）
    int blockIdx = -1;
    for(int i = 0; i < BT.blockNum; i++) {
        if(key <= BT.index[i].maxKey) {   // ★ 目标关键字 <= 该块最大关键字，说明在该块内
            blockIdx = i;
            break;
        }
    }
    if(blockIdx == -1) return -1;          // 超过所有块的最大关键字，查找失败

    // 第二步：在块内顺序查找
    int start = BT.index[blockIdx].start;
    int end = (blockIdx == BT.blockNum - 1) ? BT.length : BT.index[blockIdx + 1].start;
    for(int i = start; i < end; i++) {
        if(BT.data[i] == key)
            return i;                        // 查找成功，返回在原表中的下标
    }
    return -1;                               // 块内未找到，查找失败
}

// ======5. 分块查找的ASL计算（索引表顺序查找 + 块内顺序查找）======
// 设块数b，每块s个元素，总n=b*s
// ASL = ASL索引 + ASL块内 = (b+1)/2 + (s+1)/2 = (b+s)/2 + 1
float Calc_Block_ASL(int b, int s) {
    float asl_index = (float)(b + 1) / 2;  // 索引表顺序查找的成功ASL
    float asl_block = (float)(s + 1) / 2;  // 块内顺序查找的成功ASL
    return asl_index + asl_block;
}

// ======6. 主函数测试======
int main() {
    printf("====== 分块查找测试开始 ======\n\n");

    // 【测试1】分块查找基本功能
    printf("【测试1】分块查找基本功能（3块，每块5个，共15个元素）\n");
    BlockTable BT;
    InitBlockTable(BT);

    printf("       索引表：\n");
    for(int i = 0; i < BT.blockNum; i++) {
        printf("         块%d：maxKey=%d，start=%d\n", i+1, BT.index[i].maxKey, BT.index[i].start);
    }
    printf("       数据区：");
    for(int i = 0; i < BT.length; i++) printf("%d ", BT.data[i]);
    printf("\n");

    int r1 = BlockSearch(BT, 45);
    printf("       查找45：下标=%d（期望7，第2块）\n", r1);
    int r2 = BlockSearch(BT, 80);
    printf("       查找80：下标=%d（期望12，第3块）\n", r2);
    int r3 = BlockSearch(BT, 5);
    printf("       查找5：下标=%d（期望3，第1块）\n", r3);
    int r4 = BlockSearch(BT, 100);
    printf("       查找100：下标=%d（期望-1，不存在）\n", r4);
    int r5 = BlockSearch(BT, 25);
    printf("       查找25：下标=%d（期望6，第2块）\n", r5);

    // 【测试2】ASL计算
    printf("\n【测试2】分块查找ASL计算\n");
    int b = 3, s = 5;
    float asl = Calc_Block_ASL(b, s);
    printf("       块数b=%d，每块s=%d，总n=%d\n", b, s, b*s);
    printf("       ASL索引 = (b+1)/2 = (%d+1)/2 = %.2f\n", b, (float)(b+1)/2);
    printf("       ASL块内 = (s+1)/2 = (%d+1)/2 = %.2f\n", s, (float)(s+1)/2);
    printf("       ASL总分块 = %.2f + %.2f = %.2f\n", (float)(b+1)/2, (float)(s+1)/2, asl);
    printf("       公式：ASL = (b+s)/2 + 1 = (%d+%d)/2 + 1 = %.2f\n", b, s, (float)(b+s)/2 + 1);

    // 【测试3】最优块大小
    printf("\n【测试3】最优块大小（n固定时，s=sqrt(n)时ASL最小）\n");
    int n = 100;
    printf("       n=%d时，不同块大小的ASL：\n", n);
    for(int s_test = 2; s_test <= 20; s_test += 2) {
        int b_test = (n + s_test - 1) / s_test;  // 向上取整
        float asl_test = (float)(b_test + s_test) / 2 + 1;
        printf("         s=%d, b=%d, ASL=%.2f\n", s_test, b_test, asl_test);
    }
    printf("       当s=sqrt(n)=%d时，ASL最小≈%.2f\n", (int)sqrt(n), (float)(2*sqrt(n))/2 + 1);

    // 【测试4】分块查找的特点
    printf("\n【测试4】分块查找的特点\n");
    printf("       1. 块内无序、块间有序（核心特征）\n");
    printf("       2. 查找效率介于顺序查找O(n)和折半查找O(log n)之间\n");
    printf("       3. 插入删除只需在块内操作（块内无序，方便），适合动态查找表\n");
    printf("       4. 索引表本身有序，可以用折半查找（此时ASL=ceil(log2(b+1))+(s+1)/2）\n");

    free(BT.data);
    printf("\n====== 分块查找测试结束 ======\n");
    return 0;
}
