/*
 * ============================================================
 * 文件名：08-并查集.cpp
 * 章  节：第五章 树与二叉树（5.5.2 并查集）
 * 内  容：并查集的初始化、查Find、并Union，含"按大小合并"和"路径压缩"两种优化
 * 重要度：★★★★★ 必须熟练掌握（2022新增考点，近年选择题+大题热度极高）
 * 考研考点：双亲表示法存储、根存负数（绝对值=集合大小）、
 *          小树并入大树、路径压缩两步法、复杂度 O(nα(n))
 * ============================================================
 */

#include <stdio.h>

#define MAXSIZE 20 // 并查集最多容纳的元素个数

/*
 * ============================================================
 * 并查集的存储结构（★★★★★ 必背）：
 *   - 逻辑上是一组"互不相交的集合"
 *   - 物理上用双亲表示法：一个 int 数组 S
 *   - S[i] > 0 ：i 的双亲在数组中的下标
 *   - S[i] < 0 ：i 是根，负数的绝对值 = 该集合的元素个数
 * 为什么用双亲表示法：查（向上找根）和并（根指向根）都非常方便
 * ============================================================
 */

// ======1. 初始化：每个元素各自成集合（★★★★★ 必背）======
void Initial(int S[]) {
    for(int i = 0; i < MAXSIZE; i++) {
        S[i] = -1;              // ★ 全置-1：各自独立、都认为自己是一棵树的根
    }
}

/*
 * ======2. 查 Find：找到x所在集合的根（★★★★★ 必背）======
 * 方法：一路沿双亲链向上，直到 S[x]<0（根的特征）
 * 返回：根的下标（用它来代表"这个集合"）
 */
int Find(int S[], int x) {
    while(S[x] >= 0) {          // ★ S[x]>=0 说明x不是根
        x = S[x];               // 上移到双亲
    }
    return x;                   // 此时S[x]<0，x是根
}

/*
 * ======3. 并 Union：合并两个集合（朴素版，★★★★★ 必背）======
 * 传入两个集合的根 root1、root2
 * 做法：把 root2 的根指针指向 root1
 */
void Union(int S[], int root1, int root2) {
    if(root1 == root2) {        // ★ 同一集合无需合并
        return;
    }
    S[root2] = root1;           // ★ root2 的树根挂到 root1 下面
}

/*
 * ======4. 并 Union 优化：按大小合并（小树并入大树，★★★★★）======
 * 为什么：树越高，Find 越慢（最坏O(n)）；让树尽量"矮"是并查集优化的总思路
 * 做法：
 *   比较 S[root1]、S[root2]（都是负数）——负值越大，集合越小
 *   小树根指向大树根，并更新大树根的大小（两负值相加）
 */
void UnionBySize(int S[], int root1, int root2) {
    if(root1 == root2) {
        return;
    }
    if(S[root2] > S[root1]) {   // ★ 负值比较：root2 比 root1 更小
        S[root1] += S[root2];   // 累加结点数（如 -6 + -2 = -8）
        S[root2] = root1;       // 小树根指向大树根
    } else {
        S[root2] += S[root1];
        S[root1] = root2;
    }
}

/*
 * ======5. 查 Find 优化：路径压缩（★★★★★ 必背）======
 * 两步法：
 *   ① 先沿路径找到根 root；
 *   ② 再沿路径走一遍，把沿途每个结点直接挂到 root 下面
 * 效果：下次再查这些结点，一步就到根；树高被"压矮"
 * 复杂度：结合按大小合并，单次 Find 约 O(α(n))（α增长极慢，常见n时≤4，近似常数）
 */
int FindCompress(int S[], int x) {
    int root = x;
    while(S[root] >= 0) {       // ① 第一步：先找到根
        root = S[root];
    }
    while(x != root) {          // ② 第二步：压缩路径
        int t = S[x];           // 暂存x的原双亲
        S[x] = root;            // ★ x直接挂到根下
        x = t;                  // 继续处理下一个
    }
    return root;                // 返回根
}

// ======6. 打印并查集数组（观察状态用）======
void PrintSet(int S[], int n) {
    printf("       下标：");
    for(int i = 0; i < n; i++) {
        printf("%3d ", i);
    }
    printf("\n       S[i]：");
    for(int i = 0; i < n; i++) {
        printf("%3d ", S[i]);
    }
    printf("\n");
}

// ======主函数：测试并查集======
int main() {
    int S[MAXSIZE];             // 并查集数组
    int n = 8;                  // 本次测试用8个元素：0~7

    printf("====== 并查集测试开始 ======\n\n");

    // 测试1：初始化 + 朴素Union
    printf("【测试1】初始化 + 朴素Union\n");
    Initial(S);
    printf("       初始状态（各元素独立成集）：\n");
    PrintSet(S, n);

    Union(S, Find(S, 0), Find(S, 1));   // 并{0}和{1}
    Union(S, Find(S, 2), Find(S, 3));   // 并{2}和{3}
    Union(S, Find(S, 0), Find(S, 2));   // 并{0,1}和{2,3} → {0,1,2,3}
    printf("       合并{0,1,2,3}后：\n");
    PrintSet(S, n);
    printf("       Find(3)的根 = %d，Find(1)的根 = %d → %s\n",
           Find(S, 3), Find(S, 1),
           Find(S, 3) == Find(S, 1) ? "同一集合(正确)" : "不同集合(错误)");
    printf("       Find(3)的根 = %d，Find(4)的根 = %d → %s\n",
           Find(S, 3), Find(S, 4),
           Find(S, 3) == Find(S, 4) ? "同一集合(错误)" : "不同集合(正确)");

    // 测试2：按大小合并（看树是否长不高）
    printf("\n【测试2】按大小合并优化\n");
    Initial(S);
    for(int i = 0; i < 8; i += 2) {
        UnionBySize(S, Find(S, i), Find(S, i + 1)); // 两两合并
    }
    UnionBySize(S, Find(S, 0), Find(S, 2)); // 小树并入大树
    UnionBySize(S, Find(S, 4), Find(S, 6));
    printf("       合并后状态（根存负数，绝对值=集合大小）：\n");
    PrintSet(S, n);
    printf("       根0的大小 = %d（期望4）\n", -S[Find(S, 0)]);
    printf("       根4的大小 = %d（期望4）\n", -S[Find(S, 4)]);

    // 测试3：路径压缩
    printf("\n【测试3】路径压缩效果\n");
    Initial(S);
    // 故意造一条长链：1->2->3->4->5->6->7->0
    S[0] = -8;
    S[1] = 2;  S[2] = 3;  S[3] = 4;  S[4] = 5;  S[5] = 6;  S[6] = 7;  S[7] = 0;
    printf("       压缩前（长链）：\n");
    PrintSet(S, n);
    printf("       FindCompress(1) = %d（找到根0）\n", FindCompress(S, 1));
    printf("       压缩后（1~7全部直接挂到0下）：\n");
    PrintSet(S, n);

    // 测试4：复杂度总结
    printf("\n【测试4】复杂度总结（★必背）\n");
    printf("       朴素：Find最坏O(n)，n次合并总O(n^2)\n");
    printf("       按大小合并：Find最坏O(log n)，n次合并总O(n log n)\n");
    printf("       按大小合并+路径压缩：Find约O(α(n))（α≤4，近似常数），总O(n α(n))\n");

    printf("\n====== 并查集测试结束 ======\n");
    return 0;
}
