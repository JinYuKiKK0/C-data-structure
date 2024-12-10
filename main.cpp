#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX_NUM 20 // ��󶥵���
#define INFINITY 65535    // �����
#define VRType int        // �����ϵ����
#define VertexType char   // ��������

typedef enum { DG, DN, UDG, UDN } GraphKind; // {����ͼ��������������ͼ��������}
typedef struct ArcCell {
    VRType adj; // VRType �Ƕ����ϵ���ͣ�����Ȩͼ����1��0��ʾ�Ƿ����ڣ��Դ�Ȩͼ����ΪȨֵ����
} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef struct {
    VertexType vexs[MAX_VERTEX_NUM]; // ��������
    AdjMatrix arcs;                  // �ڽӾ���
    int vexnum, arcnum;              // ͼ�ĵ�ǰ�������ͻ���
    GraphKind kind;                  // ͼ�������־
} MGraph;

int LocateVex(MGraph G, VertexType u) {
    for (int i = 0; i < G.vexnum; ++i)
        if (G.vexs[i] == u)
            return i;
    return -1;
}

void createUDN(MGraph *G) {
    printf("�����붥�����ͱ������Կո������");
    scanf("%d %d", &G->vexnum, &G->arcnum);
    printf("������%d�����㣺", G->vexnum);
    for (int i = 0; i < G->vexnum; i++)
        scanf(" %c", &G->vexs[i]);
    for (int i = 0; i < G->vexnum; i++)
        for (int j = 0; j < G->vexnum; j++)
            G->arcs[i][j].adj = INFINITY;
    // �����ڽӾ���
    for (int k = 0; k < G->arcnum; k++) {
        char v1, v2;
        int w, i, j;
        printf("�������%d���ߵ��������㼰���ϵ�Ȩֵ���Կո������", k + 1);
        scanf(" %c %c %d", &v1, &v2, &w);
        i = LocateVex(*G, v1);
        j = LocateVex(*G, v2);
        if (i != -1 && j != -1) {
            G->arcs[i][j].adj = w;
            G->arcs[j][i].adj = w; // ����ͼ�ԳƸ�ֵ
        } else {
            printf("�����������\n");
            k--; // �������������
        }
    }
    G->kind = UDN;
}

void createDN(MGraph *G) {
    int i, j, k, w;
    printf("�����붥�����ͻ�����");
    scanf("%d %d", &G->vexnum, &G->arcnum);
    printf("������%d�����㣺", G->vexnum);
    for (i = 0; i < G->vexnum; i++)
        scanf(" %c", &G->vexs[i]);
    for (i = 0; i < G->vexnum; i++)
        for (j = 0; j < G->vexnum; j++)
            G->arcs[i][j].adj = INFINITY;
    // �����ڽӾ���
    for (k = 0; k < G->arcnum; k++) {
        char v1, v2;
        printf("�������%d���������,�յ��Ȩֵ���Կո������", k + 1);
        scanf(" %c %c %d", &v1, &v2, &w);
        i = LocateVex(*G, v1);
        j = LocateVex(*G, v2);
        if (i != -1 && j != -1) {
            G->arcs[i][j].adj = w;
        } else {
            printf("�����������\n");
            k--; // �������������
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

    // ��λ��ʼ����
    k = LocateVex(G, u);
    if (k == -1) {
        printf("��ʼ���㲻���ڡ�\n");
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
        // �ҳ�lowcost��С��j
        VRType min = INFINITY;
        int minIndex = -1;
        for (j = 0; j < G.vexnum; j++) {
            if (closedge[j].lowcost != 0 && closedge[j].lowcost < min) {
                min = closedge[j].lowcost;
                minIndex = j;
            }
        }
        if (minIndex == -1) {
            printf("ͼ����ͨ����С�����������ڡ�\n");
            return;
        }
        // ���ѡȡ�ı�
        printf("�� (%c, %c) Ȩֵ %d\n", closedge[minIndex].adjvex, G.vexs[minIndex], closedge[minIndex].lowcost);
        closedge[minIndex].lowcost = 0;
        // ����closedge
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
    int dist[MAX_VERTEX_NUM];    // Դ���㵽�����������̾���
    int sptSet[MAX_VERTEX_NUM];  // sptSet[i]Ϊ1��ʾ����i���ҵ����·��
    int parent[MAX_VERTEX_NUM];  // ��¼·��

    for (int i = 0; i < G.vexnum; i++) {
        dist[i] = INFINITY;
        sptSet[i] = 0;
        parent[i] = -1;
    }

    int src = LocateVex(G, src_vertex);
    if (src == -1) {
        printf("Դ���㲻���ڡ�\n");
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
            break; // �޷���������

        int u_index = min_index;
        sptSet[u_index] = 1;

        for (int v = 0; v < G.vexnum; v++)
            if (sptSet[v] == 0 && G.arcs[u_index][v].adj != INFINITY && dist[u_index] != INFINITY && dist[u_index] + G.arcs[u_index][v].adj < dist[v]) {
                dist[v] = dist[u_index] + G.arcs[u_index][v].adj;
                parent[v] = u_index;
            }
    }

    // ��ӡ���
    printf("����\t����\t·��");
    for (int i = 0; i < G.vexnum; i++) {
        printf("\n%c -> %c \t%d\t", G.vexs[src], G.vexs[i], dist[i]);
        if (dist[i] != INFINITY) {
            printPath(parent, i, G);
        } else {
            printf("���ɴ�");
        }
    }
    printf("\n");
}

int main() {
    int option = 0;
    MGraph G;
    printf("1. ��������������С������\n");
    printf("2. ���������������·��\n");
    printf("0. �˳�\n");
    printf("���������ѡ��");
    scanf("%d", &option);
    switch (option) {
    case 1:
        printf("�빹��һ���ڽӾ���洢����������\n");
        createUDN(&G);
        MST_PRIM(G, G.vexs[0]);
        break;
    case 2:
        printf("�빹��һ���ڽӾ���洢����������\n");
        createDN(&G);
        Dijkstra(G, G.vexs[0]);
        break;
    case 0:
        printf("�����˳���\n");
        break;
    default:
        printf("��������\n");
        break;
    }
    return 0;
}