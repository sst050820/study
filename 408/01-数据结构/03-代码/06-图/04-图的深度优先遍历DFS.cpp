/*
 * ============================================================
 * 文件名：04-图的深度优先遍历DFS.cpp
 * 章  节：第六章 图
 * 内  容：邻接表存储下 DFS + 非连通图处理 + 统计连通分量数
 * 重要度：★★★★★ 必须熟练掌握（与树的先根遍历同源，递归思想是图算法根基）
 * 考研考点：递归+visited、访问时标记、非连通外层循环、空间来自递归工作栈
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXV 100    // 最大顶点数

// ======1. 邻接表存储结构（与03文件相同，DFS/ BFS共用）======
typedef struct ArcNode {
    int adjvex;                  // 邻接顶点编号
    struct ArcNode *next;        // 下一条边
} ArcNode;

typedef struct VNode {
    char data;                   // 顶点信息
    ArcNode *first;              // 第一条边
} VNode;

typedef struct {
    VNode adjlist[MAXV];         // 顶点表
    int n, e;                    // 顶点数、边数
} ALGraph;

// ======2. 图的基本操作（遍历的地基）======
int FirstNeighbor(ALGraph G, int v) {
    if(G.adjlist[v].first != NULL) {
        return G.adjlist[v].first->adjvex;
    }
    return -1;
}

int NextNeighbor(ALGraph G, int v, int w) {
    ArcNode *p = G.adjlist[v].first;
    while(p != NULL && p->adjvex != w) {
        p = p->next;
    }
    if(p != NULL && p->next != NULL) {
        return p->next->adjvex;
    }
    return -1;
}

// ======3. 访问顶点 ======
void visit(int v) {
    printf("%c ", 'A' + v);                    // 顶点编号0对应'A'
}

// ======4. DFS 核心算法（★★★★★ 必背）======
/*
 * 思想：树的先根遍历的推广（一条路走到黑）
 * 过程：访问顶点 -> 递归访问它的所有"未访问"邻接点
 * 空间：递归工作栈（最坏O(V)）
 * 注意：DFS没有队列，递归调用栈就是它的辅助空间
 */
bool visited[MAXV];                            // 访问标记数组（全局）

void DFS(ALGraph G, int v) {
    visit(v);                                  // 1. 访问顶点v
    visited[v] = true;                         // 2. ★ 标记已访问（访问时就标）
    for(int w = FirstNeighbor(G, v); w >= 0; w = NextNeighbor(G, v, w)) {
        if(!visited[w]) {                      // 3. ★ w未被访问才递归
            DFS(G, w);                         // 4. 递归访问w（深入！）
        }
    }
    // 5. 所有邻接点都访问完，函数返回（相当于退栈），继续上一层
}

// ======5. 非连通图遍历 + 统计连通分量（★★★ 必背）======
/*
 * 与BFS同理：一次DFS只能访问一个连通分量。
 * 外层for扫描visited数组，每次调用DFS的次数 = 连通分量数。
 */
int DFSTraverse(ALGraph G) {
    int count = 0;
    for(int i = 0; i < G.n; i++) {
        visited[i] = false;                    // 初始化
    }
    for(int i = 0; i < G.n; i++) {
        if(!visited[i]) {                      // ★ 未访问就启动一次DFS
            DFS(G, i);
            count++;
        }
    }
    return count;
}

// ======6. 构建测试图（工具函数）======
// 辅助：初始化n个顶点
void InitGraphForTest(ALGraph &G, int n) {
    G.n = n;
    G.e = 0;
    for(int i = 0; i < n; i++) {
        G.adjlist[i].data = (char)('A' + i);
        G.adjlist[i].first = NULL;
    }
}

// 辅助：加无向边（头插法）
void AddEdgeForTest(ALGraph &G, int i, int j) {
    ArcNode *p1 = (ArcNode *)malloc(sizeof(ArcNode));
    p1->adjvex = j;
    p1->next = G.adjlist[i].first;
    G.adjlist[i].first = p1;
    ArcNode *p2 = (ArcNode *)malloc(sizeof(ArcNode));
    p2->adjvex = i;
    p2->next = G.adjlist[j].first;
    G.adjlist[j].first = p2;
    G.e++;
}

// 测试图1：连通图 A-B, A-C, A-D, B-E, C-E
void BuildConnectedGraph(ALGraph &G) {
    InitGraphForTest(G, 5);
    AddEdgeForTest(G, 0, 1);                   // A-B
    AddEdgeForTest(G, 0, 2);                   // A-C
    AddEdgeForTest(G, 0, 3);                   // A-D
    AddEdgeForTest(G, 1, 4);                   // B-E
    AddEdgeForTest(G, 2, 4);                   // C-E
}

// 测试图2：非连通图（2个连通分量）
//   分量1：A-B, A-C       分量2：D-E, E-F, D-F
void BuildDisconnectedGraph(ALGraph &G) {
    InitGraphForTest(G, 6);
    AddEdgeForTest(G, 0, 1);                   // A-B
    AddEdgeForTest(G, 0, 2);                   // A-C
    AddEdgeForTest(G, 3, 4);                   // D-E
    AddEdgeForTest(G, 4, 5);                   // E-F
    AddEdgeForTest(G, 3, 5);                   // D-F
}

// 测试图3：有向图（演示有向图遍历与强连通的关系）
//   A->B, B->C, C->A 是一个强连通分量；D->E, E->D 是另一个
void BuildDirectedGraph(ALGraph &G) {
    InitGraphForTest(G, 5);
    // 手动构造有向边（只在弧尾顶点加边结点）
    ArcNode *p;
    p = (ArcNode *)malloc(sizeof(ArcNode)); p->adjvex = 1; p->next = G.adjlist[0].first; G.adjlist[0].first = p;  // A->B
    p = (ArcNode *)malloc(sizeof(ArcNode)); p->adjvex = 2; p->next = G.adjlist[1].first; G.adjlist[1].first = p;  // B->C
    p = (ArcNode *)malloc(sizeof(ArcNode)); p->adjvex = 0; p->next = G.adjlist[2].first; G.adjlist[2].first = p;  // C->A
    p = (ArcNode *)malloc(sizeof(ArcNode)); p->adjvex = 4; p->next = G.adjlist[3].first; G.adjlist[3].first = p;  // D->E
    p = (ArcNode *)malloc(sizeof(ArcNode)); p->adjvex = 3; p->next = G.adjlist[4].first; G.adjlist[4].first = p;  // E->D
    G.e = 5;
}

// ======主函数：测试DFS======
int main() {
    printf("====== 深度优先遍历DFS测试开始 ======\n\n");

    ALGraph G1;
    BuildConnectedGraph(G1);
    printf("【测试1】连通图 DFS（从A出发）\n");
    printf("       图：A-B, A-C, A-D, B-E, C-E\n");
    printf("       邻接表（头插法，边表顺序与插入相反）：\n");
    for(int i = 0; i < G1.n; i++) {
        printf("         %c -> ", G1.adjlist[i].data);
        ArcNode *p = G1.adjlist[i].first;
        while(p != NULL) {
            printf("%c ", G1.adjlist[p->adjvex].data);
            p = p->next;
        }
        printf("\n");
    }
    printf("       DFS序列：");
    int cnt1 = DFSTraverse(G1);
    printf("\n       连通图DFS调用次数 = %d（期望1）\n", cnt1);

    printf("\n【测试2】非连通图 DFS（2个连通分量）\n");
    ALGraph G2;
    BuildDisconnectedGraph(G2);
    printf("       DFS序列：");
    int cnt2 = DFSTraverse(G2);
    printf("\n       非连通图DFS调用次数 = %d（期望2，等于连通分量数）\n", cnt2);

    printf("\n【测试3】从不同顶点出发的DFS序列（理解起点影响序列）\n");
    printf("       从A出发：");
    for(int i = 0; i < G1.n; i++) visited[i] = false;
    DFS(G1, 0);
    printf("\n       从E出发：");
    for(int i = 0; i < G1.n; i++) visited[i] = false;
    DFS(G1, 4);
    printf("\n       （两个序列不同：DFS序列依赖起始顶点和存储结构）\n");

    printf("\n【测试4】有向图遍历（强连通分量与调用次数）\n");
    ALGraph G3;
    BuildDirectedGraph(G3);
    printf("       图：A->B->C->A 强连通；D<->E 强连通（2个强连通分量）\n");
    printf("       从A出发的DFS：");
    for(int i = 0; i < G3.n; i++) visited[i] = false;
    DFS(G3, 0);
    printf("\n       （只能访问到A,B,C，说明有向图从一点出发不一定能遍历全图）\n");
    printf("       完整遍历序列：");
    int cnt3 = DFSTraverse(G3);
    printf("\n       完整遍历调用次数 = %d（期望2，等于强连通分量数）\n", cnt3);

    printf("\n====== 深度优先遍历DFS测试结束 ======\n");
    return 0;
}
