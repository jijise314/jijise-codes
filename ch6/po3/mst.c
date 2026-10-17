#include <stdio.h>
#include <stdlib.h>
#include "mst.h"

static int randomWeight(void)
{
    return rand() % (MAX_W - MIN_W + 1) + MIN_W;
}

static int addEdge(Graph* g, int u, int v, int w)
{
    if (u == v || g->adj[u][v] != 0) return 0;
    g->adj[u][v] = g->adj[v][u] = w;
    g->edgeCount++;
    return 1;
}

void generateGraph(Graph* g)
{
    int order[NUM_V];
    int i, j, tmp, u, v;

    for (i = 0; i < NUM_V; i++)
        for (j = 0; j < NUM_V; j++)
            g->adj[i][j] = 0;
    g->edgeCount = 0;

    for (i = 0; i < NUM_V; i++) order[i] = i;
    for (i = NUM_V - 1; i > 0; i--) {
        j = rand() % (i + 1);
        tmp = order[i]; order[i] = order[j]; order[j] = tmp;
    }

    for (i = 1; i < NUM_V; i++) {
        u = order[i];
        v = order[rand() % i];
        addEdge(g, u, v, randomWeight());
    }

    while (g->edgeCount < NUM_E) {
        u = rand() % NUM_V;
        v = rand() % NUM_V;
        addEdge(g, u, v, randomWeight());
    }

    g->edgeCount = 0;
    for (i = 0; i < NUM_V; i++) {
        for (j = i + 1; j < NUM_V; j++) {
            if (g->adj[i][j] != 0) {
                g->edges[g->edgeCount].u = i;
                g->edges[g->edgeCount].v = j;
                g->edges[g->edgeCount].w = g->adj[i][j];
                g->edgeCount++;
            }
        }
    }
}

void floydWarshall(Graph* g)
{
    int i, j, k;

    /* 초기화 */
    for (i = 0; i < NUM_V; i++) {
        for (j = 0; j < NUM_V; j++) {
            if (i == j) {
                g->dist[i][j] = 0;
                g->next[i][j] = j;
            }
            else if (g->adj[i][j] != 0) {
                g->dist[i][j] = g->adj[i][j];
                g->next[i][j] = j;
            }
            else {
                g->dist[i][j] = INF;
                g->next[i][j] = NO_VERTEX;
            }
        }
    }

    for (k = 0; k < NUM_V; k++) {
        for (i = 0; i < NUM_V; i++) {
            if (g->dist[i][k] == INF) continue;
            for (j = 0; j < NUM_V; j++) {
                if (g->dist[k][j] == INF) continue;
                if (g->dist[i][k] + g->dist[k][j] < g->dist[i][j]) {
                    g->dist[i][j] = g->dist[i][k] + g->dist[k][j];
                    g->next[i][j] = g->next[i][k];
                }
            }
        }
    }
}

int reconstructPath(const Graph* g, int u, int v, int path[])
{
    int len = 0;

    if (g->next[u][v] == NO_VERTEX) return 0;

    path[len++] = u;
    while (u != v) {
        u = g->next[u][v];
        path[len++] = u;
    }
    return len;
}

void printGraph(const Graph* g)
{
    int i;

    printf("그래프\n");
    printf("정점 수 : %d\n", NUM_V);
    printf("간선 수 : %d\n\n", g->edgeCount);

    printf("간선 목록\n");
    for (i = 0; i < g->edgeCount; i++)
        printf("(%d, %d) 가중치: %d\n", g->edges[i].u, g->edges[i].v, g->edges[i].w);
    printf("\n");
}

void printShortestPaths(const Graph* g)
{
    int u, v, i, len;
    int path[NUM_V];
    int pairCount = 0;

    printf("최단경로\n\n");

    for (u = 0; u < NUM_V; u++) {
        for (v = u + 1; v < NUM_V; v++) {
            len = reconstructPath(g, u, v, path);

            printf("%d -> %d\n", u, v);
            printf("경로     : ");
            for (i = 0; i < len; i++) {
                printf("%d", path[i]);
                if (i < len - 1) printf(" -> ");
            }
            printf("\n");
            printf("최단거리 : %d\n\n", g->dist[u][v]);

            pairCount++;
        }
    }

    printf("전체 정점 쌍 수 : %d\n", pairCount);
}
