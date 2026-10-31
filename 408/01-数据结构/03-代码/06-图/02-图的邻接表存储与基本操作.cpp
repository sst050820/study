/*
 * ============================================================
 * 文件名：02-图的邻接表存储与基本操作.cpp
 * 章  节：第六章 图
 * 内  容：邻接表存储 + 求度/出度/入度 + 判边 + FirstNeighbor/NextNeighbor
 * 重要度：★★★★★ 必须熟练掌握（邻接表是BFS/DFS和拓扑排序的常用存储）
 * 考研考点：无向边存2份、有向图找入边麻烦、表示不唯一、空间O(n+e)
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXV 100    // 最大顶点数

// ======1. 边（弧）结点：存"邻接顶点编号" ======
typedef struct ArcNode {
    int adjvex;                  // 该边指向的顶点编号（在顶点表中的下标）
    struct ArcNode *next;        // 指向下一条边（弧）
    // InfoType info;            // 边权值（带权图才需要，这里省略）
} ArcNode;

// ======2. 顶点结点：数据 + 指向第一条边的指针 ======
typedef struct VNode {
    char data;                   // 顶点信息
    ArcNode *first;              // 指向该顶点的第一条边（弧）
} VNode;

// ======3. 图的邻接表存储结构 ======
typedef struct {
    VNode adjlist[MAXV];         // 顶点表（数组，顺序存储）
    int n, e;                    // 当前顶点数、边数
} ALGraph;

// ======4. 初始化空图 ======
void InitGraph(ALGraph &G) {
    G.n = 0;
    G.e = 0;
    for(int i = 0; i < MAXV; i++) {
        G.adjlist[i].data = '\0';
        G.adjlist[i].first = NULL;    // 每个顶点的边表都置空
    }
}

// ======5. 添加顶点 ======
bool AddVertex(ALGraph &G, char v) {
    if(G.n >= MAXV) {
        return false;
    }
    G.adjlist[G.n].data = v;          // 写入顶点表末尾
    G.adjlist[G.n].first = NULL;      // 新顶点还没有边
    G.n++;
    return true;
}

// ======6. 按顶点值查下标 ======
int LocateVex(ALGraph G, char v) {
    for(int i = 0; i < G.n; i++) {
        if(G.adjlist[i].data == v) {
            return i;
        }
    }
    return -1;
}

// ======7. 添加一条无向边（每条边存2份，用头插法 O(1)）======
/*
 * 头插法：新边结点插到链表头部（比尾插省时间，只需O(1)）
 * 无向图：u和v的边表里各存一份（u->v 和 v->u）
 */
bool AddEdge(ALGraph &G, char u, char v) {
    int i = LocateVex(G, u);
    int j = LocateVex(G, v);
    if(i == -1 || j == -1 || i == j) {
        return false;
    }
    // 在u的边表头插一条指向v的边
    ArcNode *p1 = (ArcNode *)malloc(sizeof(ArcNode));
    p1->adjvex = j;                    // 指向v
    p1->next = G.adjlist[i].first;     // ★ 头插：新结点next指向原来的第一条边
    G.adjlist[i].first = p1;           // 顶点i的first指向新结点
    // 在v的边表头插一条指向u的边（无向边要存2份）
    ArcNode *p2 = (ArcNode *)malloc(sizeof(ArcNode));
    p2->adjvex = i;                    // 指向u
    p2->next = G.adjlist[j].first;
    G.adjlist[j].first = p2;
    G.e++;                             // 边数只加1（虽然存了2个边结点）
    return true;
}

// ======8. 添加一条有向边（弧，只存1份，头插法）======
bool AddArc(ALGraph &G, char u, char v) {
    int i = LocateVex(G, u);
    int j = LocateVex(G, v);
    if(i == -1 || j == -1 || i == j) {
        return false;
    }
    ArcNode *p = (ArcNode *)malloc(sizeof(ArcNode));
    p->adjvex = j;                     // 弧<u,v>：u的边表里存指向v的结点
    p->next = G.adjlist[i].first;
    G.adjlist[i].first = p;
    G.e++;
    return true;
}

// ======9. 求无向图顶点的度（数该顶点边表的结点个数）======
int Degree(ALGraph G, char v) {
    int i = LocateVex(G, v);
    if(i == -1) {
        return -1;
    }
    int cnt = 0;
    ArcNode *p = G.adjlist[i].first;
    while(p != NULL) {                 // 遍历v的边表
        cnt++;
        p = p->next;
    }
    return cnt;
}

// ======10. 求有向图顶点的出度（边表结点个数，方便）======
int OutDegree(ALGraph G, char v) {
    return Degree(G, v);               // 邻接表的边表存的就是出边，数结点个数即可
}

// ======11. 求有向图顶点的入度（★ 必须遍历整个邻接表，麻烦）======
/*
 * 邻接表存有向图的缺点：边表里只存出边，
 * 想找"指向某顶点的弧"只能把所有顶点的边表全部扫一遍
 */
int InDegree(ALGraph G, char v) {
    int j = LocateVex(G, v);
    if(j == -1) {
        return -1;
    }
    int cnt = 0;
    for(int i = 0; i < G.n; i++) {     // ★ 遍历所有顶点
        ArcNode *p = G.adjlist[i].first;
        while(p != NULL) {
            if(p->adjvex == j) {       // 有一条边指向v
                cnt++;
            }
            p = p->next;
        }
    }
    return cnt;
}

// ======12. 判断边(u,v)是否存在（扫描u的边表）======
bool Adjacent(ALGraph G, char u, char v) {
    int i = LocateVex(G, u);
    int j = LocateVex(G, v);
    if(i == -1 || j == -1) {
        return false;
    }
    ArcNode *p = G.adjlist[i].first;
    while(p != NULL) {
        if(p->adjvex == j) {           // 在u的边表里找到了v
            return true;
        }
        p = p->next;
    }
    return false;
}

// ======13. 求顶点v的第一个邻接点（边表第一个结点，O(1)）======
int FirstNeighbor(ALGraph G, char v) {
    int i = LocateVex(G, v);
    if(i == -1) {
        return -1;
    }
    if(G.adjlist[i].first != NULL) {
        return G.adjlist[i].first->adjvex;   // ★ 边表第一个结点就是第一个邻接点
    }
    return -1;
}

// ======14. 求顶点v在邻接点w之后的下一个邻接点（边表往后找）======
int NextNeighbor(ALGraph G, char v, int w) {
    int i = LocateVex(G, v);
    if(i == -1) {
        return -1;
    }
    ArcNode *p = G.adjlist[i].first;
    while(p != NULL && p->adjvex != w) {     // 先找到w这个结点
        p = p->next;
    }
    if(p != NULL && p->next != NULL) {
        return p->next->adjvex;              // ★ w的下一个结点就是下一个邻接点
    }
    return -1;
}

// ======15. 打印邻接表（工具函数）======
void PrintGraph(ALGraph G) {
    for(int i = 0; i < G.n; i++) {
        printf("       %c -> ", G.adjlist[i].data);
        ArcNode *p = G.adjlist[i].first;
        while(p != NULL) {
            printf("%c ", G.adjlist[p->adjvex].data);   // 打印邻接顶点
            p = p->next;
        }
        printf("\n");
    }
}

// ======主函数：测试所有操作======
int main() {
    printf("====== 邻接表存储与基本操作测试开始 ======\n\n");

    ALGraph G;                           // 无向图G
    InitGraph(G);

    printf("【测试1】添加顶点 A B C D E\n");
    AddVertex(G, 'A');
    AddVertex(G, 'B');
    AddVertex(G, 'C');
    AddVertex(G, 'D');
    AddVertex(G, 'E');
    printf("       当前顶点数 = %d（期望5）\n", G.n);

    printf("\n【测试2】添加无向边 A-B A-C A-D B-E C-E\n");
    AddEdge(G, 'A', 'B');
    AddEdge(G, 'A', 'C');
    AddEdge(G, 'A', 'D');
    AddEdge(G, 'B', 'E');
    AddEdge(G, 'C', 'E');
    printf("       当前边数 = %d（期望5，虽然边结点存了10个）\n", G.e);
    printf("       邻接表如下（注意每个边存了2份）：\n");
    PrintGraph(G);

    printf("\n【测试3】求无向图顶点的度\n");
    printf("       顶点A的度 = %d（期望3）\n", Degree(G, 'A'));
    printf("       顶点E的度 = %d（期望2）\n", Degree(G, 'E'));

    printf("\n【测试4】判断边\n");
    printf("       边(A,B)是否存在？%s（期望存在）\n", Adjacent(G, 'A', 'B') ? "是" : "否");
    printf("       边(B,C)是否存在？%s（期望不存在）\n", Adjacent(G, 'B', 'C') ? "是" : "否");

    printf("\n【测试5】FirstNeighbor / NextNeighbor\n");
    int w = FirstNeighbor(G, 'A');       // A的边表第一个邻接点（头插法，可能是D）
    printf("       A的第一个邻接点下标 = %d\n", w);
    int w2 = NextNeighbor(G, 'A', w);
    printf("       A的下一个邻接点下标 = %d\n", w2);

    printf("\n【测试6】有向图：出度入度（演示邻接表找入度要遍历全表）\n");
    ALGraph DG;                          // 有向图DG
    InitGraph(DG);
    AddVertex(DG, 'a');
    AddVertex(DG, 'b');
    AddVertex(DG, 'c');
    AddVertex(DG, 'd');
    AddArc(DG, 'a', 'b');                // a -> b
    AddArc(DG, 'a', 'c');                // a -> c
    AddArc(DG, 'b', 'd');                // b -> d
    AddArc(DG, 'c', 'a');                // c -> a
    AddArc(DG, 'd', 'c');                // d -> c
    printf("       有向图邻接表：\n");
    PrintGraph(DG);
    printf("       顶点a的出度 = %d（期望2）\n", OutDegree(DG, 'a'));
    printf("       顶点a的入度 = %d（期望1：c->a）\n", InDegree(DG, 'a'));
    printf("       顶点c的入度 = %d（期望2：a->c, d->c）\n", InDegree(DG, 'c'));

    printf("\n====== 邻接表存储与基本操作测试结束 ======\n");
    return 0;
}
