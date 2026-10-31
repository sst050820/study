/*
 * ============================================================
 * 文件名：05-最小生成树Prim与Kruskal.cpp
 * 章  节：第六章 图
 * 内  容：带权无向图 + Prim算法（选顶点）+ Kruskal算法（选边，并查集判环）
 * 重要度：★★★★ 熟练掌握（手算必会；代码能写出是加分项）
 * 考研考点：Prim O(V^2) 适合稠密图、Kruskal O(ElogE) 适合稀疏图、MST不唯一但权和唯一
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXV 100        // 最大顶点数
#define INF 0x7fffffff  // 无穷（表示无边）

// ======1. 带权无向图：用邻接矩阵存（Prim的直接存储基础）======
/*
 * 带权图：Edge[i][j]存权值，无边存INF（而不是0，因为0可能是合法权值）
 * 自己到自己置0（方便计算）
 */
typedef struct {
    int n, e;                    // 顶点数、边数
    char VerticesList[MAXV];     // 顶点表
    int Edge[MAXV][MAXV];        // 邻接矩阵（存权值）
} MGraph;

// ======2. 初始化空图 ======
void InitGraph(MGraph &G) {
    G.n = 0;
    G.e = 0;
    for(int i = 0; i < MAXV; i++) {
        G.VerticesList[i] = '\0';
        for(int j = 0; j < MAXV; j++) {
            G.Edge[i][j] = INF;  // 默认全部无边
        }
    }
}

// ======3. 添加顶点 ======
bool AddVertex(MGraph &G, char v) {
    if(G.n >= MAXV) return false;
    G.VerticesList[G.n] = v;
    G.n++;
    return true;
}

// ======4. 查顶点下标 ======
int LocateVex(MGraph G, char v) {
    for(int i = 0; i < G.n; i++) {
        if(G.VerticesList[i] == v) return i;
    }
    return -1;
}

// ======5. 添加带权无向边 ======
void AddWeightedEdge(MGraph &G, char u, char v, int w) {
    int i = LocateVex(G, u);
    int j = LocateVex(G, v);
    G.Edge[i][j] = w;            // 无向图：权值对称存放
    G.Edge[j][i] = w;
    G.e++;
}

// ======6. Prim 算法（★★★★ 必会）======
/*
 * 思想：从任意顶点出发，每轮在"树内-树外"的边中选权值最小的一条，
 *       把新顶点并入树内，直到所有顶点入树。
 * 两个辅助数组：
 *   - lowCost[]：记录各顶点加入当前树的最小代价（INF=还没边连到树）
 *   - isJoin[] ：标记顶点是否已入树
 * 复杂度：O(V^2)（n-1轮 × 每轮扫数组O(n)），适合稠密图
 */
void Prim(MGraph G, int v0) {
    int lowCost[MAXV];           // 各顶点到当前生成树的最小边权
    bool isJoin[MAXV];           // 是否已加入生成树
    int parent[MAXV];            // 记录每个顶点是通过谁加入树的（用于输出边）
    int sum = 0;                 // 生成树总权值

    for(int i = 0; i < G.n; i++) {
        lowCost[i] = G.Edge[v0][i];   // ★ 初始：v0到各顶点的边权（无边为INF）
        isJoin[i] = false;            // 都还没入树
        parent[i] = v0;               // 暂时都认为从v0连过来
    }
    isJoin[v0] = true;                // v0先入树

    printf("        Prim从%c出发：\n", G.VerticesList[v0]);
    for(int t = 1; t < G.n; t++) {    // 共需n-1轮，每轮加入一个顶点
        // a. 找"未入树且lowCost最小"的顶点u
        int u = -1, min = INF;
        for(int i = 0; i < G.n; i++) {
            if(!isJoin[i] && lowCost[i] < min) {   // ★ 未入树 + 代价最小
                min = lowCost[i];
                u = i;
            }
        }
        // b. 把u并入生成树（输出这条选中的边）
        isJoin[u] = true;
        sum += min;
        printf("         选边 %c-%c（权%d）\n",
               G.VerticesList[parent[u]], G.VerticesList[u], min);
        // c. ★ 用u更新其他未入树顶点的lowCost（u加入后可能有更近的边）
        for(int i = 0; i < G.n; i++) {
            if(!isJoin[i] && G.Edge[u][i] < lowCost[i]) {
                lowCost[i] = G.Edge[u][i];   // 发现更小的边权就更新
                parent[i] = u;               // 并记录前驱
            }
        }
    }
    printf("        MST总权值 = %d\n", sum);
}

// ======7. 并查集（第5章内容，Kruskal判环要用）======
int S[MAXV];                    // 并查集数组：S[i]<0表示i是根，|S[i]|是集合元素数

// 找根（含路径压缩）
int Find(int x) {
    if(S[x] < 0) return x;      // 负数表示自己是根
    return S[x] = Find(S[x]);   // ★ 路径压缩：直接把x挂到根下
}

// 合并两个集合（按大小合并）
void Union(int a, int b) {
    int ra = Find(a), rb = Find(b);
    if(ra == rb) return;        // 已在同一集合
    if(S[ra] > S[rb]) {         // 负值：-3比-1小，说明ra的集合更大（3>1）
        S[ra] += S[rb];         // 小集合并入大集合
        S[rb] = ra;
    } else {
        S[rb] += S[ra];
        S[ra] = rb;
    }
}

// ======8. 边结构体（Kruskal要按权值排序）======
typedef struct {
    int u, v;                   // 边的两个端点
    int w;                      // 权值
} Edge;

// 简单选择排序：按权值从小到大（边少时够用）
void SortEdges(Edge edges[], int m) {
    for(int i = 0; i < m - 1; i++) {
        int minIdx = i;
        for(int j = i + 1; j < m; j++) {
            if(edges[j].w < edges[minIdx].w) minIdx = j;
        }
        if(minIdx != i) {       // 交换
            Edge t = edges[i];
            edges[i] = edges[minIdx];
            edges[minIdx] = t;
        }
    }
}

// ======9. Kruskal 算法（★★★★ 必会）======
/*
 * 思想：所有边按权值从小到大排序，逐条检查，
 *       两端点不在同一集合就选它（Union），在同一集合就跳过（否则成环）。
 * 判环：并查集！两个端点Find结果相同 → 已在同一连通集合 → 选它必成环。
 * 复杂度：O(ElogE)（排序主导），适合稀疏图
 */
void Kruskal(MGraph G) {
    Edge edges[MAXV * MAXV];    // 边集
    int m = 0;
    // a. 收集所有边（无向图只取上半三角，避免重复）
    for(int i = 0; i < G.n; i++) {
        for(int j = i + 1; j < G.n; j++) {
            if(G.Edge[i][j] != INF) {
                edges[m].u = i;
                edges[m].v = j;
                edges[m].w = G.Edge[i][j];
                m++;
            }
        }
    }
    SortEdges(edges, m);        // b. ★ 按权值从小到大排序

    for(int i = 0; i < G.n; i++) S[i] = -1;   // c. 并查集初始化：每个顶点独立成集

    int sum = 0, cnt = 0;       // 总权值、已选边数
    printf("        Kruskal选边过程：\n");
    for(int i = 0; i < m && cnt < G.n - 1; i++) {   // 选够n-1条边即可停止
        int u = edges[i].u, v = edges[i].v, w = edges[i].w;
        if(Find(u) != Find(v)) {     // ★ 两端不在同一集合 → 不会成环
            Union(u, v);             // 合并两个集合
            sum += w;
            cnt++;
            printf("         选边 %c-%c（权%d）\n",
                   G.VerticesList[u], G.VerticesList[v], w);
        }
        // 同一集合 → 跳过（选它会成环）
    }
    printf("        MST总权值 = %d\n", sum);
}

// ======10. 构建测试图（6个顶点带权无向图）======
/*
 * 图结构（顶点0~5）：
 *   0-1:6  0-2:5  0-3:1  1-3:5  2-3:4  2-4:2  3-4:6  4-5:3
 * 手工推算MST总权值：
 *   Prim从0出发：0-3(1) -> 3-2(4) -> 2-4(2) -> 4-5(3) -> 3-1(5)，总权=15
 *   Kruskal：0-3(1), 2-4(2), 4-5(3), 2-3(4), 1-3(5)，总权=15
 */
void BuildGraph(MGraph &G) {
    InitGraph(G);
    AddVertex(G, '0'); AddVertex(G, '1'); AddVertex(G, '2');
    AddVertex(G, '3'); AddVertex(G, '4'); AddVertex(G, '5');
    AddWeightedEdge(G, '0', '1', 6);
    AddWeightedEdge(G, '0', '2', 5);
    AddWeightedEdge(G, '0', '3', 1);
    AddWeightedEdge(G, '1', '3', 5);
    AddWeightedEdge(G, '2', '3', 4);
    AddWeightedEdge(G, '2', '4', 2);
    AddWeightedEdge(G, '3', '4', 6);
    AddWeightedEdge(G, '4', '5', 3);
}

// ======主函数：测试Prim和Kruskal======
int main() {
    printf("====== 最小生成树 Prim 与 Kruskal 测试开始 ======\n\n");

    MGraph G;
    BuildGraph(G);
    printf("【测试1】Prim 算法（从顶点0出发）\n");
    printf("       图边：0-1:6 0-2:5 0-3:1 1-3:5 2-3:4 2-4:2 3-4:6 4-5:3\n");
    printf("       期望MST总权 = 15（选边0-3, 3-2, 2-4, 4-5, 3-1）\n");
    Prim(G, 0);

    printf("\n【测试2】Kruskal 算法\n");
    printf("       期望MST总权 = 15（选边0-3, 2-4, 4-5, 2-3, 1-3）\n");
    Kruskal(G);

    printf("\n【测试3】从不同顶点出发的Prim（验证MST总权值唯一）\n");
    Prim(G, 3);                  // 从顶点3出发，总权应仍为15

    printf("\n====== 最小生成树 Prim 与 Kruskal 测试结束 ======\n");
    return 0;
}
