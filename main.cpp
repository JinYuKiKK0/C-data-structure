#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX_NUM 20 // 最大顶点数
#define INFINITY 65535    // 无穷大
#define VRType int        // 顶点关系类型
#define VertexType char   // 顶点类型

typedef enum { DG, DN, UDG, UDN } GraphKind; // {有向图，有向网，无向图，无向网}
typedef struct ArcCell {
    VRType adj; // VRType 是顶点关系类型，对无权图，用1或0表示是否相邻；对带权图，则为权值类型
} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef struct {
    VertexType vexs[MAX_VERTEX_NUM]; // 顶点向量
    AdjMatrix arcs;                  // 邻接矩阵
    int vexnum, arcnum;              // 图的当前顶点数和弧数
    GraphKind kind;                  // 图的种类标志
} MGraph;

int LocateVex(MGraph G, VertexType u) {
    for (int i = 0; i < G.vexnum; ++i)
        if (G.vexs[i] == u)
            return i;
    return -1;
}

void createUDN(MGraph *G) {
    printf("请输入顶点数和边数，以空格隔开：");
    scanf("%d %d", &G->vexnum, &G->arcnum);
    printf("请输入%d个顶点：", G->vexnum);
    for (int i = 0; i < G->vexnum; i++)
        scanf(" %c", &G->vexs[i]);
    for (int i = 0; i < G->vexnum; i++)
        for (int j = 0; j < G->vexnum; j++)
            G->arcs[i][j].adj = INFINITY;
    // 构造邻接矩阵
    for (int k = 0; k < G->arcnum; k++) {
        char v1, v2;
        int w, i, j;
        printf("请输入第%d条边的两个顶点及其上的权值，以空格隔开：", k + 1);
        scanf(" %c %c %d", &v1, &v2, &w);
        i = LocateVex(*G, v1);
        j = LocateVex(*G, v2);
        if (i != -1 && j != -1) {
            G->arcs[i][j].adj = w;
            G->arcs[j][i].adj = w; // 无向图对称赋值
        } else {
            printf("顶点输入错误。\n");
            k--; // 重新输入该条边
        }
    }
    G->kind = UDN;
}

void createDN(MGraph *G) {
    int i, j, k, w;
    printf("请输入顶点数和弧数：");
    scanf("%d %d", &G->vexnum, &G->arcnum);
    printf("请输入%d个顶点：", G->vexnum);
    for (i = 0; i < G->vexnum; i++)
        scanf(" %c", &G->vexs[i]);
    for (i = 0; i < G->vexnum; i++)
        for (j = 0; j < G->vexnum; j++)
            G->arcs[i][j].adj = INFINITY;
    // 构造邻接矩阵
    for (k = 0; k < G->arcnum; k++) {
        char v1, v2;
        printf("请输入第%d条弧的起点,终点和权值，以空格隔开：", k + 1);
        scanf(" %c %c %d", &v1, &v2, &w);
        i = LocateVex(*G, v1);
        j = LocateVex(*G, v2);
        if (i != -1 && j != -1) {
            G->arcs[i][j].adj = w;
        } else {
            printf("顶点输入错误。\n");
            k--; // 重新输入该条弧
        }
    }
    G->kind = DN;
}

void MST_PRIM(MGraph G, VertexType u) {
    int i, j, k;
    struct {
        VertexType adjvex;
        VRType lowcost;
    } closedge[MAX_VERTEX_NUM];

    // 定位起始顶点
    k = LocateVex(G, u);
    if (k == -1) {
        printf("起始顶点不存在。\n");
        return;
    }
    for (j = 0; j < G.vexnum; j++) {
        if (j != k) {
            closedge[j].adjvex = u;
            closedge[j].lowcost = G.arcs[k][j].adj;
        } else {
            closedge[j].lowcost = 0;
        }
    }

    for (i = 1; i < G.vexnum; i++) {
        // 找出lowcost最小的j
        VRType min = INFINITY;
        int minIndex = -1;
        for (j = 0; j < G.vexnum; j++) {
            if (closedge[j].lowcost != 0 && closedge[j].lowcost < min) {
                min = closedge[j].lowcost;
                minIndex = j;
            }
        }
        if (minIndex == -1) {
            printf("图不连通，最小生成树不存在。\n");
            return;
        }
        // 输出选取的边
        printf("边 (%c, %c) 权值 %d\n", closedge[minIndex].adjvex, G.vexs[minIndex], closedge[minIndex].lowcost);
        closedge[minIndex].lowcost = 0;
        // 更新closedge
        for (j = 0; j < G.vexnum; j++) {
            if (closedge[j].lowcost != 0 && G.arcs[minIndex][j].adj < closedge[j].lowcost) {
                closedge[j].adjvex = G.vexs[minIndex];
                closedge[j].lowcost = G.arcs[minIndex][j].adj;
            }
        }
    }
}

void printPath(int parent[], int j, MGraph G) {
    if (parent[j] == -1) {
        printf("%c ", G.vexs[j]);
        return;
    }
    printPath(parent, parent[j], G);
    printf("%c ", G.vexs[j]);
}

void Dijkstra(MGraph G, VertexType src_vertex) {
    int dist[MAX_VERTEX_NUM];    // 源顶点到其他顶点的最短距离
    int sptSet[MAX_VERTEX_NUM];  // sptSet[i]为1表示顶点i已找到最短路径
    int parent[MAX_VERTEX_NUM];  // 记录路径

    for (int i = 0; i < G.vexnum; i++) {
        dist[i] = INFINITY;
        sptSet[i] = 0;
        parent[i] = -1;
    }

    int src = LocateVex(G, src_vertex);
    if (src == -1) {
        printf("源顶点不存在。\n");
        return;
    }
    dist[src] = 0;

    for (int count = 0; count < G.vexnum - 1; count++) {
        int min = INFINITY, min_index = -1;
        for (int v = 0; v < G.vexnum; v++)
            if (sptSet[v] == 0 && dist[v] <= min) {
                min = dist[v];
                min_index = v;
            }

        if (min_index == -1)
            break; // 无法继续更新

        int u_index = min_index;
        sptSet[u_index] = 1;

        for (int v = 0; v < G.vexnum; v++)
            if (sptSet[v] == 0 && G.arcs[u_index][v].adj != INFINITY && dist[u_index] != INFINITY && dist[u_index] + G.arcs[u_index][v].adj < dist[v]) {
                dist[v] = dist[u_index] + G.arcs[u_index][v].adj;
                parent[v] = u_index;
            }
    }

    // 打印结果
    printf("顶点\t距离\t路径");
    for (int i = 0; i < G.vexnum; i++) {
        printf("\n%c -> %c \t%d\t", G.vexs[src], G.vexs[i], dist[i]);
        if (dist[i] != INFINITY) {
            printPath(parent, i, G);
        } else {
            printf("不可达");
        }
    }
    printf("\n");
}

int main() {
    int option = 0;
    MGraph G;
    printf("1. 求解无向网络的最小生成树\n");
    printf("2. 求解有向网络的最短路径\n");
    printf("0. 退出\n");
    printf("请输入你的选择：");
    scanf("%d", &option);
    switch (option) {
    case 1:
        printf("请构建一个邻接矩阵存储的无向网络\n");
        createUDN(&G);
        MST_PRIM(G, G.vexs[0]);
        break;
    case 2:
        printf("请构建一个邻接矩阵存储的有向网络\n");
        createDN(&G);
        Dijkstra(G, G.vexs[0]);
        break;
    case 0:
        printf("程序退出。\n");
        break;
    default:
        printf("输入有误。\n");
        break;
    }
    return 0;
}