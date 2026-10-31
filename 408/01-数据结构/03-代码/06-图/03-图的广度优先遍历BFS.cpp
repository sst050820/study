/*
 * ============================================================
 * 文件名：03-图的广度优先遍历BFS.cpp
 * 章  节：第六章 图
 * 内  容：邻接表存储下 BFS + 非连通图处理 + 统计连通分量数 + 广度优先生成树边
 * 重要度：★★★★★ 必须熟练掌握（与树的层序遍历同源，算法设计题大本营）
 * 考研考点：队列+visited、入队时标记、非连通外层循环、序列唯一性（矩阵唯一/表不唯一）
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXV 100    // 最大顶点数

// ======1. 邻接表存储结构（沿用02文件，BFS/DFS都以它为存储基础）======
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

// ======2. 循环队列（BFS的辅助结构，★★★★ 必须会用）======
/*
 * 队列用于"按层扩展"：先入队的顶点先扩展其邻接点
 * 用数组模拟循环队列，front指向队头、rear指向队尾的下一个位置
 */
#define MAXQSIZE 100    // 队列最大容量
typedef struct {
    int data[MAXQSIZE];          // 存顶点下标
    int front, rear;             // 队头、队尾指针
} SqQueue;

// 初始化队列
void InitQueue(SqQueue &Q) {
    Q.front = Q.rear = 0;        // 空队列：front==rear
}

// 入队
bool EnQueue(SqQueue &Q, int x) {
    if((Q.rear + 1) % MAXQSIZE == Q.front) {   // 队满判断（牺牲一个位置）
        return false;
    }
    Q.data[Q.rear] = x;          // 写入队尾
    Q.rear = (Q.rear + 1) % MAXQSIZE;          // ★ 循环后移
    return true;
}

// 出队
bool DeQueue(SqQueue &Q, int &x) {
    if(Q.front == Q.rear) {      // 队空
        return false;
    }
    x = Q.data[Q.front];         // 取队头
    Q.front = (Q.front + 1) % MAXQSIZE;        // ★ 循环后移
    return true;
}

// 判队空
bool QueueEmpty(SqQueue Q) {
    return Q.front == Q.rear;
}

// ======3. 图的基本操作（BFS依赖这两个函数）======
int FirstNeighbor(ALGraph G, int v) {
    if(G.adjlist[v].first != NULL) {
        return G.adjlist[v].first->adjvex;     // 第一个邻接点 = 边表第一个结点
    }
    return -1;
}

int NextNeighbor(ALGraph G, int v, int w) {
    ArcNode *p = G.adjlist[v].first;
    while(p != NULL && p->adjvex != w) {       // 先定位到w
        p = p->next;
    }
    if(p != NULL && p->next != NULL) {
        return p->next->adjvex;                // 下一个邻接点
    }
    return -1;
}

// ======4. 访问顶点（打印出来，表示"访问"动作）======
void visit(int v) {
    printf("%c ", 'A' + v);                    // 演示：顶点编号0对应'A'
}

// ======5. BFS 核心算法（★★★★★ 必背）======
/*
 * 思想：树的层序遍历的推广
 * 过程：访问顶点入队 -> 出队 -> 把它的所有"未访问"邻接点访问并入队
 * 要点：visited标记在【入队时】就置true（防止同一顶点被重复入队）
 */
bool visited[MAXV];                            // 访问标记数组（全局）
SqQueue Q;                                     // 全局辅助队列

void BFS(ALGraph G, int v) {
    visit(v);                                  // 1. 访问顶点v
    visited[v] = true;                         // 2. ★ 标记已访问（入队前就标！）
    EnQueue(Q, v);                             // 3. v入队
    while(!QueueEmpty(Q)) {                    // 4. 队列非空时循环
        int u;
        DeQueue(Q, u);                         // 5. 出队一个顶点u
        for(int w = FirstNeighbor(G, u); w >= 0; w = NextNeighbor(G, u, w)) {
            if(!visited[w]) {                  // 6. ★ u的邻接点w未被访问
                visit(w);                      // 7. 访问w
                visited[w] = true;             // 8. ★ 标记（防止重复入队）
                EnQueue(Q, w);                 // 9. w入队
            }
        }
    }
}

// ======6. 非连通图遍历 + 统计连通分量（★★★ 必背，易漏）======
/*
 * 一次BFS只能访问一个连通分量。
 * 外层for扫描visited数组，碰到没访问过的顶点就再启动一次BFS。
 * 调用次数 = 连通分量的个数（选择题高频结论）
 */
int BFSTraverse(ALGraph G) {
    int count = 0;                             // 记录BFS调用次数 = 连通分量数
    for(int i = 0; i < G.n; i++) {
        visited[i] = false;                    // 初始化：全部未访问
    }
    for(int i = 0; i < G.n; i++) {
        if(!visited[i]) {                      // ★ 顶点i还没被访问过
            BFS(G, i);                         // 从i启动一次BFS
            count++;                           // 每启动一次就是一个连通分量
        }
    }
    return count;
}

// ======7. 构建测试图（工具函数，手工连边）======
// 辅助：初始化n个顶点（顶点数据给'A'+i）
void InitGraphForTest(ALGraph &G, int n) {
    G.n = n;
    G.e = 0;
    for(int i = 0; i < n; i++) {
        G.adjlist[i].data = (char)('A' + i);   // 顶点数据直接给'A'+i
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

/*
 * 测试图1：连通图（5个顶点）
 *   A-B, A-C, A-D, B-E, C-E
 *   （这就是055视频讲解图的概念时用的例子）
 */
void BuildConnectedGraph(ALGraph &G) {
    InitGraphForTest(G, 5);
    AddEdgeForTest(G, 0, 1);                   // A-B
    AddEdgeForTest(G, 0, 2);                   // A-C
    AddEdgeForTest(G, 0, 3);                   // A-D
    AddEdgeForTest(G, 1, 4);                   // B-E
    AddEdgeForTest(G, 2, 4);                   // C-E
}

/*
 * 测试图2：非连通图（6个顶点，2个连通分量）
 *   分量1：A-B, A-C       分量2：D-E, E-F, D-F
 *   （用于演示BFSTraverse调用次数=连通分量数）
 */
void BuildDisconnectedGraph(ALGraph &G) {
    InitGraphForTest(G, 6);
    AddEdgeForTest(G, 0, 1);                   // A-B
    AddEdgeForTest(G, 0, 2);                   // A-C
    AddEdgeForTest(G, 3, 4);                   // D-E
    AddEdgeForTest(G, 4, 5);                   // E-F
    AddEdgeForTest(G, 3, 5);                   // D-F
}

// ======主函数：测试BFS======
int main() {
    printf("====== 广度优先遍历BFS测试开始 ======\n\n");

    ALGraph G1;
    BuildConnectedGraph(G1);                   // 连通图
    printf("【测试1】连通图 BFS（从A出发）\n");
    printf("       图：A-B, A-C, A-D, B-E, C-E\n");
    printf("       邻接表（注意头插法，边表顺序与插入相反）：\n");
    for(int i = 0; i < G1.n; i++) {
        printf("         %c -> ", G1.adjlist[i].data);
        ArcNode *p = G1.adjlist[i].first;
        while(p != NULL) {
            printf("%c ", G1.adjlist[p->adjvex].data);
            p = p->next;
        }
        printf("\n");
    }
    printf("       BFS序列：");
    int cnt1 = BFSTraverse(G1);
    printf("\n       连通图BFS调用次数 = %d（期望1，连通图只调1次）\n", cnt1);

    printf("\n【测试2】非连通图 BFS（2个连通分量）\n");
    printf("       图：分量1 = A-B,A-C；分量2 = D-E,E-F,D-F\n");
    ALGraph G2;
    BuildDisconnectedGraph(G2);
    printf("       BFS序列（先访问完分量1再访问分量2）：");
    int cnt2 = BFSTraverse(G2);
    printf("\n       非连通图BFS调用次数 = %d（期望2，等于连通分量数）\n", cnt2);

    printf("\n【测试3】从不同顶点出发序列不同（理解序列与起点有关）\n");
    printf("       从A出发："); 
    for(int i = 0; i < G1.n; i++) visited[i] = false;
    InitQueue(Q);
    BFS(G1, 0);                                // 从A出发
    printf("\n       从E出发：");
    for(int i = 0; i < G1.n; i++) visited[i] = false;
    InitQueue(Q);
    BFS(G1, 4);                                // 从E出发
    printf("\n       （两个序列不同，说明BFS序列依赖起始顶点）\n");

    printf("\n====== 广度优先遍历BFS测试结束 ======\n");
    return 0;
}
