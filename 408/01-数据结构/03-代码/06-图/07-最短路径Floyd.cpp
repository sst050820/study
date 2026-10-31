/*
 * ============================================================
 * 文件名：07-最短路径Floyd.cpp
 * 章  节：第六章 图
 * 内  容：全源最短路径 Floyd 算法（动态规划，三层循环 + path中转点记录 + 负环检测）
 * 重要度：★★★★ 熟练掌握（代码只有6行但考得细；手算必会）
 * 考研考点：k必须最外层、O(V^3)/O(V^2)、允许负边但不可有负环、path矩阵还原路径
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXV 100        // 最大顶点数
#define INF 0x7fffffff  // 无穷

// ======1. 带权有向图：邻接矩阵存储 ======
typedef struct {
    int n, e;                    // 顶点数、边数
    char VerticesList[MAXV];     // 顶点表
    int Edge[MAXV][MAXV];        // 邻接矩阵（权值，无边INF，自身0）
} MGraph;

void InitGraph(MGraph &G) {
    G.n = 0;
    G.e = 0;
    for(int i = 0; i < MAXV; i++) {
        G.VerticesList[i] = '\0';
        for(int j = 0; j < MAXV; j++) {
            G.Edge[i][j] = INF;
        }
    }
}

bool AddVertex(MGraph &G, char v) {
    if(G.n >= MAXV) return false;
    G.VerticesList[G.n] = v;
    G.n++;
    return true;
}

int LocateVex(MGraph G, char v) {
    for(int i = 0; i < G.n; i++) {
        if(G.VerticesList[i] == v) return i;
    }
    return -1;
}

void AddArc(MGraph &G, char u, char v, int w) {
    int i = LocateVex(G, u);
    int j = LocateVex(G, v);
    G.Edge[i][j] = w;
    G.e++;
}

// ======2. Floyd 算法（★★★★★ 核心6行必背）======
/*
 * 思想：动态规划，分n个阶段。
 *   第k阶段：允许顶点0..k作为中转点，检查所有点对(i,j)：
 *   若 A[i][k] + A[k][j] < A[i][j]，则更新A[i][j]并记录中转点k。
 * A矩阵：A[i][j] = i到j的当前最短距离（初始为邻接矩阵）
 * path矩阵：path[i][j] = i到j最短路径上的中转点（-1表示直接到达）
 * 注意：★ k必须是最外层循环！
 * 复杂度：时间O(V^3)，空间O(V^2)
 */
void Floyd(MGraph G, int A[][MAXV], int path[][MAXV]) {
    int n = G.n;
    // 1. 初始化A和path
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i == j) {
                A[i][j] = 0;           // 自己到自己距离为0
            } else {
                A[i][j] = G.Edge[i][j];
            }
            path[i][j] = -1;           // 初始：认为i到j直接可达
        }
    }

    // 2. ★ 核心三层循环：k在最外层
    for(int k = 0; k < n; k++) {           // 中转点（第k阶段）
        for(int i = 0; i < n; i++) {       // 起点
            for(int j = 0; j < n; j++) {   // 终点
                if(A[i][k] < INF && A[k][j] < INF) {   // 防溢出
                    if(A[i][k] + A[k][j] < A[i][j]) {
                        A[i][j] = A[i][k] + A[k][j];   // ★ 更新最短距离
                        path[i][j] = k;                // ★ 记录中转点
                    }
                }
            }
        }
    }

    // 3. 负环检测：若某点到自己变成负数，说明存在负权回路
    for(int i = 0; i < n; i++) {
        if(A[i][i] < 0) {
            printf("       检测到负权回路！最短路径不存在（绕圈会越来越短）\n");
            break;
        }
    }
}

// ======3. 打印A矩阵（工具函数）======
void PrintMatrix(int A[][MAXV], int n, const char *name) {
    printf("       %s：\n", name);
    for(int i = 0; i < n; i++) {
        printf("         ");
        for(int j = 0; j < n; j++) {
            if(A[i][j] == INF) {
                printf("  ∞");
            } else {
                printf("%3d", A[i][j]);
            }
        }
        printf("\n");
    }
}

// ======4. 递归还原路径（path矩阵可能嵌套多个中转点）======
void PrintPath(int path[][MAXV], char vertices[], int i, int j) {
    if(path[i][j] == -1) {       // 直接可达：没有中转点
        printf("%c", vertices[j]);
        return;
    }
    int k = path[i][j];          // 中转点k
    PrintPath(path, vertices, i, k);   // 先打印 i -> k 这一段
    printf(" -> ");              // ★ 段间分隔符
    PrintPath(path, vertices, k, j);   // 再打印 k -> j 这一段
}

// ======5. 构建测试图（视频065的教材例图，3顶点有向带权）======
/*
 * 顶点：v0 v1 v2
 * 弧：v0->v1:6  v0->v2:13  v1->v0:10  v1->v2:4  v2->v0:5
 * 手算期望（三轮）：
 *   初始A:  0 6 13 / 10 0 4 / 5 ∞ 0
 *   k=0：v2->v1 经v0：5+6=11 < ∞ -> 11（path[2][1]=0）
 *   k=1：v0->v2 经v1：6+4=10 < 13 -> 10（path[0][2]=1）
 *   k=2：v1->v0 经v2：4+5=9 < 10 -> 9（path[1][0]=2）
 *   最终：v0->v1=6, v0->v2=10(经v1), v1->v0=9(经v2), v1->v2=4, v2->v0=5, v2->v1=11(经v0)
 */
void BuildGraph(MGraph &G) {
    InitGraph(G);
    AddVertex(G, '0'); AddVertex(G, '1'); AddVertex(G, '2');
    AddArc(G, '0', '1', 6);
    AddArc(G, '0', '2', 13);
    AddArc(G, '1', '0', 10);
    AddArc(G, '1', '2', 4);
    AddArc(G, '2', '0', 5);
}

// ======6. 带负边的图（演示Floyd能处理负边）======
/*
 * 顶点：v0 v1 v2
 * 弧：v0->v1:4  v1->v2:3  v0->v2:-2? 用简单例子：
 * v0->v1:4, v0->v2:1? 设计：v0->v1:4, v1->v2:-2, v0->v2:5
 * 期望：v0->v2 经v1：4+(-2)=2 < 5 -> 2（Floyd能处理负边）
 */
void BuildNegEdgeGraph(MGraph &G) {
    InitGraph(G);
    AddVertex(G, '0'); AddVertex(G, '1'); AddVertex(G, '2');
    AddArc(G, '0', '1', 4);
    AddArc(G, '1', '2', -2);
    AddArc(G, '0', '2', 5);
}

// ======主函数：测试Floyd======
int main() {
    printf("====== 最短路径 Floyd 测试开始 ======\n\n");

    int A[MAXV][MAXV], path[MAXV][MAXV];

    MGraph G;
    BuildGraph(G);
    printf("【测试1】Floyd 教材例图（3顶点）\n");
    printf("       弧：0->1:6 0->2:13 1->0:10 1->2:4 2->0:5\n");
    printf("       期望：0->2最短=10（经1），1->0最短=9（经2），2->1最短=11（经0）\n");
    Floyd(G, A, path);
    PrintMatrix(A, G.n, "最终A矩阵（所有点对最短距离）");

    printf("\n       各点对最短路径（期望打印出中转路径）：\n");
    for(int i = 0; i < G.n; i++) {
        for(int j = 0; j < G.n; j++) {
            if(i != j && A[i][j] < INF) {
                printf("         %c -> ", G.VerticesList[i]);
                PrintPath(path, G.VerticesList, i, j);
                printf("，长度%d\n", A[i][j]);
            }
        }
    }

    printf("\n【测试2】Floyd 处理负权边（Dijkstra做不到的）\n");
    printf("       弧：0->1:4 1->2:-2 0->2:5\n");
    printf("       期望：0->2最短=2（经1：4+(-2)），Floyd能正确求出\n");
    MGraph G2;
    BuildNegEdgeGraph(G2);
    Floyd(G2, A, path);
    PrintMatrix(A, G2.n, "最终A矩阵");
    printf("       验证：0->2最短 = %d（期望2）\n", A[0][2]);

    printf("\n【测试3】负权回路（Floyd检测出并报错）\n");
    printf("       弧：0->1:1 1->2:1 2->0:-5（0->0绕一圈 = -3 < 0）\n");
    printf("       期望：检测到负权回路，A[0][0] < 0\n");
    MGraph G3;
    InitGraph(G3);
    AddVertex(G3, '0'); AddVertex(G3, '1'); AddVertex(G3, '2');
    AddArc(G3, '0', '1', 1);
    AddArc(G3, '1', '2', 1);
    AddArc(G3, '2', '0', -5);
    Floyd(G3, A, path);
    printf("       A[0][0] = %d（期望负数，证明存在负环）\n", A[0][0]);

    printf("\n====== 最短路径 Floyd 测试结束 ======\n");
    return 0;
}
