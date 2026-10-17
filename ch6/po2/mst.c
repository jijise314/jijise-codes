#define _POSIX_C_SOURCE 199309L
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
#include <time.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mst.h"

#if defined(_MSC_VER)
#define NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE
#endif

static int rand_range(int lo, int hi) { return lo + rand() % (hi - lo + 1); }

static bool is_connected(bool used[NUM_V][NUM_V])
{
    bool visited[NUM_V] = { false };
    int queue[NUM_V], front = 0, rear = 0, cnt = 1;
    visited[0] = true;
    queue[rear++] = 0;
    while (front < rear) {
        int u = queue[front++];
        for (int v = 0; v < NUM_V; v++)
            if (used[u][v] && !visited[v]) {
                visited[v] = true;
                queue[rear++] = v;
                cnt++;
            }
    }
    return cnt == NUM_V;
}

void generate_graph(Edge* out, int E)
{
    static bool used[NUM_V][NUM_V];
    if (E < NUM_V - 1 || E > NUM_V * (NUM_V - 1) / 2) {
        fprintf(stderr, "잘못된 간선 수입니다: %d\n", E);
        exit(1);
    }
    do {
        memset(used, 0, sizeof(used));
        int perm[NUM_V], n = 0;
        for (int i = 0; i < NUM_V; i++) perm[i] = i;
        for (int i = NUM_V - 1; i > 0; i--) {
            int j = rand() % (i + 1), t = perm[i];
            perm[i] = perm[j]; perm[j] = t;
        }
        for (int i = 1; i < NUM_V; i++) {
            int a = perm[i], b = perm[rand() % i];
            used[a][b] = used[b][a] = true;
            out[n++] = (Edge){ a, b, rand_range(1, MAX_W) };
        }
        while (n < E) {
            int a = rand() % NUM_V, b = rand() % NUM_V;
            if (a == b || used[a][b]) continue;
            used[a][b] = used[b][a] = true;
            out[n++] = (Edge){ a, b, rand_range(1, MAX_W) };
        }
    } while (!is_connected(used));
}

static void push_node(AdjList* g, int u, int v, int w)
{
    Node* p = malloc(sizeof(Node));
    if (!p) { perror("malloc"); exit(1); }
    p->v = v; p->w = w; p->next = g->head[u];
    g->head[u] = p;
}

void build_list(AdjList* g, const Edge* e, int E)
{
    memset(g->head, 0, sizeof(g->head));
    g->edges = E;
    for (int i = 0; i < E; i++) {
        push_node(g, e[i].u, e[i].v, e[i].w);
        push_node(g, e[i].v, e[i].u, e[i].w);
    }
}

void free_list(AdjList* g)
{
    for (int i = 0; i < NUM_V; i++) {
        Node* p = g->head[i];
        while (p) { Node* n = p->next; free(p); p = n; }
        g->head[i] = NULL;
    }
}

void build_matrix(AdjMatrix* g, const Edge* e, int E)
{
    memset(g->w, 0, sizeof(g->w));
    g->edges = E;
    for (int i = 0; i < E; i++)
        g->w[e[i].u][e[i].v] = g->w[e[i].v][e[i].u] = e[i].w;
}

static int cmp_edge(const void* a, const void* b)
{
    const Edge* x = a, * y = b;
    if (x->w != y->w) return x->w - y->w;
    if (x->u != y->u) return x->u - y->u;
    return x->v - y->v;
}

static int uf_find(int* parent, int x)
{
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

static bool uf_union(int* parent, int* rnk, int a, int b)
{
    a = uf_find(parent, a);
    b = uf_find(parent, b);
    if (a == b) return false;
    if (rnk[a] < rnk[b]) { int t = a; a = b; b = t; }
    parent[b] = a;
    if (rnk[a] == rnk[b]) rnk[a]++;
    return true;
}

NOINLINE void kruskal(const AdjList* g, MSTResult* res)
{
    Edge* edges = malloc(sizeof(Edge) * g->edges);
    if (!edges) { perror("malloc"); exit(1); }
    int m = 0;
    for (int u = 0; u < NUM_V; u++)
        for (Node* p = g->head[u]; p; p = p->next)
            if (u < p->v) edges[m++] = (Edge){ u, p->v, p->w };

    qsort(edges, m, sizeof(Edge), cmp_edge);

    int parent[NUM_V], rnk[NUM_V];
    for (int i = 0; i < NUM_V; i++) { parent[i] = i; rnk[i] = 0; }

    res->count = 0;
    res->cost = 0;
    for (int i = 0; i < m && res->count < NUM_V - 1; i++)
        if (uf_union(parent, rnk, edges[i].u, edges[i].v)) {
            res->edges[res->count++] = edges[i];
            res->cost += edges[i].w;
        }
    free(edges);
}

NOINLINE void prim(const AdjMatrix* g, MSTResult* res)
{
    int  key[NUM_V];
    int  parent[NUM_V];
    bool inMST[NUM_V];
    for (int i = 0; i < NUM_V; i++) { key[i] = MST_INF; parent[i] = -1; inMST[i] = false; }
    key[0] = 0;

    res->count = 0;
    res->cost = 0;
    for (int iter = 0; iter < NUM_V; iter++) {
        int u = -1;
        for (int v = 0; v < NUM_V; v++)
            if (!inMST[v] && (u == -1 || key[v] < key[u])) u = v;
        if (u == -1 || key[u] == MST_INF) break;

        inMST[u] = true;
        if (parent[u] != -1) {
            res->edges[res->count++] = (Edge){ parent[u], u, key[u] };
            res->cost += key[u];
        }
        for (int v = 0; v < NUM_V; v++) {
            int w = g->w[u][v];
            if (w != NO_EDGE && !inMST[v] && w < key[v]) {
                key[v] = w;
                parent[v] = u;
            }
        }
    }
}

MemInfo mem_kruskal(const AdjList* g)
{
    MemInfo m;
    m.graph = sizeof(g->head)
        + 2 * (size_t)g->edges * sizeof(Node);
    m.aux = (size_t)g->edges * sizeof(Edge)
        + NUM_V * sizeof(int)
        + NUM_V * sizeof(int);
    m.result = sizeof(MSTResult);
    m.total = m.graph + m.aux + m.result;
    return m;
}

MemInfo mem_prim(const AdjMatrix* g)
{
    MemInfo m;
    m.graph = sizeof(g->w);
    m.aux = NUM_V * sizeof(int)
        + NUM_V * sizeof(int)
        + NUM_V * sizeof(bool);
    m.result = sizeof(MSTResult);
    m.total = m.graph + m.aux + m.result;
    return m;
}

static double now_us(void)
{
#ifdef _WIN32
    static LARGE_INTEGER freq;
    LARGE_INTEGER c;
    if (freq.QuadPart == 0) QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&c);
    return (double)c.QuadPart * 1e6 / (double)freq.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1e6 + ts.tv_nsec / 1e3;
#endif
}

static volatile long long sink;

double time_kruskal(const AdjList* g, MSTResult* res)
{
    kruskal(g, res);
    double t0 = now_us();
    for (int i = 0; i < REPEAT; i++) { kruskal(g, res); sink += res->cost; }
    return (now_us() - t0) / REPEAT;
}

double time_prim(const AdjMatrix* g, MSTResult* res)
{
    prim(g, res);
    double t0 = now_us();
    for (int i = 0; i < REPEAT; i++) { prim(g, res); sink += res->cost; }
    return (now_us() - t0) / REPEAT;
}
