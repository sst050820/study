/*
 * ============================================================
 * 文件名：08-拓扑排序.cpp
 * 章  节：第六章 图
 * 内  容：AOV网 + 拓扑排序（入度数组 + 栈 + 判环）
 * 重要度：★★★★ 熟练掌握（手算+代码骨架都要会）
 * 考研考点：反复选入度为0的顶点、删除出边（入度-1）、count<n说明有环、O(V+E)
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXV 100    // 最大顶点数

// ======1. 邻接表存储结构（AOV网用顶点表示活动）======
typedef struct ArcNode {
    int adjvex;                  // 邻接顶点编号（该活动指向的后继活动）
    struct ArcNode *next;
} ArcNode;

typedef struct VNode {
    char data;                   // 顶点信息（活动名）
    ArcNode *first;              // 第一条出边
} VNode;

typedef struct {
    VNode adjlist[MAXV];
    int n, e;
} ALGraph;

// ======2. 数组模拟栈（存当前入度为0的顶点，用队列也行）======
typedef struct {
    int data[MAXV];
    int top;                     // 栈顶指针
} Stack;

void InitStack(Stack &S) {
    S.top = -1;
}

bool Push(Stack &S, int x) {
    if(S.top >= MAXV - 1) return false;
    S.data[++S.top] = x;         // 先加后存
    return true;
}

bool Pop(Stack &S, int &x) {
    if(S.top == -1) return false;
    x = S.data[S.top--];         // 先取再减
    return true;
}

bool StackEmpty(Stack S) {
    return S.top == -1;
}

// ======3. 拓扑排序（★★★★★ 必背骨架）======
/*
 * 思路：
 *   1. 统计所有顶点的入度（邻接表存有向图时入度要遍历全表，这里直接算）；
 *   2. 入度为0的顶点入栈（这些顶点没有前置依赖，可以先做）；
 *   3. 循环：出栈顶点u输出，删除u的所有出边（即把u的后继入度-1），
 *      后继入度减到0就入栈；
 *   4. 结束时若输出的顶点数count < n，说明图中有环，拓扑排序失败。
 * 复杂度：邻接表 O(V+E)；每个顶点出入栈一次，每条边处理一次。
 */
bool TopologicalSort(ALGraph G, int topo[]) {
    int indegree[MAXV];          // 各顶点入度
    // a. 计算所有顶点的入度（遍历所有顶点的边表）
    for(int i = 0; i < G.n; i++) {
        indegree[i] = 0;
    }
    for(int i = 0; i < G.n; i++) {
        ArcNode *p = G.adjlist[i].first;
        while(p != NULL) {
            indegree[p->adjvex]++;   // i有一条边指向p->adjvex，后者入度+1
            p = p->next;
        }
    }

    // b. 入度为0的顶点入栈
    Stack S;
    InitStack(S);
    for(int i = 0; i < G.n; i++) {
        if(indegree[i] == 0) {
            Push(S, i);          // ★ 无前驱的活动先入栈
        }
    }

    // c. 主循环：出栈 -> 输出 -> 删出边（后继入度-1）-> 减到0入栈
    int count = 0;               // 已输出的顶点数
    while(!StackEmpty(S)) {
        int u;
        Pop(S, u);
        topo[count++] = u;       // ★ 记录拓扑序列
        ArcNode *p = G.adjlist[u].first;
        while(p != NULL) {
            int v = p->adjvex;
            if(--indegree[v] == 0) {   // ★ 删除出边：v的入度-1，减到0就入栈
                Push(S, v);
            }
            p = p->next;
        }
    }

    return count == G.n;         // ★ count<n 说明还有顶点没输出 = 存在环
}

// ======4. 打印拓扑序列 ======
void PrintTopo(ALGraph G, int topo[], int n) {
    for(int i = 0; i < n; i++) {
        if(i > 0) printf(" -> ");
        printf("%c", G.adjlist[topo[i]].data);
    }
    printf("\n");
}

// ======5. 构建测试图 ======
// 辅助：初始化顶点
void InitGraphForTest(ALGraph &G, int n) {
    G.n = n;
    G.e = 0;
    for(int i = 0; i < n; i++) {
        G.adjlist[i].data = (char)('A' + i);
        G.adjlist[i].first = NULL;
    }
}

// 辅助：加有向边（头插）
void AddArcForTest(ALGraph &G, int i, int j) {
    ArcNode *p = (ArcNode *)malloc(sizeof(ArcNode));
    p->adjvex = j;
    p->next = G.adjlist[i].first;
    G.adjlist[i].first = p;
    G.e++;
}

/*
 * 测试图1：DAG（5个顶点，AOV网）
 * 边：A->C, A->D, B->D, B->E, C->E
 * 入度：A=0, B=0, C=1(A), D=2(A,B), E=2(C,B)
 * 拓扑序列：A->B->C->D->E 或 B->A->C->D->E 等（不唯一）
 */
void BuildDAG(ALGraph &G) {
    InitGraphForTest(G, 5);
    AddArcForTest(G, 0, 2);      // A->C
    AddArcForTest(G, 0, 3);      // A->D
    AddArcForTest(G, 1, 3);      // B->D
    AddArcForTest(G, 1, 4);      // B->E
    AddArcForTest(G, 2, 4);      // C->E
}

/*
 * 测试图2：有环图（比测试图1多一条边 D->A，构成环 A->C->E? 实际是 A->D->A 环）
 * 边：A->C, A->D, B->D, B->E, C->E, D->A
 * 环：A -> D -> A（D->A 使A又有了入度）
 * 期望：拓扑排序失败（count < n）
 */
void BuildCycleGraph(ALGraph &G) {
    InitGraphForTest(G, 5);
    AddArcForTest(G, 0, 2);      // A->C
    AddArcForTest(G, 0, 3);      // A->D
    AddArcForTest(G, 1, 3);      // B->D
    AddArcForTest(G, 1, 4);      // B->E
    AddArcForTest(G, 2, 4);      // C->E
    AddArcForTest(G, 3, 0);      // D->A（★ 构成环 A->D->A）
}

// ======主函数：测试拓扑排序======
int main() {
    printf("====== 拓扑排序测试开始 ======\n\n");

    int topo[MAXV];

    ALGraph G1;
    BuildDAG(G1);
    printf("【测试1】DAG 拓扑排序（AOV网）\n");
    printf("       边：A->C, A->D, B->D, B->E, C->E\n");
    printf("       入度：A=0 B=0 C=1 D=2 E=2\n");
    printf("       拓扑序列（期望：A B C D E 或 B A C D E，不唯一）：\n");
    printf("       ");
    if(TopologicalSort(G1, topo)) {
        PrintTopo(G1, topo, G1.n);
        printf("       排序成功（DAG无环）\n");
    } else {
        printf("排序失败，图中有环！\n");
    }

    printf("\n【测试2】有环图（加一条边 D->A 构成环 A->D->A）\n");
    ALGraph G2;
    BuildCycleGraph(G2);
    printf("       期望：输出前count<n，返回false，提示有环\n");
    printf("       ");
    if(TopologicalSort(G2, topo)) {
        PrintTopo(G2, topo, G2.n);
        printf("       排序成功\n");
    } else {
        printf("       排序失败！图中存在环（A->D->A），工程无法进行\n");
    }

    printf("\n【测试3】单顶点与链状图（边界）\n");
    ALGraph G3;
    InitGraphForTest(G3, 4);
    AddArcForTest(G3, 0, 1);     // A->B
    AddArcForTest(G3, 1, 2);     // B->C
    AddArcForTest(G3, 2, 3);     // C->D（一条链）
    printf("       链状图 A->B->C->D 拓扑序列：");
    if(TopologicalSort(G3, topo)) {
        PrintTopo(G3, topo, G3.n);
    } else {
        printf("排序失败！\n");
    }

    printf("\n====== 拓扑排序测试结束 ======\n");
    return 0;
}
