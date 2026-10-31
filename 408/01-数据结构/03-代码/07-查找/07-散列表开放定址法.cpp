/*
 * 文件名：07-散列表开放定址法.cpp
 * 章节：第7章 查找
 * 内容：散列表（开放定址法处理冲突）：线性探测、平方探测 + ASL计算
 * 重要度：★★★★★（全章最高频之一，线性探测手算画表+算ASL必会）
 * 考研考点：开放定址法公式、线性探测/平方探测、堆积问题、成功/失败ASL、逻辑删除
 */

#include <stdio.h>
#include <stdlib.h>

typedef int ElemType; // 元素类型

#define TABLE_SIZE 11   // 散列表长度（取质数）
#define NULL_KEY -32768 // 空位置标记

// ======1. 开放定址法散列表结构======
typedef struct {
    ElemType data[TABLE_SIZE]; // 散列表数组
    int count;                  // 已存入记录数
} OpenHashTable;

// ======2. 散列函数：除留余数法======
int Hash(ElemType key) {
    return key % TABLE_SIZE;   // H(key) = key mod p，p=11
}

// ======3. 初始化散列表======
void InitOpenHash(OpenHashTable &HT) {
    for(int i = 0; i < TABLE_SIZE; i++)
        HT.data[i] = NULL_KEY;  // 所有位置标记为空
    HT.count = 0;
}

// ======4. 线性探测法插入（★★★★★ 最常考）======
// 冲突后依次检查下一个位置：d_i = 0, 1, 2, 3, ...
bool LinearProbe_Insert(OpenHashTable &HT, ElemType key) {
    if(HT.count >= TABLE_SIZE) return false;  // 表满，无法插入
    int addr = Hash(key);
    int i = 0;
    while(i < TABLE_SIZE) {
        int probe = (addr + i) % TABLE_SIZE;  // ★ H_i = (H(key) + d_i) mod m
        if(HT.data[probe] == NULL_KEY) {       // 找到空位置
            HT.data[probe] = key;
            HT.count++;
            return true;
        }
        if(HT.data[probe] == key) return false; // 已存在
        i++;  // 冲突，继续探测下一个位置
    }
    return false;  // 表满
}

/*
 * ★ 线性探测法的问题：堆积（Primary Clustering）
 * 本来不冲突的关键字，因为前面的位置被占了，也被迫往后移，
 * 导致大量关键字聚集在一起，降低查找效率。
 */

// ======5. 线性探测法查找======
// 返回找到的位置，失败返回-1；同时返回比较次数
int LinearProbe_Search(OpenHashTable HT, ElemType key, int &compareCount) {
    int addr = Hash(key);
    int i = 0;
    compareCount = 0;
    while(i < TABLE_SIZE) {
        int probe = (addr + i) % TABLE_SIZE;
        compareCount++;
        if(HT.data[probe] == key) return probe;   // 找到
        if(HT.data[probe] == NULL_KEY) return -1; // ★ 遇到空位置，查找失败（停止）
        i++;
    }
    return -1;  // 查完整个表都没找到
}

// ======6. 平方探测法插入（★★★ 常考）======
// 冲突后检查 +1, -1, +4, -4, +9, -9, ... 的位置
// d_i = 0^2, 1^2, -1^2, 2^2, -2^2, ...
bool QuadraticProbe_Insert(OpenHashTable &HT, ElemType key) {
    if(HT.count >= TABLE_SIZE) return false;
    int addr = Hash(key);
    // 先检查初始位置
    if(HT.data[addr] == NULL_KEY) {
        HT.data[addr] = key;
        HT.count++;
        return true;
    }
    if(HT.data[addr] == key) return false;
    // 平方探测：k=1,2,3,... 检查 +k^2 和 -k^2
    for(int k = 1; k <= TABLE_SIZE / 2; k++) {
        int offset = k * k;
        // 检查 +k^2
        int probe1 = (addr + offset) % TABLE_SIZE;
        if(HT.data[probe1] == NULL_KEY) {
            HT.data[probe1] = key;
            HT.count++;
            return true;
        }
        if(HT.data[probe1] == key) return false;
        // 检查 -k^2（注意取模时可能为负数，需要处理）
        int probe2 = (addr - offset) % TABLE_SIZE;
        if(probe2 < 0) probe2 += TABLE_SIZE;
        if(HT.data[probe2] == NULL_KEY) {
            HT.data[probe2] = key;
            HT.count++;
            return true;
        }
        if(HT.data[probe2] == key) return false;
    }
    return false;  // 平方探测不能探测到所有位置，可能插入失败
}

/*
 * ★ 平方探测法的特点：
 * 1. 可以避免堆积（不会出现线性探测那种连续聚集）
 * 2. 但不能探测到表中所有位置（只能探测到约一半的位置）
 * 3. 表长m必须是4k+3形式的质数，才能保证探测到所有位置（了解即可）
 */

// ======7. 打印散列表======
void PrintOpenHash(OpenHashTable HT, const char *method) {
    printf("       散列表（%s，表长=%d，已存%d个，α=%.2f）：\n",
           method, TABLE_SIZE, HT.count, (float)HT.count / TABLE_SIZE);
    printf("         下标：");
    for(int i = 0; i < TABLE_SIZE; i++) printf("%3d ", i);
    printf("\n         数据：");
    for(int i = 0; i < TABLE_SIZE; i++) {
        if(HT.data[i] == NULL_KEY)
            printf("  - ");
        else
            printf("%3d ", HT.data[i]);
    }
    printf("\n");
}

// ======8. 计算线性探测法成功ASL======
float Calc_Linear_Success_ASL(OpenHashTable HT) {
    int totalCompare = 0;
    int totalCount = 0;
    for(int i = 0; i < TABLE_SIZE; i++) {
        if(HT.data[i] == NULL_KEY) continue;
        int addr = Hash(HT.data[i]);
        int compare = 0;
        int j = 0;
        while(j < TABLE_SIZE) {
            int probe = (addr + j) % TABLE_SIZE;
            compare++;
            if(HT.data[probe] == HT.data[i]) break;
            j++;
        }
        totalCompare += compare;
        totalCount++;
    }
    if(totalCount == 0) return 0;
    return (float)totalCompare / totalCount;
}

// ======9. 计算线性探测法失败ASL======
// 对每个散列地址，从该地址开始到遇到第一个空位置的比较次数
float Calc_Linear_Fail_ASL(OpenHashTable HT) {
    int totalCompare = 0;
    for(int addr = 0; addr < TABLE_SIZE; addr++) {
        int compare = 0;
        int j = 0;
        while(j < TABLE_SIZE) {
            int probe = (addr + j) % TABLE_SIZE;
            compare++;
            if(HT.data[probe] == NULL_KEY) break;  // ★ 遇到空位置停止
            j++;
        }
        totalCompare += compare;
    }
    return (float)totalCompare / TABLE_SIZE;  // 分母是表长m
}

// ======10. 主函数测试======
int main() {
    printf("====== 散列表开放定址法测试开始 ======\n\n");

    // 【测试1】线性探测法构造
    printf("【测试1】线性探测法构造（H(key)=key mod 11，表长11）\n");
    OpenHashTable HT1;
    InitOpenHash(HT1);
    ElemType keys[] = {47, 7, 29, 11, 16, 92, 22, 8, 3};
    int n = 9;
    printf("       插入序列：");
    for(int i = 0; i < n; i++) printf("%d ", keys[i]);
    printf("\n");
    for(int i = 0; i < n; i++) {
        LinearProbe_Insert(HT1, keys[i]);
    }
    PrintOpenHash(HT1, "线性探测");

    // 【测试2】线性探测过程演示
    printf("\n【测试2】线性探测过程演示（插入29）\n");
    printf("       H(29) = 29 mod 11 = 7\n");
    printf("       地址7为空？是 → 直接插入地址7\n");
    printf("       插入92：H(92)=92 mod 11=4，地址4为空 → 直接插入\n");
    printf("       插入22：H(22)=22 mod 11=0，地址0为空 → 直接插入\n");
    printf("       插入8：H(8)=8 mod 11=8，地址8被29占了 → 冲突！\n");
    printf("         探测地址9：空 → 插入地址9（比较了2次）\n");
    printf("       插入3：H(3)=3 mod 11=3，地址3为空 → 直接插入\n");

    // 【测试3】查找与比较次数
    printf("\n【测试3】查找与比较次数\n");
    int cmp;
    int pos1 = LinearProbe_Search(HT1, 8, cmp);
    printf("       查找8：位置=%d，比较次数=%d（期望2次：地址8冲突→地址9找到）\n", pos1, cmp);
    int pos2 = LinearProbe_Search(HT1, 47, cmp);
    printf("       查找47：位置=%d，比较次数=%d（期望1次，直接命中）\n", pos2, cmp);
    int pos3 = LinearProbe_Search(HT1, 100, cmp);
    printf("       查找100：位置=%d，比较次数=%d（期望失败，遇到空位置停止）\n", pos3, cmp);

    // 【测试4】ASL计算
    printf("\n【测试4】ASL计算（线性探测）\n");
    float asl_succ = Calc_Linear_Success_ASL(HT1);
    float asl_fail = Calc_Linear_Fail_ASL(HT1);
    printf("       成功ASL = %.2f（分母是关键字数n=%d）\n", asl_succ, HT1.count);
    printf("       失败ASL = %.2f（分母是表长m=%d）\n", asl_fail, TABLE_SIZE);
    printf("       装填因子α = n/m = %d/%d = %.2f\n", HT1.count, TABLE_SIZE, (float)HT1.count/TABLE_SIZE);

    // 【测试5】平方探测法对比
    printf("\n【测试5】平方探测法对比（同样的插入序列）\n");
    OpenHashTable HT2;
    InitOpenHash(HT2);
    for(int i = 0; i < n; i++) {
        QuadraticProbe_Insert(HT2, keys[i]);
    }
    PrintOpenHash(HT2, "平方探测");
    printf("       平方探测可以避免堆积，但不能探测到所有位置\n");
    printf("       线性探测容易产生堆积（连续聚集），但能探测到所有位置\n");

    // 【测试6】开放定址法的删除问题
    printf("\n【测试6】开放定址法的删除问题（★ 易错点）\n");
    printf("       不能简单物理删除！因为删除后留空位置，\n");
    printf("       会导致后面因冲突而放在后面的关键字无法被找到\n");
    printf("       （查找时遇到空位置就停止了）\n");
    printf("       正确做法：逻辑删除（加删除标记）或重新散列(Rehash)\n");

    // 【测试7】三种冲突处理方法对比
    printf("\n【测试7】冲突处理方法对比（★ 记结论）\n");
    printf("       | 方法     | 堆积 | α范围   | 删除 | 适用场景       |\n");
    printf("       |----------|------|---------|------|----------------|\n");
    printf("       | 拉链法   | 无   | 可>1    | 简单 | 经常插入删除   |\n");
    printf("       | 线性探测 | 有   | 必须<1  | 复杂 | 记录少、不常删 |\n");
    printf("       | 平方探测 | 无   | 必须<1  | 复杂 | 避免堆积       |\n");

    printf("\n====== 散列表开放定址法测试结束 ======\n");
    return 0;
}
