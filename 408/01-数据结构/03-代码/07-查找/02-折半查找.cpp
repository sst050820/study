/*
 * 文件名：02-折半查找.cpp
 * 章节：第7章 查找
 * 内容：折半查找（二分查找）核心代码 + 查找判定树 + ASL计算
 * 重要度：★★★★★（全章最高频之一，判定树和ASL是选择题必考点）
 * 考研考点：折半查找代码、判定树构造、成功/失败ASL、复杂度分析
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef int ElemType; // 元素类型

// ======1. 有序顺序表结构（下标1~n存元素）======
typedef struct {
    ElemType *element; // element[1]~element[n] 存有序元素
    int length;        // 表长
} SSTable;

// ======2. 初始化（分配n+1个空间）======
bool InitTable(SSTable &ST, int n) {
    ST.element = (ElemType *)malloc(sizeof(ElemType) * (n + 1));
    if(ST.element == NULL) return false;
    ST.length = n;
    return true;
}

// ======3. 折半查找核心代码（★★★★★ 能默写）======
int Binary_Search(SSTable L, ElemType key) {
    int low = 1, high = L.length, mid;  // low指向区间左端，high指向右端
    while(low <= high) {                  // ★ 循环条件是 low<=high，不是 low<high
        mid = (low + high) / 2;           // ★★★ 向下取整（C语言整数除法自动向下取整）
        if(L.element[mid] == key)
            return mid;                     // 查找成功，返回位置
        else if(L.element[mid] > key)
            high = mid - 1;                // ★ 目标在左半区，high=mid-1（不是mid）
        else
            low = mid + 1;                 // ★ 目标在右半区，low=mid+1（不是mid）
    }
    return 0;                               // 查找失败（low>high）
}

/*
 * ★ 易错点总结：
 * 1. mid = (low+high)/2 是向下取整
 * 2. 循环条件 low <= high（否则漏掉最后一个元素）
 * 3. 更新边界 high=mid-1, low=mid+1（否则会死循环）
 * 4. 只适用于有序顺序表（链表不能随机访问，无序表不满足单调性）
 */

// ======4. 递归版折半查找（了解）======
int Binary_Search_Recur(SSTable L, ElemType key, int low, int high) {
    if(low > high) return 0;               // 查找失败
    int mid = (low + high) / 2;
    if(L.element[mid] == key) return mid;
    else if(L.element[mid] > key)
        return Binary_Search_Recur(L, key, low, mid - 1);
    else
        return Binary_Search_Recur(L, key, mid + 1, high);
}

// ======5. 计算折半查找的成功ASL（基于判定树，按层统计）======
// 对于n个元素的判定树，第h层有2^(h-1)个节点（最后一层可能不满）
// 每个第h层节点需要比较h次
float Calc_Binary_Success_ASL(int n) {
    if(n <= 0) return 0;
    int total = 0;   // 总比较次数
    int count = 0;   // 已统计节点数
    int level = 1;   // 当前层（从第1层开始）
    while(count < n) {
        int level_count = 1 << (level - 1);  // 该层最多节点数 = 2^(level-1)
        if(count + level_count > n)
            level_count = n - count;           // 最后一层可能不满
        total += level_count * level;          // 该层节点数 × 该层比较次数
        count += level_count;
        level++;
    }
    return (float)total / n;                    // 等概率，除以n
}

// ======6. 判定树高度计算======
int Calc_Decision_Tree_Height(int n) {
    // n个元素的判定树高度 = floor(log2(n)) + 1 = ceil(log2(n+1))
    if(n <= 0) return 0;
    int h = 0;
    int temp = n;
    while(temp > 0) {
        temp /= 2;
        h++;
    }
    return h;  // = floor(log2(n)) + 1
}

// ======7. 主函数测试======
int main() {
    printf("====== 折半查找测试开始 ======\n\n");

    // 【测试1】折半查找基本功能
    printf("【测试1】折半查找基本功能\n");
    SSTable ST;
    InitTable(ST, 11);
    // 有序表：下标1~11
    int vals[] = {0, 7, 10, 13, 16, 19, 29, 32, 37, 41, 43, 76}; // vals[0]不用
    for(int i = 1; i <= 11; i++) ST.element[i] = vals[i];
    printf("       有序表（下标1~11）：");
    for(int i = 1; i <= 11; i++) printf("%d ", ST.element[i]);
    printf("\n");

    int r1 = Binary_Search(ST, 33);
    printf("       查找33：位置=%d（期望0，不存在）\n", r1);
    int r2 = Binary_Search(ST, 29);
    printf("       查找29：位置=%d（期望6）\n", r2);
    int r3 = Binary_Search(ST, 76);
    printf("       查找76：位置=%d（期望11，最后一个）\n", r3);
    int r4 = Binary_Search(ST, 7);
    printf("       查找7：位置=%d（期望1，第一个）\n", r4);

    // 【测试2】查找过程演示（查找33）
    printf("\n【测试2】查找33的过程演示（low/high/mid变化）\n");
    int low = 1, high = 11, mid;
    printf("       初始：low=1, high=11\n");
    while(low <= high) {
        mid = (low + high) / 2;
        printf("       mid=(%d+%d)/2=%d，element[%d]=%d，", low, high, mid, mid, ST.element[mid]);
        if(ST.element[mid] == 33) {
            printf("找到！\n");
            break;
        } else if(ST.element[mid] > 33) {
            printf("33<%d，目标在左，high=%d\n", ST.element[mid], mid - 1);
            high = mid - 1;
        } else {
            printf("33>%d，目标在右，low=%d\n", ST.element[mid], mid + 1);
            low = mid + 1;
        }
    }
    printf("       最终low=%d > high=%d，查找失败（比较了4次）\n", low, high);

    // 【测试3】判定树与ASL
    printf("\n【测试3】判定树与ASL计算（n=11）\n");
    int h = Calc_Decision_Tree_Height(11);
    printf("       判定树高度 = %d（期望4，=floor(log2(11))+1=3+1=4）\n", h);
    float asl_succ = Calc_Binary_Success_ASL(11);
    printf("       成功ASL = %.2f（期望3.00）\n", asl_succ);
    printf("       手算：第1层1个×1 + 第2层2个×2 + 第3层4个×3 + 第4层4个×4 = 1+4+12+16=33\n");
    printf("       33/11 = 3.00\n");

    // 【测试4】失败ASL（n=11，失败节点数=n+1=12）
    printf("\n【测试4】失败ASL（n=11，失败节点数=12）\n");
    printf("       判定树有4层，失败节点分布：第3层下方4个（比较3次），第4层下方8个（比较4次）\n");
    printf("       失败ASL = (4×3 + 8×4) / 12 = (12+32)/12 = 44/12 = %.2f\n", 44.0/12);
    printf("       （失败ASL的分母是失败节点数n+1=12，不是n=11）\n");

    // 【测试5】复杂度分析
    printf("\n【测试5】复杂度分析\n");
    printf("       时间复杂度：O(log2 n) = O(log n)（判定树高度）\n");
    printf("       空间复杂度：O(1)（只需三个指针low/high/mid）\n");
    printf("       适用条件：有序 + 顺序存储（链表不能用，无序表不能用）\n");

    free(ST.element);
    printf("\n====== 折半查找测试结束 ======\n");
    return 0;
}
