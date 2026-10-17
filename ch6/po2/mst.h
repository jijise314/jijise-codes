#ifndef MST_H
#define MST_H

#if defined(_MSC_VER)
#pragma execution_character_set("utf-8")
#endif

#include <stdbool.h>
#include <stddef.h>

#define NUM_V    50
#define SPARSE_E 100
#define DENSE_E  600
#define MAX_W    100
#define REPEAT   10000
#define NO_EDGE  0
#define MST_INF  0x3f3f3f3f

typedef struct { int u, v, w; } Edge;

typedef struct Node {
    int v, w;
    struct Node* next;
} Node;

typedef struct {
    Node* head[NUM_V];
    int   edges;
} AdjList;
 
typedef struct {
    int w[NUM_V][NUM_V];
    int edges;
} AdjMatrix;

typedef struct {
    Edge      edges[NUM_V - 1];
    int       count;
    long long cost;
} MSTResult;

typedef struct {
    size_t graph;
    size_t aux;
    size_t result;
    size_t total;
} MemInfo;

void generate_graph(Edge* out, int E);
void build_list(AdjList* g, const Edge* e, int E);
void free_list(AdjList* g);
void build_matrix(AdjMatrix* g, const Edge* e, int E);

void kruskal(const AdjList* g, MSTResult* res);
void prim(const AdjMatrix* g, MSTResult* res);

MemInfo mem_kruskal(const AdjList* g);
MemInfo mem_prim(const AdjMatrix* g);
double  time_kruskal(const AdjList* g, MSTResult* res);
double  time_prim(const AdjMatrix* g, MSTResult* res);

#endif
