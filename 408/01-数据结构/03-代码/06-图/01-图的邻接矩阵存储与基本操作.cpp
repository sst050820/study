/*
 * ============================================================
 * 文件名：01-图的邻接矩阵存储与基本操作.cpp
 * 章  节：第六章 图
 * 内  容：邻接矩阵存储 + 求度/出度/入度 + 判边 + 插删顶点 + FirstNeighbor/NextNeighbor
 * 重要度：★★★★★ 必须熟练掌握（邻接矩阵是BFS/DFS和Dijkstra/Floyd的存储基础）
 * 考研考点：矩阵对称性、求度的行/列法、O(1)判边、删除顶点用置空标记
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXV 100    // 最大顶点数（矩阵大小固定，先定义好）
#define INF 0x7fffffff  // 表示"无穷"（带权图中表示无边）

// ======1. 图的邻接矩阵存储结构（★★★★★ 必背）======
/*
 * 结构说明：
 *   - VerticesList：一维数组，存顶点信息（这里用char：'A','B','C'...）
 *   - Edge[][]     ：二维数组，Edge[i][j]=1 表示顶点i和j有边（无权图）
 *                     带权图存权值，无边存INF
 *   - numVertices  ：当前顶点数
 *   - numEdges     ：当前边数
 * 特点：n个顶点的图，空间O(n^2)；判断任意两边是否有边只需O(1)
 */
typedef struct {
    int numVertices;          // 当前顶点数
    int numEdges;             // 当前边数
    char VerticesList[MAXV];  // 顶点表（存放各个顶点的信息）
    int Edge[MAXV][MAXV];     // 邻接矩阵（Edge[i][j]表示顶点i与j的邻接关系）
} MGraph;

// ======2. 初始化一个空图（所有边置0）======
void InitGraph(MGraph &G) {
    G.numVertices = 0;        // 顶点数先置0
    G.numEdges = 0;           // 边数先置0
    for(int i = 0; i < MAXV; i++) {
        G.VerticesList[i] = '\0';        // 顶点表清空
        for(int j = 0; j < MAXV; j++) {
            G.Edge[i][j] = 0;            // ★ 所有边先置0（"无边"状态）
        }
    }
}

// ======3. 添加一个顶点（O(1)，插在顶点表末尾）======
bool AddVertex(MGraph &G, char v) {
    if(G.numVertices >= MAXV) {          // 顶点表满了
        return false;
    }
    G.VerticesList[G.numVertices] = v;   // 写在数组末尾
    G.numVertices++;                     // 顶点数+1
    return true;
}

// ======4. 按顶点值查下标（工具函数：把'A'转成0号）======
int LocateVex(MGraph G, char v) {
    for(int i = 0; i < G.numVertices; i++) {
        if(G.VerticesList[i] == v) {     // 找到匹配的顶点
            return i;                    // 返回其下标
        }
    }
    return -1;                           // 找不到返回-1
}

// ======5. 添加一条无向边（对称位置都置1，O(1)）======
bool AddEdge(MGraph &G, char u, char v) {
    int i = LocateVex(G, u);             // 找u的下标
    int j = LocateVex(G, v);             // 找v的下标
    if(i == -1 || j == -1 || i == j) {   // 顶点不存在或自环（简单图不允许自环）
        return false;
    }
    if(G.Edge[i][j] == 1) {              // 边已存在，不重复加
        return false;
    }
    G.Edge[i][j] = 1;                    // ★ 无向图：i行j列置1
    G.Edge[j][i] = 1;                    // ★ 对称位置也置1（每条无向边对应两个1）
    G.numEdges++;                        // 边数只加1（不是2！）
    return true;
}

// ======6. 判断边(u,v)是否存在（O(1)，邻接矩阵最大优势）======
bool Adjacent(MGraph G, char u, char v) {
    int i = LocateVex(G, u);
    int j = LocateVex(G, v);
    if(i == -1 || j == -1) {
        return false;
    }
    return G.Edge[i][j] == 1;            // ★ 直接查矩阵元素，O(1)
}

// ======7. 求顶点的度（无向图：数该行/列的非零元素个数，O(n)）======
int Degree(MGraph G, char v) {
    int i = LocateVex(G, v);
    if(i == -1) {
        return -1;
    }
    int cnt = 0;
    for(int j = 0; j < G.numVertices; j++) {
        if(G.Edge[i][j] == 1) {          // 遍历第i行，数1的个数
            cnt++;
        }
    }
    return cnt;
}

// ======8. 求有向图顶点的出度（第i行1的个数）======
int OutDegree(MGraph G, char v) {
    int i = LocateVex(G, v);
    if(i == -1) {
        return -1;
    }
    int cnt = 0;
    for(int j = 0; j < G.numVertices; j++) {
        if(G.Edge[i][j] == 1) {          // ★ 行 → 出度（从该顶点发射的弧）
            cnt++;
        }
    }
    return cnt;
}

// ======9. 求有向图顶点的入度（第i列1的个数）======
int InDegree(MGraph G, char v) {
    int i = LocateVex(G, v);
    if(i == -1) {
        return -1;
    }
    int cnt = 0;
    for(int j = 0; j < G.numVertices; j++) {
        if(G.Edge[j][i] == 1) {          // ★ 列 → 入度（指向该顶点的弧）
            cnt++;
        }
    }
    return cnt;
}

// ======10. 求顶点v的第一个邻接点（扫描第v行找第一个1，O(n)）======
/*
 * 这是遍历算法的地基操作（BFS/DFS都要用），必须理解：
 * 找不到返回-1；找到返回该邻接点的下标
 */
int FirstNeighbor(MGraph G, char v) {
    int i = LocateVex(G, v);
    if(i == -1) {
        return -1;
    }
    for(int j = 0; j < G.numVertices; j++) {
        if(G.Edge[i][j] == 1) {          // ★ 从左到右扫第i行，找到第一个1
            return j;                    // 返回邻接点下标
        }
    }
    return -1;                           // 没有邻接点
}

// ======11. 求顶点v在邻接点w之后的下一个邻接点（继续往后扫）======
int NextNeighbor(MGraph G, char v, int w) {
    int i = LocateVex(G, v);
    if(i == -1) {
        return -1;
    }
    for(int j = w + 1; j < G.numVertices; j++) {
        if(G.Edge[i][j] == 1) {          // ★ 从w的下一个位置继续找
            return j;
        }
    }
    return -1;                           // 后面没有了
}

// ======12. 删除顶点（置空标记法，O(n)，不移动大量元素）======
/*
 * 为什么不把后面顶点前移？因为邻接矩阵有n^2个元素，
 * 移动会导致大量开销。标准做法：把该顶点所在行和列全部置0，
 * 再在顶点表中打"空"标记（这里用'\0'表示该位置已空）
 */
bool DeleteVertex(MGraph &G, char v) {
    int i = LocateVex(G, v);
    if(i == -1) {
        return false;
    }
    // 统计被删边数：第i行非零个数就是该顶点的度（无向图）
    int cnt = 0;
    for(int j = 0; j < G.numVertices; j++) {
        if(G.Edge[i][j] == 1) {
            cnt++;
        }
    }
    // 把第i行、第i列全部置0（相当于删掉该顶点及其所有关联边）
    for(int j = 0; j < G.numVertices; j++) {
        G.Edge[i][j] = 0;                // 第i行清0
        G.Edge[j][i] = 0;                // 第i列清0
    }
    G.VerticesList[i] = '\0';            // 顶点表打空标记（该位置以后不再使用）
    G.numVertices--;                     // 顶点数-1
    G.numEdges -= cnt;                   // 边数减去该顶点的度
    return true;
}

// ======13. 打印邻接矩阵和顶点信息（工具函数）======
void PrintGraph(MGraph G) {
    printf("       顶点表：");
    for(int i = 0; i < G.numVertices; i++) {
        if(G.VerticesList[i] != '\0') {
            printf("%c ", G.VerticesList[i]);   // 只打印非空顶点
        }
    }
    printf("\n");
    printf("       邻接矩阵（行=起点，列=终点）：\n");
    for(int i = 0; i < G.numVertices; i++) {
        printf("         %c |", G.VerticesList[i]);
        for(int j = 0; j < G.numVertices; j++) {
            printf(" %d", G.Edge[i][j]);
        }
        printf("\n");
    }
}

// ======主函数：测试所有操作======
int main() {
    printf("====== 邻接矩阵存储与基本操作测试开始 ======\n\n");

    MGraph G;                            // 定义图G
    InitGraph(G);                        // 初始化

    printf("【测试1】添加顶点 A B C D E（期望：5个顶点）\n");
    AddVertex(G, 'A');
    AddVertex(G, 'B');
    AddVertex(G, 'C');
    AddVertex(G, 'D');
    AddVertex(G, 'E');
    printf("       当前顶点数 = %d（期望5）\n", G.numVertices);

    printf("\n【测试2】添加无向边 A-B A-C A-D B-E C-E（期望：边数=5）\n");
    AddEdge(G, 'A', 'B');
    AddEdge(G, 'A', 'C');
    AddEdge(G, 'A', 'D');
    AddEdge(G, 'B', 'E');
    AddEdge(G, 'C', 'E');
    printf("       当前边数 = %d（期望5，注意无向图每条边矩阵里有2个1但不重复计数）\n", G.numEdges);
    PrintGraph(G);

    printf("\n【测试3】判断边是否存在（O(1)）\n");
    printf("       边(A,B)是否存在？%s（期望存在）\n", Adjacent(G, 'A', 'B') ? "是" : "否");
    printf("       边(B,C)是否存在？%s（期望不存在）\n", Adjacent(G, 'B', 'C') ? "是" : "否");

    printf("\n【测试4】求各顶点度（无向图：数行中1的个数）\n");
    printf("       顶点A的度 = %d（期望3：连B,C,D）\n", Degree(G, 'A'));
    printf("       顶点B的度 = %d（期望2：连A,E）\n", Degree(G, 'B'));
    printf("       顶点E的度 = %d（期望2：连B,C）\n", Degree(G, 'E'));

    printf("\n【测试5】FirstNeighbor / NextNeighbor（遍历算法的地基）\n");
    int w = FirstNeighbor(G, 'A');       // A的第一个邻接点（矩阵从左到右扫）
    printf("       A的第一个邻接点下标 = %d（期望1，即B）\n", w);
    int w2 = NextNeighbor(G, 'A', w);    // A在B之后的下一个邻接点
    printf("       A在B之后的下一个邻接点下标 = %d（期望2，即C）\n", w2);

    printf("\n【测试6】删除顶点D（置空标记法）\n");
    DeleteVertex(G, 'D');                // D只连A，删掉它
    printf("       删除D后顶点数 = %d（期望4）\n", G.numVertices);
    printf("       删除D后边数 = %d（期望4：原来的5条减去D的1条）\n", G.numEdges);
    printf("       删除D后顶点A的度 = %d（期望2：只连B,C）\n", Degree(G, 'A'));

    printf("\n【测试7】有向图的出度/入度（另建一个有向图演示）\n");
    MGraph DG;                           // 有向图DG
    InitGraph(DG);
    AddVertex(DG, 'a');
    AddVertex(DG, 'b');
    AddVertex(DG, 'c');
    AddVertex(DG, 'd');
    // 手动给矩阵赋值：a->b, a->c, b->d, c->a, d->c（每条弧只置一个1）
    DG.Edge[0][1] = 1;                   // a -> b
    DG.Edge[0][2] = 1;                   // a -> c
    DG.Edge[1][3] = 1;                   // b -> d
    DG.Edge[2][0] = 1;                   // c -> a
    DG.Edge[3][2] = 1;                   // d -> c
    DG.numEdges = 5;
    printf("       顶点a的出度 = %d（期望2：a->b, a->c）\n", OutDegree(DG, 'a'));
    printf("       顶点a的入度 = %d（期望1：c->a）\n", InDegree(DG, 'a'));
    printf("       顶点c的出度 = %d（期望1：c->a）\n", OutDegree(DG, 'c'));
    printf("       顶点c的入度 = %d（期望2：a->c, d->c）\n", InDegree(DG, 'c'));

    printf("\n====== 邻接矩阵存储与基本操作测试结束 ======\n");
    return 0;
}
