/*
 * ============================================================
 * 文件名：09-关键路径.cpp
 * 章  节：第六章 图
 * 内  容：AOE网 + 事件最早ve + 事件最迟vl + 活动最早e/最迟l + 关键活动判定
 * 重要度：★★★ 熟练掌握（手算必会，代码理解为主；大题常考手算填表）
 * 考研考点：ve正向取大、vl反向取小、e=ve[弧尾]、l=vl[弧头]-权、余量为0是关键活动
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXV 100        // 最大顶点数
#define MAXE 500        // 最大边数

// ======1. 边的结构体（AOE网：边表示活动，边权=活动耗时）======
typedef struct {
    int u, v;            // 活动从事件u到事件v
    int w;               // 活动耗时（边权）
} AOEArc;

// ======2. 图结构（用边集数组 + 邻接关系数组实现，便于求ve/vl）======
typedef struct {
    int n, e;                    // 顶点数（事件数）、边数（活动数）
    char V[MAXV];                // 事件名
    AOEArc arcs[MAXE];           // 边集（活动集）
    int inDegree[MAXV];          // 各事件入度（求拓扑序）
} AOEGraph;

// ======3. 拓扑排序（求事件序列，关键路径第一步）======
/*
 * AOE网必须是无环的（DAG），先做拓扑排序拿到事件顺序，
 * 之后按拓扑序正向算ve、反向算vl。
 */
bool TopoOrder(AOEGraph G, int order[]) {
    int indegree[MAXV];
    for(int i = 0; i < G.n; i++) indegree[i] = G.inDegree[i];

    // 数组模拟队列：存当前入度为0的事件
    int queue[MAXV], front = 0, rear = 0;
    for(int i = 0; i < G.n; i++) {
        if(indegree[i] == 0) queue[rear++] = i;
    }
    int count = 0;
    while(front < rear) {
        int u = queue[front++];      // 出队
        order[count++] = u;          // 记录拓扑序
        for(int k = 0; k < G.e; k++) {
            if(G.arcs[k].u == u) {   // 找到u的所有出边<u,v>
                int v = G.arcs[k].v;
                if(--indegree[v] == 0) queue[rear++] = v;   // 入度减到0入队
            }
        }
    }
    return count == G.n;             // false说明有环
}

// ======4. 关键路径计算（★★★★ 必背公式）======
/*
 * 四个量：
 *   ve[i]：事件i的"最早"发生时间 —— 拓扑序正向，取max
 *          ve[k] = max{ ve[j] + w(j,k) }，源点ve=0
 *   vl[i]：事件i的"最迟"发生时间 —— 逆拓扑序反向，取min
 *          vl[k] = min{ vl[j] - w(k,j) }，汇点vl=ve
 *   e[i] ：活动i的"最早"开始时间 = ve[弧尾u]
 *   l[i] ：活动i的"最迟"开始时间 = vl[弧头v] - w
 * 时间余量 = l - e；余量为0的活动 = 关键活动 → 连起来是关键路径
 */
void CriticalPath(AOEGraph G) {
    int ve[MAXV], vl[MAXV];
    int e[MAXE], l[MAXE];
    int order[MAXV];

    // a. 拓扑排序（必须先做）
    if(!TopoOrder(G, order)) {
        printf("       AOE网有环，不存在关键路径！\n");
        return;
    }

    // b. 求ve：按拓扑序正向，取max
    for(int i = 0; i < G.n; i++) ve[i] = 0;      // 初始全0（源点最早=0）
    for(int i = 0; i < G.n; i++) {
        int u = order[i];                        // 按拓扑序取事件u
        for(int k = 0; k < G.e; k++) {
            if(G.arcs[k].u == u) {               // 活动<u,v>
                int v = G.arcs[k].v;
                if(ve[u] + G.arcs[k].w > ve[v]) {
                    ve[v] = ve[u] + G.arcs[k].w; // ★ 取max：所有前驱都完成才发生
                }
            }
        }
    }

    // c. 求vl：按逆拓扑序反向，取min
    for(int i = 0; i < G.n; i++) vl[i] = ve[G.n - 1];  // 初始：汇点vl=ve
    for(int i = G.n - 1; i >= 0; i--) {          // ★ 逆拓扑序
        int u = order[i];
        for(int k = 0; k < G.e; k++) {
            if(G.arcs[k].u == u) {               // 活动<u,v>
                int v = G.arcs[k].v;
                if(vl[v] - G.arcs[k].w < vl[u]) {
                    vl[u] = vl[v] - G.arcs[k].w; // ★ 取min：所有后继都来得及
                }
            }
        }
    }

    // d. 求每条活动的 e 和 l，并标记关键活动
    printf("       各活动参数（活动  e  l  余量  是否关键）：\n");
    int keyCnt = 0;
    for(int k = 0; k < G.e; k++) {
        int u = G.arcs[k].u, v = G.arcs[k].v, w = G.arcs[k].w;
        e[k] = ve[u];            // ★ 活动最早 = 弧尾事件最早
        l[k] = vl[v] - w;        // ★ 活动最迟 = 弧头事件最迟 - 耗时
        int rest = l[k] - e[k];  // 时间余量
        bool isKey = (rest == 0);
        if(isKey) keyCnt++;
        printf("         %c-%c:%d   %2d %2d  %2d    %s\n",
               G.V[u], G.V[v], w, e[k], l[k], rest, isKey ? "★关键活动" : "");
    }

    // e. 输出事件最早/最迟
    printf("       各事件ve（最早）/vl（最迟）：\n");
    for(int i = 0; i < G.n; i++) {
        printf("         %c: ve=%d vl=%d %s\n", G.V[i], ve[i], vl[i],
               (ve[i] == vl[i]) ? "（关键事件）" : "");
    }
    printf("       关键活动数 = %d，工程最短完成时间 = %d\n",
           keyCnt, ve[G.n - 1]);
}

// ======5. 构建测试图（视频068的教材例图AOE网）======
/*
 * AOE网（7个活动）：
 *   v1->v2:3   v1->v3:2   v2->v4:2   v2->v5:3
 *   v3->v4:4   v4->v6:2   v5->v6:1
 * 手算期望：
 *   ve：v1=0 v2=3 v3=2 v4=6(max(3+2,2+4)) v5=6(3+3) v6=8(max(6+2,6+1))
 *   vl：v6=8 v5=7(8-1) v4=6(8-2) v3=2(6-4) v2=4(min(6-2,7-3)) v1=0
 *   e:  a1=0 a2=0 a3=3 a4=3 a5=2 a6=6 a7=6
 *   l:  a1=1 a2=0 a3=4 a4=4 a5=2 a6=6 a7=7
 *   余量: a1=1 a2=0 a3=1 a4=1 a5=0 a6=0 a7=1
 *   关键活动：a2(v1->v3), a5(v3->v4), a6(v4->v6)
 *   关键路径：v1 -> v3 -> v4 -> v6，工期=8
 */
void BuildAOE(AOEGraph &G) {
    G.n = 6;
    G.e = 7;
    for(int i = 0; i < G.n; i++) {
        G.V[i] = (char)('1' + i);   // 事件名：v1 v2 v3 v4 v5 v6
        G.inDegree[i] = 0;
    }
    G.arcs[0] = (AOEArc){0, 1, 3};   // v1->v2: 3
    G.arcs[1] = (AOEArc){0, 2, 2};   // v1->v3: 2
    G.arcs[2] = (AOEArc){1, 3, 2};   // v2->v4: 2
    G.arcs[3] = (AOEArc){1, 4, 3};   // v2->v5: 3
    G.arcs[4] = (AOEArc){2, 3, 4};   // v3->v4: 4
    G.arcs[5] = (AOEArc){3, 5, 2};   // v4->v6: 2
    G.arcs[6] = (AOEArc){4, 5, 1};   // v5->v6: 1
    // 计算入度
    for(int k = 0; k < G.e; k++) {
        G.inDegree[G.arcs[k].v]++;
    }
}

// ======主函数：测试关键路径======
int main() {
    printf("====== 关键路径 AOE网 测试开始 ======\n\n");

    AOEGraph G;
    BuildAOE(G);
    printf("【测试1】AOE网关键路径计算\n");
    printf("       活动：v1->v2:3 v1->v3:2 v2->v4:2 v2->v5:3 v3->v4:4 v4->v6:2 v5->v6:1\n");
    printf("       期望关键路径：v1 -> v3 -> v4 -> v6\n");
    printf("       期望关键活动：v1->v3(2), v3->v4(4), v4->v6(2)\n");
    printf("       期望最短工期 = 8\n\n");
    CriticalPath(G);

    printf("\n====== 关键路径 AOE网 测试结束 ======\n");
    return 0;
}
