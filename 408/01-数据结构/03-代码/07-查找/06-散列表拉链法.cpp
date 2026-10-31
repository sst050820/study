/*
 * 文件名：06-散列表拉链法.cpp
 * 章节：第7章 查找
 * 内容：散列表（拉链法处理冲突）的构造、查找、插入、ASL计算
 * 重要度：★★★★★（全章最高频之一，手算画表+算ASL必会）
 * 考研考点：散列函数、拉链法结构、查找插入、成功/失败ASL、装填因子
 */

#include <stdio.h>
#include <stdlib.h>

typedef int ElemType; // 元素类型

#define TABLE_SIZE 13   // 散列表长度（取质数，减少冲突）

// ======1. 链表节点结构（每个散列地址对应一个链表）======
typedef struct HashNode {
    ElemType data;              // 关键字
    struct HashNode *next;      // 下一个同地址的节点
} HashNode;

// ======2. 散列表结构（数组，每个元素是链表头指针）======
typedef struct {
    HashNode *table[TABLE_SIZE]; // 散列表：每个位置是一个链表的头指针
    int count;                    // 已存入的记录数
} HashTable;

// ======3. 散列函数：除留余数法（★★★ 最常用）======
int Hash(ElemType key) {
    return key % TABLE_SIZE;   // H(key) = key mod p，p=TABLE_SIZE=13（质数）
}

/*
 * ★ 除留余数法：
 * H(key) = key mod p
 * p是不大于表长m的质数（或不含小于20的质因子的合数）
 * 选质数是为了让关键字分布更均匀，减少冲突
 */

// ======4. 初始化散列表======
void InitHashTable(HashTable &HT) {
    for(int i = 0; i < TABLE_SIZE; i++)
        HT.table[i] = NULL;     // 每个链表初始为空
    HT.count = 0;
}

// ======5. 拉链法插入（头插法）======
bool Hash_Insert(HashTable &HT, ElemType key) {
    int addr = Hash(key);                    // 计算散列地址
    // 检查是否已存在（避免重复插入）
    HashNode *p = HT.table[addr];
    while(p != NULL) {
        if(p->data == key) return false;     // 已存在，插入失败
        p = p->next;
    }
    // 头插法：新节点插到链表头部
    HashNode *s = (HashNode*)malloc(sizeof(HashNode));
    s->data = key;
    s->next = HT.table[addr];
    HT.table[addr] = s;
    HT.count++;
    return true;
}

// ======6. 拉链法查找======
HashNode* Hash_Search(HashTable HT, ElemType key) {
    int addr = Hash(key);                    // 先算散列地址
    HashNode *p = HT.table[addr];
    while(p != NULL) {
        if(p->data == key) return p;         // 在链表中找到
        p = p->next;                          // 链表中顺序查找
    }
    return NULL;                              // 查找失败
}

// ======7. 打印散列表======
void PrintHashTable(HashTable HT) {
    printf("       散列表（拉链法，表长=%d，已存%d个，装填因子α=%.2f）：\n",
           TABLE_SIZE, HT.count, (float)HT.count / TABLE_SIZE);
    for(int i = 0; i < TABLE_SIZE; i++) {
        printf("         [%2d] → ", i);
        HashNode *p = HT.table[i];
        while(p != NULL) {
            printf("%d → ", p->data);
            p = p->next;
        }
        printf("NULL\n");
    }
}

// ======8. 计算拉链法成功ASL======
// 对每个已存入的关键字，计算从初始散列地址开始到找到该关键字的比较次数
float Calc_Chaining_Success_ASL(HashTable HT) {
    int totalCompare = 0;
    int totalCount = 0;
    for(int i = 0; i < TABLE_SIZE; i++) {
        HashNode *p = HT.table[i];
        int pos = 1;  // 链表中第1个节点比较1次，第2个比较2次...
        while(p != NULL) {
            totalCompare += pos;
            totalCount++;
            pos++;
            p = p->next;
        }
    }
    if(totalCount == 0) return 0;
    return (float)totalCompare / totalCount;
}

// ======9. 计算拉链法失败ASL======
// 对每个散列地址，查找失败时需要遍历完整个链表（比较次数=链表长度）
float Calc_Chaining_Fail_ASL(HashTable HT) {
    int totalCompare = 0;
    for(int i = 0; i < TABLE_SIZE; i++) {
        int len = 0;
        HashNode *p = HT.table[i];
        while(p != NULL) {
            len++;
            p = p->next;
        }
        totalCompare += len;  // 失败时遍历完整个链表，比较次数=链表长度
    }
    return (float)totalCompare / TABLE_SIZE;  // 分母是表长m
}

// ======10. 主函数测试======
int main() {
    printf("====== 散列表拉链法测试开始 ======\n\n");

    // 【测试1】散列表构造
    printf("【测试1】散列表构造（除留余数法，表长13）\n");
    HashTable HT;
    InitHashTable(HT);
    ElemType keys[] = {19, 14, 23, 1, 68, 20, 84, 27, 55, 11, 10, 79};
    int n = 12;
    printf("       插入序列：");
    for(int i = 0; i < n; i++) printf("%d ", keys[i]);
    printf("\n");
    for(int i = 0; i < n; i++) {
        Hash_Insert(HT, keys[i]);
    }
    PrintHashTable(HT);

    // 【测试2】散列函数计算演示
    printf("\n【测试2】散列函数计算演示（H(key)=key mod 13）\n");
    int demoKeys[] = {19, 14, 23, 1, 68, 20, 84, 27};
    for(int i = 0; i < 8; i++) {
        printf("         H(%d) = %d mod 13 = %d\n", demoKeys[i], demoKeys[i], Hash(demoKeys[i]));
    }
    printf("       注意：19和84都映射到地址6（19%%13=6, 84%%13=6），发生冲突，用拉链法解决\n");

    // 【测试3】查找
    printf("\n【测试3】查找\n");
    HashNode *r1 = Hash_Search(HT, 68);
    printf("       查找68：%s（期望找到，地址68%%13=3）\n", r1 ? "找到" : "未找到");
    HashNode *r2 = Hash_Search(HT, 84);
    printf("       查找84：%s（期望找到，地址84%%13=6，与19冲突，在链表中）\n", r2 ? "找到" : "未找到");
    HashNode *r3 = Hash_Search(HT, 100);
    printf("       查找100：%s（期望未找到）\n", r3 ? "找到" : "未找到");

    // 【测试4】ASL计算
    printf("\n【测试4】ASL计算\n");
    float asl_succ = Calc_Chaining_Success_ASL(HT);
    float asl_fail = Calc_Chaining_Fail_ASL(HT);
    printf("       成功ASL = %.2f\n", asl_succ);
    printf("       失败ASL = %.2f（分母是表长m=%d）\n", asl_fail, TABLE_SIZE);
    printf("       装填因子α = n/m = %d/%d = %.2f\n", HT.count, TABLE_SIZE, (float)HT.count/TABLE_SIZE);
    printf("\n       手算验证：\n");
    printf("         地址6的链表：84→19（84是头，比较1次；19比较2次）\n");
    printf("         成功ASL = 各关键字比较次数之和 / 关键字总数\n");

    // 【测试5】拉链法的特点
    printf("\n【测试5】拉链法的特点（★ 记）\n");
    printf("       1. 不会产生堆积（二次聚集），冲突只在同一链表内\n");
    printf("       2. 装填因子α可以>1（链表可以无限延长）\n");
    printf("       3. 删除操作简单（链表中删节点即可）\n");
    printf("       4. 适合经常插入删除的场景\n");
    printf("       5. 指针需要额外空间，但记录较大时指针开销相对小\n");

    // 【测试6】近似公式
    printf("\n【测试6】拉链法ASL近似公式（α较大时）\n");
    float alpha = (float)HT.count / TABLE_SIZE;
    printf("       成功ASL ≈ 1 + α/2 = 1 + %.2f/2 = %.2f\n", alpha, 1 + alpha/2);
    printf("       失败ASL ≈ α = %.2f\n", alpha);
    printf("       （近似公式，实际手算按具体链表长度计算）\n");

    printf("\n====== 散列表拉链法测试结束 ======\n");
    return 0;
}
