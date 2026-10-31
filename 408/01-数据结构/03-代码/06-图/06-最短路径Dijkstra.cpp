/*
 * ============================================================
 * 文件名：06-最短路径Dijkstra.cpp
 * 章  节：第六章 图
 * 内  容：带权有向图 + Dijkstra单源最短路径（三数组 + 路径还原）
 * 重要度：★★★★★ 必须熟练掌握（手算大题高频，代码骨架也要会写）
 * 考研考点：final/dist/path三数组含义、每轮选最小并松弛、不能有负权边、与Prim相似
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXV 100        // 最大顶点数
#define INF 0x7fffffff  // 无穷（无边）

// ======1. 带权有向图：邻接矩阵存储 ======
typedef struct {
    int n, e;                    // 顶点数、边数
    char VerticesList[MAXV];     // 顶点表
    int Edge[MAXV][MAXV];        // 邻接矩阵（Edge[i][j] = <i,j>的权，无边为INF）
} MGraph;

// ======2. 初始化 + 加顶点 + 加有向带权边 ======
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

// 有向边：只置一处（i->j）
void AddArc(MGraph &G, char u, char v, int w) {
    int i = LocateVex(G, u);
    int j = LocateVex(G, v);
    G.Edge[i][j] = w;
    G.e++;
}

// ======3. Dijkstra 算法（★★★★★ 必背骨架）======
/*
 * 三数组：
 *   - final[]：顶点是否已确定最短路径（true=已确定，不会再看它）
 *   - dist[] ：当前从源点到各顶点的最短路径长度（会不断被"松弛"更新）
 *   - path[] ：各顶点最短路径上的直接前驱（用于最后还原整条路径）
 * 执行过程（共n-1轮）：
 *   1. 在未确定(final=false)的顶点中选dist最小的u；
 *   2. 把u的final置true（u的最短路径就此锁定）；
 *   3. 松弛：检查u的所有出边<u,v>，若dist[u]+w < dist[v] 则更新dist[v]和path[v]。
 * 复杂度：O(V^2)；不能处理负权边（已确定的最短路径不会再被修正）。
 */
void Dijkstra(MGraph G, int v0) {
    bool final[MAXV];            // final[i]=true 表示顶点i的最短路径已确定
    int dist[MAXV];              // 源点到顶点i的当前最短路径长度
    int path[MAXV];              // 顶点i在最短路径上的直接前驱

    // 1. 初始化三数组
    for(int i = 0; i < G.n; i++) {
        final[i] = false;
        dist[i] = G.Edge[v0][i]; // ★ 初始：源点直接出边的权（无边为INF）
        path[i] = (G.Edge[v0][i] < INF) ? v0 : -1;   // 直接可达则前驱是源点
    }
    final[v0] = true;            // 源点自身最短路径=0，先确定
    dist[v0] = 0;
    path[v0] = -1;

    printf("       每轮确定的顶点（final置true的顺序）：\n");
    // 2. 共n-1轮：每轮确定一个顶点
    for(int t = 1; t < G.n; t++) {
        // a. 在未确定的顶点中选dist最小的u
        int u = -1, min = INF;
        for(int i = 0; i < G.n; i++) {
            if(!final[i] && dist[i] < min) {
                min = dist[i];
                u = i;
            }
        }
        if(u == -1) break;       // 剩余顶点都不可达（INF），提前结束
        final[u] = true;         // b. ★ 确定u的最短路径
        printf("         确定顶点 %c，dist=%d\n", G.VerticesList[u], dist[u]);

        // c. ★ 松弛：检查u的所有出边，尝试更新邻接点的dist
        for(int v = 0; v < G.n; v++) {
            if(!final[v] && G.Edge[u][v] < INF) {   // v未确定 且 <u,v>有边
                if(dist[u] + G.Edge[u][v] < dist[v]) {
                    dist[v] = dist[u] + G.Edge[u][v];  // ★ 发现更短路径就更新
                    path[v] = u;                       // 记录前驱u
                }
            }
        }
    }

    // 3. 输出最终结果
    printf("       最终dist（源点->各顶点最短距离）：\n");
    for(int i = 0; i < G.n; i++) {
        if(dist[i] == INF) {
            printf("         到%c：不可达\n", G.VerticesList[i]);
        } else {
            printf("         到%c：%d\n", G.VerticesList[i], dist[i]);
        }
    }

    // 4. 路径还原：从目标顶点沿path[]逆向回源点，再倒序输出
    printf("       各顶点最短路径：\n");
    for(int i = 0; i < G.n; i++) {
        if(i == v0 || dist[i] == INF) continue;
        int tmp[MAXV], cnt = 0;
        int p = i;
        while(p != -1) {         // ★ 逆向前驱链（从目标到源点）
            tmp[cnt++] = p;
            p = path[p];
        }
        printf("         %c", G.VerticesList[v0]);   // 先打源点
        for(int k = cnt - 2; k >= 0; k--) {   // ★ 跳过最后一个（就是源点自身），倒序输出
            printf(" -> %c", G.VerticesList[tmp[k]]);
        }
        printf("，长度%d\n", dist[i]);
    }
}

// ======4. 构建测试图（视频064的教材例图，5顶点有向带权）======
/*
 * 顶点：v0 v1 v2 v3 v4
 * 弧：v0->v1:10  v0->v4:5  v1->v2:1  v2->v4:3?（视频用v2->v1:1? 见下）
 * 教材例：v0->v1=10, v0->v4=5, v4->v1=3, v4->v2=9, v4->v3=2, v3->v2=6, v2->v1=1
 * 手算期望：
 *   dist[v4]=5 -> 松弛出边：v1=8(path=4), v2=14, v3=7
 *   确定v3=7 -> 松弛：v2=min(14, 7+6)=13（path=3）
 *   确定v1=8 -> 松弛：v1的出边只有?（本图v1无出边，v2保持13）
 *   确定v2=13
 *   最终：v1=8, v2=13, v3=7, v4=5
 * 说明：本图与王道教材例图一致（v2->v1:1 表示v2到v1，方向别记反），
 *       所以到v2的最短路径是 0->4->3->2 = 13（若v1能到v2才是9）
 */
void BuildGraph(MGraph &G) {
    InitGraph(G);
    AddVertex(G, '0'); AddVertex(G, '1'); AddVertex(G, '2');
    AddVertex(G, '3'); AddVertex(G, '4');
    AddArc(G, '0', '1', 10);
    AddArc(G, '0', '4', 5);
    AddArc(G, '4', '1', 3);
    AddArc(G, '4', '2', 9);
    AddArc(G, '4', '3', 2);
    AddArc(G, '3', '2', 6);
    AddArc(G, '2', '1', 1);
}

// ======主函数：测试Dijkstra======
int main() {
    printf("====== 最短路径 Dijkstra 测试开始 ======\n\n");

    MGraph G;
    BuildGraph(G);
    printf("【测试1】Dijkstra 从顶点0出发\n");
    printf("       图弧：0->1:10 0->4:5 4->1:3 4->2:9 4->3:2 3->2:6 2->1:1\n");
    printf("       期望结果：到1=8（经4），到2=13（经4,3），到3=7（经4），到4=5\n");
    printf("       期望确定顺序：4(5) -> 3(7) -> 1(8) -> 2(13)\n");
    Dijkstra(G, 0);

    printf("\n【测试2】Dijkstra 从顶点1出发（演示不同源点）\n");
    Dijkstra(G, 1);

    printf("\n【测试3】负权边演示（为什么Dijkstra不能处理负权）\n");
    printf("       图：0->1:10, 0->2:7, 1->2:-5\n");
    printf("       真相：0->1->2 = 10+(-5) = 5 < 直连7，最短应到2=5\n");
    printf("       Dijkstra第一轮会确定dist[2]=7（贪心认为7已经最小），\n");
    printf("       之后发现经过1更短也来不及改 → 结果错误。所以Dijkstra要求权值非负。\n");

    printf("\n====== 最短路径 Dijkstra 测试结束 ======\n");
    return 0;
}
