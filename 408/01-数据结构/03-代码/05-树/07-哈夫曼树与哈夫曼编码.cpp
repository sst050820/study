/*
 * ============================================================
 * 文件名：07-哈夫曼树与哈夫曼编码.cpp
 * 章  节：第五章 树与二叉树
 * 内  容：哈夫曼树的构造、WPL计算、哈夫曼编码生成
 * 重要度：★★★★ 重点掌握（选择+大题常考：构造、WPL、编码）
 * 考研考点：每次选两个最小权合并、n叶→2n-1结点、无度1结点、
 *          WPL=各次合并权值和=叶子权×路径长、前缀编码无歧义
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ============================================================
 * 哈夫曼树的顺序存储（★★★★ 必背结构）：
 *   - 用数组存整棵树，下标从1开始（0号不用，方便和"结点数"对应）
 *   - 前 n 个是叶子，后面 n-1 个是合并产生的分支结点
 *   - parent/lchild/rchild 存数组下标；0表示"无"
 *   - parent=0 的结点属于"当前森林"，可以被继续合并
 * ============================================================
 */
typedef struct {
    int weight;                 // 权值（叶子权值/合并后根权值）
    int parent, lchild, rchild; // 双亲、左孩子、右孩子下标（0=无）
} HuffmanNode;

// ======1. 从森林中选两个权值最小且无双亲的结点（★★★★ 核心）======
// 结果通过引用参数带回
void SelectTwoMin(HuffmanNode huff[], int n, int &s1, int &s2) {
    // 方法：扫描两次，第一次找最小s1，第二次找次小s2（跳过s1）
    int min1 = -1, min2 = -1;
    // 找最小
    for(int i = 1; i <= n; i++) {
        if(huff[i].parent == 0) {           // 只考虑还在森林里的树根
            if(min1 == -1 || huff[i].weight < huff[min1].weight) {
                min1 = i;
            }
        }
    }
    // 找次小（跳过min1）
    for(int i = 1; i <= n; i++) {
        if(huff[i].parent == 0 && i != min1) {
            if(min2 == -1 || huff[i].weight < huff[min2].weight) {
                min2 = i;
            }
        }
    }
    s1 = min1;
    s2 = min2;
}

/*
 * ======2. 构造哈夫曼树（★★★★★ 必背）======
 * 参数：huff数组、叶子个数n
 * 过程（共 n-1 次合并）：
 *   每次选两个最小根 s1、s2 → 生成新结点 n+k（k从1到n-1）
 *   新结点权值 = 两者之和；s1、s2 的parent指向新结点；
 *   新结点lchild=s1、rchild=s2
 */
void CreateHuffmanTree(HuffmanNode huff[], int n) {
    int m = 2 * n - 1;                      // ★ 总结点数 = 2n-1
    // 初始化所有结点（前n个叶子的权值已填入，其余字段置0）
    for(int i = n + 1; i <= m; i++) {       // 合并产生的新结点从n+1开始
        huff[i].weight = 0;
        huff[i].parent = huff[i].lchild = huff[i].rchild = 0;
    }
    // 循环 n-1 次合并
    for(int k = n + 1; k <= m; k++) {       // 新结点编号从 n+1 到 2n-1
        int s1, s2;
        SelectTwoMin(huff, k - 1, s1, s2);  // ★ 从"已有k-1个结点"的森林里选两个最小
        huff[s1].parent = k;                // s1认新结点为双亲
        huff[s2].parent = k;
        huff[k].lchild = s1;                // 新结点左孩子=小的那个
        huff[k].rchild = s2;                // 右孩子=次小的那个
        huff[k].weight = huff[s1].weight + huff[s2].weight; // ★ 新根权值=和
    }
}

/*
 * ======3. 求WPL（带权路径长度，★★★★★ 必算）======
 * 两种算法：
 *   ① 定义法：WPL = Σ 叶子权 × 根到叶子的路径长度（要算路径长度）
 *   ② 技巧法：WPL = 所有"合并产生的新结点"权值之和（各次合并的和）
 *            （每次合并的权值和，恰好等于被合并叶子的带权路径贡献）
 * 这里用②，最省事且不易错
 */
int GetWPL(HuffmanNode huff[], int n) {
    int wpl = 0;
    for(int i = n + 1; i <= 2 * n - 1; i++) { // 只累加分支结点
        wpl += huff[i].weight;              // ★ 新结点权值累加
    }
    return wpl;
}

/*
 * ======4. 生成哈夫曼编码（★★★★★ 必写）======
 * 思路：对每个叶子，从叶子向根回溯——
 *   是双亲的左孩子 → 本层编码'0'；是右孩子 → '1'
 *   从叶子走到根，得到的编码是"反的"，最后逆序输出
 * 约定：左0右1（左右互换也行，编码会不同但都是合法的）
 */
void GetHuffmanCode(HuffmanNode huff[], int n) {
    char code[64];                          // 存放单个叶子的编码（逆序）
    for(int i = 1; i <= n; i++) {           // 逐个叶子
        int len = 0;
        int c = i;                          // 当前结点下标（从叶子开始）
        int f = huff[i].parent;             // 它的双亲
        while(f != 0) {                     // ★ 只要没到根（根无parent）
            if(huff[f].lchild == c) {       // 自己是双亲的左孩子
                code[len++] = '0';
            } else {                        // 自己是双亲的右孩子
                code[len++] = '1';
            }
            c = f;                          // 上移到双亲
            f = huff[f].parent;
        }
        // 逆序输出（从根到叶）
        printf("       权值%2d 的编码：", huff[i].weight);
        for(int j = len - 1; j >= 0; j--) {
            printf("%c", code[j]);
        }
        printf("\n");
    }
}

// ======5. 打印哈夫曼树结构（验证用）======
void PrintHuffmanTree(HuffmanNode huff[], int n) {
    printf("       下标  权值  双亲  左孩  右孩\n");
    for(int i = 1; i <= 2 * n - 1; i++) {
        printf("       %2d   %4d   %4d   %4d   %4d\n",
               i, huff[i].weight, huff[i].parent,
               huff[i].lchild, huff[i].rchild);
    }
}

// ======主函数：测试哈夫曼树======
int main() {
    /*
     * 测试数据：权值 5 7 10 15（教材典型例题）
     * 合并过程：
     *   5+7=12 → 森林{10,12,15}
     *   10+12=22 → 森林{15,22}
     *   15+22=37 → 完成
     * WPL = 12+22+37 = 71
     * （验证：5*3+7*3+10*2+15*1 = 15+21+20+15 = 71 ✓）
     */
    int n = 4;                              // 叶子个数
    HuffmanNode huff[2 * 4];                // 数组大小=2n

    printf("====== 哈夫曼树与哈夫曼编码测试开始 ======\n\n");

    // 初始化：前n个放叶子
    int weights[] = {0, 5, 7, 10, 15};      // 下标1..n对应权值
    for(int i = 1; i <= n; i++) {
        huff[i].weight = weights[i];
        huff[i].parent = huff[i].lchild = huff[i].rchild = 0;
    }

    // 测试1：构造哈夫曼树
    printf("【测试1】构造哈夫曼树（叶子权值：5 7 10 15）\n");
    CreateHuffmanTree(huff, n);
    PrintHuffmanTree(huff, n);
    printf("       总结点数 = %d（期望2n-1=7）\n", 2 * n - 1);

    // 测试2：WPL
    printf("\n【测试2】带权路径长度WPL\n");
    printf("       WPL = %d（期望71）\n", GetWPL(huff, n));

    // 测试3：哈夫曼编码
    printf("\n【测试3】哈夫曼编码（左0右1）\n");
    GetHuffmanCode(huff, n);
    printf("       验证：编码均为前缀编码（任一编码不是另一编码的前缀），解码无歧义\n");

    /*
     * 【易错提醒】
     * 1. 哈夫曼树不唯一（两个最小权谁左谁右可换），但WPL唯一且最小；
     * 2. n个叶子必产生n-1次合并，总结点数恒为2n-1；
     * 3. 哈夫曼树没有度为1的结点；
     * 4. 权值越小的叶子离根越远（路径越长）；
     * 5. WPL = 各次合并的权值和（验算神器）。
     */

    printf("\n====== 哈夫曼树与哈夫曼编码测试结束 ======\n");
    return 0;
}
