#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

static unsigned int rng_state = 1;

void rng_init(unsigned int s) { rng_state = s ? s : 1; }

unsigned int rng_next(void)
{
    unsigned int x = rng_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return rng_state = x;
}

int rng_range(int n) { return (int)(rng_next() % (unsigned int)n); }

void generate_edges(Edge* out, int E)
{
    static Edge pairs[MAX_E];
    int k = 0;
    for (int u = 0; u < V; u++)
        for (int v = u + 1; v < V; v++) {
            pairs[k].u = u;
            pairs[k].v = v;
            k++;
        }
    for (int i = MAX_E - 1; i > 0; i--) {
        int j = rng_range(i + 1);
        Edge t = pairs[i]; pairs[i] = pairs[j]; pairs[j] = t;
    }
    for (int i = 0; i < E; i++) {
        if (rng_next() & 1u) { out[i].u = pairs[i].v; out[i].v = pairs[i].u; }
        else                 out[i] = pairs[i];
    }
}

AdjMatrix* matrix_create(void)
{
    AdjMatrix* g = (AdjMatrix*)malloc(sizeof(AdjMatrix));
    if (!g) { fprintf(stderr, "out of memory\n"); exit(1); }
    g->n = V;
    g->edges = 0;
    memset(g->adj, 0, sizeof(g->adj));
    return g;
}

void matrix_destroy(AdjMatrix* g) { free(g); }

int matrix_has_edge(const AdjMatrix* g, int u, int v, long* cmp)
{
    (*cmp)++;
    return g->adj[u][v];
}

int matrix_insert_edge(AdjMatrix* g, int u, int v, long* cmp)
{
    if (matrix_has_edge(g, u, v, cmp)) return 0;
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
    g->edges++;
    return 1;
}

int matrix_delete_edge(AdjMatrix* g, int u, int v, long* cmp)
{
    if (!matrix_has_edge(g, u, v, cmp)) return 0;
    g->adj[u][v] = 0;
    g->adj[v][u] = 0;
    g->edges--;
    return 1;
}

int matrix_neighbors(const AdjMatrix* g, int u, int* out, long* cmp)
{
    int k = 0;
    for (int v = 0; v < g->n; v++) {
        (*cmp)++;
        if (g->adj[u][v]) out[k++] = v;
    }
    return k;
}

size_t matrix_memory(const AdjMatrix* g)
{
    (void)g;
    return sizeof(AdjMatrix);
}

AdjList* list_create(void)
{
    AdjList* g = (AdjList*)malloc(sizeof(AdjList));
    if (!g) { fprintf(stderr, "out of memory\n"); exit(1); }
    g->n = V;
    g->edges = 0;
    g->nodes = 0;
    for (int i = 0; i < V; i++) g->head[i] = NULL;
    return g;
}

void list_destroy(AdjList* g)
{
    for (int i = 0; i < g->n; i++) {
        Node* p = g->head[i];
        while (p) { Node* nx = p->next; free(p); p = nx; }
    }
    free(g);
}

int list_find(const AdjList* g, int u, int v, long* cmp)
{
    for (const Node* p = g->head[u]; p; p = p->next) {
        (*cmp)++;
        if (p->vertex == v) return 1;
    }
    return 0;
}

static void list_push_front(AdjList* g, int u, int v)
{
    Node* nd = (Node*)malloc(sizeof(Node));
    if (!nd) { fprintf(stderr, "out of memory\n"); exit(1); }
    nd->vertex = v;
    nd->next = g->head[u];
    g->head[u] = nd;
    g->nodes++;
}

int list_insert_edge(AdjList* g, int u, int v, long* cmp)
{
    if (list_find(g, u, v, cmp)) return 0;
    list_push_front(g, u, v);
    list_push_front(g, v, u);
    g->edges++;
    return 1;
}

static int list_remove_node(AdjList* g, int u, int v, long* cmp)
{
    Node* prev = NULL;
    for (Node* p = g->head[u]; p; prev = p, p = p->next) {
        (*cmp)++;
        if (p->vertex == v) {
            if (prev) prev->next = p->next;
            else      g->head[u] = p->next;
            free(p);
            g->nodes--;
            return 1;
        }
    }
    return 0;
}

int list_delete_edge(AdjList* g, int u, int v, long* cmp)
{
    if (!list_remove_node(g, u, v, cmp)) return 0;
    list_remove_node(g, v, u, cmp);
    g->edges--;
    return 1;
}

int list_neighbors(const AdjList* g, int u, int* out, long* cmp)
{
    int k = 0;
    for (const Node* p = g->head[u]; p; p = p->next) {
        (*cmp)++;
        out[k++] = p->vertex;
    }
    return k;
}

size_t list_memory(const AdjList* g)
{
    return sizeof(AdjList) + (size_t)g->nodes * sizeof(Node);
}

Average average_matrix(const AdjMatrix* g)
{
    Average a = { 0, 0, 0, 0 };
    long c, n;
    int buf[V];

    c = 0; n = 0;
    for (int u = 0; u < V; u++)
        for (int v = 0; v < V; v++)
            if (u != v && !g->adj[u][v]) { matrix_has_edge(g, u, v, &c); n++; }
    a.ins = n ? (double)c / n : 0;

    c = 0; n = 0;
    for (int u = 0; u < V; u++)
        for (int v = 0; v < V; v++)
            if (g->adj[u][v]) { matrix_has_edge(g, u, v, &c); n++; }
    a.del = n ? (double)c / n : 0;

    c = 0; n = 0;
    for (int u = 0; u < V; u++)
        for (int v = 0; v < V; v++)
            if (u != v) { matrix_has_edge(g, u, v, &c); n++; }
    a.conn = (double)c / n;

    c = 0;
    for (int u = 0; u < V; u++) matrix_neighbors(g, u, buf, &c);
    a.adj = (double)c / V;
    return a;
}

Average average_list(const AdjList* g, const AdjMatrix* ref)
{
    Average a = { 0, 0, 0, 0 };
    long c, n;
    int buf[V];

    c = 0; n = 0;
    for (int u = 0; u < V; u++)
        for (int v = 0; v < V; v++)
            if (u != v && !ref->adj[u][v]) { list_find(g, u, v, &c); n++; }
    a.ins = n ? (double)c / n : 0;

    c = 0; n = 0;
    for (int u = 0; u < V; u++)
        for (int v = 0; v < V; v++)
            if (ref->adj[u][v]) { list_find(g, u, v, &c); list_find(g, v, u, &c); n++; }
    a.del = n ? (double)c / n : 0;

    c = 0; n = 0;
    for (int u = 0; u < V; u++)
        for (int v = 0; v < V; v++)
            if (u != v) { list_find(g, u, v, &c); n++; }
    a.conn = (double)c / n;

    c = 0;
    for (int u = 0; u < V; u++) list_neighbors(g, u, buf, &c);
    a.adj = (double)c / V;
    return a;
}
