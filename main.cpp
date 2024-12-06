#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTEX_NUM 20 // ��󶥵���
#define INFINITY 65535 // �����
#define VRType int // �����ϵ����
#define VertexType char // ��������

typedef enum{DG,DN,UDG,UDN} GraphKind; // {����ͼ��������������ͼ��������}
typedef struct ArcCell{
    VRType  adj;// VRType �Ƕ����ϵ���ͣ�����Ȩͼ����1��0��ʾ�Ƿ����ڣ��Դ�Ȩͼ����ΪȨֵ����
}ArcCell,AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef struct
{
    VertexType vexs[MAX_VERTEX_NUM]; // ��������
    AdjMatrix arcs;  // �ڽӾ���
    int vexnum,arcnum; // ͼ�ĵ�ǰ�������ͻ���
    GraphKind kind; // ͼ�������־
}MGraph;
int LocateVex(MGraph G,VertexType u)
{
    for (int i = 0; i < G.vexnum; ++i)
        if (G.vexs[i] == u)
            return i;
}
void createUDN(MGraph &G)
{
    printf("�����붥�����ͱ������Կո������");
    scanf("%d %d", &G.vexnum, &G.arcnum);
    printf("������%d������", G.vexnum);
    for (int i = 0; i < G.vexnum; i++)  scanf(" %c", &G.vexs[i]);
    for(int i = 0;i<G.vexnum;i++)
        for (int j =0 ;j<G.vexnum;j++)
            G.arcs[i][j].adj = INFINITY;
    //�����ڽӾ���
    for(int k = 0;k<G.arcnum;k++)
    {
        char v1 ,v2;
        int w,i,j;
        printf("�������%d���ߵ��������㼰���ϵ�Ȩֵ���Կո������",k);
        scanf("%c %c %d",&v1,&v2,&w);
        i = LocateVex(G,v1);
        j = LocateVex(G,v2);
        G.arcs[i][j].adj = w;
    }
    G.kind = UDN;

}
void createDN(MGraph &G)
    {
        int i, j, k, w;
        printf("�����붥�����ͻ���: ");
        scanf("%d %d", &G.vexnum, &G.arcnum);
        printf("�����붥����Ϣ: ");
        for (i = 0; i < G.vexnum; i++)
            scanf(" %c", &G.vexs[i]);
        for (i = 0; i < G.vexnum; i++)
            for (j = 0; j < G.vexnum; j++)
                G.arcs[i][j].adj = INFINITY;
        printf("������ÿ���������,�յ��Ȩֵ: ");
        for (k = 0; k < G.arcnum; k++)
        {
            char v1, v2;
            scanf(" %c %c %d", &v1, &v2, &w);
            i = LocateVex(G,v1);
            j = LocateVex(G,v2);
            G.arcs[i][j].adj = w;
        }
        G.kind = DN;
    }
void MST_PRIM(MGraph G, VertexType u)
{
    int i, j, k;
    struct
    {
        VertexType adjvex;
        VRType lowcost;
    } closedge[MAX_VERTEX_NUM];

    // Locate the starting vertex
    k = LocateVex(G, u);
    for (j = 0; j < G.vexnum; j++)
    {
        if (j != k)
        {
            closedge[j].adjvex = u;
            closedge[j].lowcost = G.arcs[k][j].adj;
        }
    }
    closedge[k].lowcost = 0; // Initialize the starting vertex

    for (i = 1; i < G.vexnum; i++)
    {
        // Find the vertex with the minimum cost
        VRType min = INFINITY;
        for (j = 0; j < G.vexnum; j++)
        {
            if (closedge[j].lowcost != 0 && closedge[j].lowcost < min)
            {
                min = closedge[j].lowcost;
                k = j;
            }
        }

        // Add the vertex to the MST
        printf("Edge (%c, %c) with weight %d\n", closedge[k].adjvex, G.vexs[k], closedge[k].lowcost);
        closedge[k].lowcost = 0;

        // Update the costs
        for (j = 0; j < G.vexnum; j++)
        {
            if (G.arcs[k][j].adj < closedge[j].lowcost)
            {
                closedge[j].adjvex = G.vexs[k];
                closedge[j].lowcost = G.arcs[k][j].adj;
            }
        }
    }
}
void printPath(int parent[], int j, MGraph G)
{
    // Base Case : If j is source
    if (parent[j] == -1)
        return;

    printPath(parent, parent[j], G);
    printf("%c ", G.vexs[j]);
}
void Dijkstra(MGraph G, VertexType u)
{
    int dist[MAX_VERTEX_NUM]; // The shortest distance from the source to each vertex
    bool sptSet[MAX_VERTEX_NUM]; // sptSet[i] will be true if vertex i is included in the shortest path tree
    int parent[MAX_VERTEX_NUM]; // To store the shortest path tree

    // Initialize all distances as INFINITY and sptSet[] as false
    for (int i = 0; i < G.vexnum; i++)
    {
        dist[i] = INFINITY;
        sptSet[i] = false;
        parent[i] = -1;
    }

    // Distance of source vertex from itself is always 0
    int src = LocateVex(G, u);
    dist[src] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < G.vexnum - 1; count++)
    {
        // Pick the minimum distance vertex from the set of vertices not yet processed
        int min = INFINITY, min_index;
        for (int v = 0; v < G.vexnum; v++)
            if (!sptSet[v] && dist[v] <= min)
            {
                min = dist[v];
                min_index = v;
            }

        // Mark the picked vertex as processed
        int u = min_index;
        sptSet[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex
        for (int v = 0; v < G.vexnum; v++)
            if (!sptSet[v] && G.arcs[u][v].adj && dist[u] != INFINITY && dist[u] + G.arcs[u][v].adj < dist[v])
            {
                dist[v] = dist[u] + G.arcs[u][v].adj;
                parent[v] = u;
            }
    }

    // Print the constructed distance array
    printf("Vertex\tDistance from Source\tPath");
    for (int i = 0; i < G.vexnum; i++)
    {
        printf("\n%c -> %c \t\t %d\t\t", G.vexs[src], G.vexs[i], dist[i]);
        printPath(parent, i, G);
    }
}


int main()
{
    int option = 0;
    MGraph G;
    printf("1. ��������������С������\n");
    printf("2. ���������������·��\n");
    printf("0. �˳�\n");
    printf("���������ѡ��:");
    scanf("%d",&option);
    switch (option)
    {
    case 1:
        printf("�빹��һ���ڽӾ���洢����������\n");
        createUDN(G);
        MST_PRIM(G,G.vexs[0]);
        break;
    case 2:
        printf("�빹��һ���ڽӾ���洢����������\n");
        createDN(G);
        Dijkstra(G,G.vexs[0]);
        break;
    }
    return 0;
}
